#include "Segmentation.h"

void Log(string message)
{
    std::fstream outputFile;
    outputFile.open("Log.txt", ios::app);
    if(outputFile.is_open())
    outputFile << message.c_str() << std::endl;
    outputFile.close(); 
}

map<int, int> Segmentation::TuningBackground(Area A, bool printParametrs)
{
    printf("TuningBackground: \n");
    map<int, int> Stat;
    vector<pair<int, int>> Stat1;
    for (uint32_t y = A.y1; y < A.y2; y++)
		for (uint32_t x = A.x1; x < A.x2; x++)
        {
            TColorPix Px = Img->Pix(x,y);
            int P = *reinterpret_cast<int*>(&Px);
            if (Stat.count(P)) {Stat[P]++;}
            else Stat[P] = 1;    
        }

    FindSegmentSize.x = A.x2 - A.x1;
    FindSegmentSize.y = A.y2 - A.y1;
    FindSegmentBrightness = Img->BrightnessArea(A);
   // fon = MediumColorContur(Img, {A.x1, A.y1}, SizeSegment.x, SizeSegment.y); 
    fon = MediumColorAreal(Img, {A.x1, A.y1}, FindSegmentSize.x, FindSegmentSize.y);
    
   return Stat ;
}

shared_ptr<IMG> Segmentation::Convolution(shared_ptr<IMG> InImg, int Kernel_width, int Kernel_height, std::vector<int> convKernel)
{
    auto Res = make_shared<IMG>(InImg->width, InImg->height);

    int grX = (Kernel_width - 1) / 2;
    int grY = (Kernel_height - 1) / 2;
    
    for (int y = 0; y < InImg->height; y++)
    {
        for (int x = 0; x < InImg->width; x++)
        {
            //int pix = 0;
            int b=0; int g=0; int r=0; 
            for (int ky = -grY; ky <= grY; ky++)
            {
                for (int kx = -grX; kx <= grX; kx++)
                {
                    //int vP = 0; 
                    if (x + kx < 0 || y + ky < 0
                        || x + kx >= InImg->width
                        || y + ky >= InImg->height) {b=0; g=0; r=0;}
                    else
                    {     
                    //vP = InImg->PixToInt(x + kx, y + ky) * convKernel.at(kx + grX + (ky+grY) * Kernel_width);
                        r+= (int)InImg->Pix(x + kx, y + ky).Red * convKernel.at(kx + grX + (ky+grY) * Kernel_width);
                        g+= (int)InImg->Pix(x + kx, y + ky).Green * convKernel.at(kx + grX + (ky+grY) * Kernel_width);
                        b+= (int)InImg->Pix(x + kx, y + ky).Blue * convKernel.at(kx + grX + (ky+grY) * Kernel_width);    
                    }
                   // pix += vP;
                    
                }
            }
            TColorPix pix;
            pix.IntsToColor(b, g, r);    
            Res->Set(x, y, pix); 
        }
    }
   return Res;
}

shared_ptr<IMG> Segmentation::DetectBorders(shared_ptr<IMG> InImg)
{
    shared_ptr<IMG> In;
    if(InImg != nullptr) In = InImg; else In = Img;

    int Kernel_height = 3;
    int Kernel_width = 3;
    std::vector<int> convKernel1 = { 1, 0, -1, 1, 0, -1, 1, 0, -1 };
    OutImg = Convolution(In, Kernel_width, Kernel_height, convKernel1);
    
    std::vector<int> convKernel2 = { 1, 1, 1, 0, 0, 0, -1, -1, -1 };
    shared_ptr<IMG> Img_DB = Convolution(In, Kernel_width, Kernel_height, convKernel2);
    
    auto it2 = Img_DB->begin();
    for(auto out = OutImg->begin(); out < OutImg->end(); out++) {out->Red |= it2->Red; out->Green |= it2->Green; out->Blue |= it2->Blue; it2++;};

    return  OutImg;
}

/*TArea Segmentation::FindColorArea(shared_ptr<IMG> InImg)
{
    shared_ptr<IMG> In;
    if(InImg != nullptr) In = InImg; else In = Img;
    TArea R;
    TPoint minP; int minDif = 100;
    unsigned int width = SizeSegment.x;
    
    for (unsigned int y = 0; y < In->height-2; y++)
    {
        for (unsigned int x = 0; x < In->width - width; x++)
        {
           // TColorPix Cp = MediumColorContur(In, {x, y}, SizeSegment.x, SizeSegment.y); 
            TColorPix Cp = MediumColorAreal(In, {x, y}, SizeSegment.x, SizeSegment.y);
            TColorPix dif =  CompareColors(Cp, fon);   
            if (dif < minDif ) {minP = {x, y}; minDif = dif;}  
            if (dif == 0) {minP = {x, y}; minDif = dif; break; break;}   
        }
    }
    TPoint maxP = {minP.x + SizeSegment.x, minP.y + SizeSegment.y};
    if(maxP.x > In->width) maxP.x = In->width-1;
    if(maxP.y > In->height) maxP.y = In->height-1;
  //  for (unsigned int x = minP.x + width; x < In->width; x++) if (CompareColors(In->Pix(x, minP.y+1), fon) <= minDif) maxP.x = x; else break;
  //  for (unsigned int y = minP.y + 1; y < In->height; y++) if (CompareColors(In->Pix(minP.x+1, y), fon) <= minDif) maxP.y = y; else break;

    return  {minP.x, minP.y, maxP.x, maxP.y} ;  
}*/

shared_ptr<IMG> Segmentation::Difference(shared_ptr<IMG> Img1, shared_ptr<IMG> Img2, int &count)
{
    shared_ptr<IMG> Res = make_shared<IMG>(Img1->width, Img1->height);
    auto im1 = Img1->begin();
    auto im2 = Img2->begin();
    count = 0;
    for(auto r=Res->begin(); r < Res->end(); r++) 
    {
        TColorPix dif = CompareColors(*im1, *im2);
        if(dif.Red + dif.Green + dif.Blue > 200) {*r = {0xff, 0xff, 0xff, 0xff}; count++;}
        else *r = {0, 0, 0, 0};
     //   r->Red = abs(im1->Red - im2->Red);
     //   r->Green = abs(im1->Green - im2->Green);
     //   r->Blue = abs(im1->Blue - im2->Blue);
        im1++; im2++;
    }
    return Res;
}

TColorPix Segmentation::RandomColor()
{
    static uint8_t num=1;
    uint8_t rn = rand()%150 + 30;
    TColorPix ColorM = {rn, 10, rn, 0};
    if (num==1) ColorM = {rn, 10, 10, 0};    
    if (num==2) ColorM = {30, rn, 10, 0}; 
    if (num==3) ColorM = {30, 20, rn, 0}; 
    if (num==4) ColorM = {30, rn, rn, 0};
    if(++num > 4) num = 1;
    return  ColorM ;
}

shared_ptr<IMG> Segmentation::PaintClusters(shared_ptr<IMG> InImg, uint32_t deep, vector<TArea>& Clusters, TColorPix findColor)
{
    shared_ptr<IMG> out = make_shared<IMG>(InImg->width, InImg->height); 
    srand(time(NULL));

    for (int k = 0; k < InImg->width * InImg->height ; k++)
      if(InImg->at(k) == findColor && out->at(k).toInt() == 0)
      {
         vector<TPoint> loc;
         loc.push_back({k % InImg->width, k / InImg->width});    
         TArea A = {loc.front().x, loc.front().y, loc.front().x, loc.front().y};     
         TColorPix ColorCl = RandomColor();    
  
         while(!loc.empty())
         {  
            auto it = loc.begin(); 
            uint32_t x0 = it->x;
            uint32_t y0 = it->y;
            
            if(A.x1 > x0) A.x1 = x0;
            if(A.y1 > y0) A.y1 = y0;
            if(A.x2 < x0) A.x2 = x0;
            if(A.y2 < y0) A.y2 = y0;
            
            out->Set(x0, y0, ColorCl);     
            loc.erase(it);  
            uint32_t Xbegin = (x0 >= deep) ? x0 - deep : 0;
            uint32_t Ybegin = (y0 >= deep) ? y0 - deep : 0;
            uint32_t Xend = (x0 + deep < InImg->width) ?  x0 + deep : InImg->width-1;
            uint32_t Yend = (y0 + deep < InImg->height) ? y0 + deep : InImg->height-1;
    
            for(int y = Ybegin; y <= Yend; y++)    
                for(int x = Xbegin; x <= Xend; x++) 
                    if(InImg->Pix(x, y) == findColor && out->Pix(x, y).toInt() == 0 ) 
                    {  out->Set(x, y, ColorCl); 
                       loc.push_back({x, y});
                    }                 
         }  
         //Clusters.push_back(A);    
         if ((A.x2-A.x1 > 3 && A.y2-A.y1 > 3)) {Clusters.push_back(A); 
                                     Log(" x1 = "+to_string(A.x1)+" y1 = "+to_string(A.y1)+" x2 = "+to_string(A.x2)+" y2 = "+to_string(A.y2));}
    }
    return out;
}

bool Segmentation::FilterSizeClusters(TArea A)
{
   if (A.x2-A.x1 < 4 || A.y2-A.y1 < 4) return false;
 
   double S_A = (A.x2 - A.x1)*(A.y2 - A.y1); 
   double S = (double)FindSegmentSize.x * (double)FindSegmentSize.y ;
 
   if  ((S_A  >= 0.7*S) && (S_A  <= 1.3*S) ) return true; 
   
   return false; 
}

bool Segmentation::FilterBrightnessClusters(TArea A)
{
  double Bcl = Img->BrightnessArea(A); 
  if  ((Bcl  >= 0.8*FindSegmentBrightness) && (Bcl  <= 1.2*FindSegmentBrightness) ) return true;    
  return false;   
}

int Segmentation::CompareFrames() // сравнение последовательно идущих кадров
{
    static shared_ptr<IMG> Previous = nullptr;
    static shared_ptr<IMG> Dif = nullptr;
    int countDif = -1;
    int procent = -1;
    if (Previous != nullptr) 
    { 
        Dif = Difference(Previous, Img, countDif); 
        procent = countDif * 100 / Dif->size();
    };  
    Previous = Img;
    return  procent;
}

shared_ptr<IMG> Segmentation::FastDetectBorders(shared_ptr<IMG> InImg, int Level)
{
    shared_ptr<IMG> In; 
    if(InImg != nullptr) In = InImg; else In = Img;
    shared_ptr<IMG> out = make_shared<IMG>(In->width, In->height);

    for (unsigned int y = 1; y < In->height-1; y++)
    {
        for (unsigned int x = 1; x < In->width-1; x++)
        {
           TColorPix dif[2];
           dif[0] = CompareColors(In->Pix(x-1, y), In->Pix(x+1, y)); 
           dif[1] = CompareColors(In->Pix(x, y-1), In->Pix(x, y+1)); 
           
           if(Level == 0) { dif[0].Red |= dif[1].Red;  dif[0].Green |= dif[1].Green; dif[0].Blue |= dif[1].Blue; out->Set(x, y, dif[0]) ;}
           else 
            { int d = dif[0].Red + dif[1].Red + dif[0].Green + dif[1].Green + dif[0].Blue + dif[1].Blue;
                if(d > Level) out->Set(x, y, {0xff, 0xff, 0xff, 0xff}) ;
                else out->Set(x, y, {0, 0, 0, 0}) ;
            }
        }
    }
    return out;
}


