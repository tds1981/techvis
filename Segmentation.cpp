#include "Segmentation.h"

void PrintMap(string namef)
{
 /*   std::ofstream outputFile(namef);
    outputFile << "Stat Fon :  x1=" << A.x1 <<" y1= " << A.y1 << " x2= " << A.x2 << " y2 = " << A.y2 << std::endl;
    outputFile << "Size area : "<< (A.x2 - A.x1)*(A.y2 - A.y1) << " x= " << A.x2 - A.x1 <<" y= " << A.y2 - A.y1  << std::endl;    
   // outputFile << "Color fon : R = "<< fon.Red << " G = " << fon.Green <<" B = " << fon.Blue  << std::endl;
    if(outputFile.is_open())
        for(auto it{Stat.begin()}; it != Stat.end(); it++) 
            if (it->second >1)
            outputFile << "RGB = " << std::hex << it->first << " , count = "<< std::dec << it->second << std::endl;
            //printf("RGB = %x , count = %i \n", it->first,  it->second);
   outputFile.close(); */
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

    SizeSegment.x = A.x2 - A.x1;
    SizeSegment.y = A.y2 - A.y1;
   // fon = MediumColorContur(Img, {A.x1, A.y1}, SizeSegment.x, SizeSegment.y); 
    fon = MediumColorAreal(Img, {A.x1, A.y1}, SizeSegment.x, SizeSegment.y);
    
   return Stat ;
}
/*
shared_ptr<IMG> Segmentation::Monochrom(shared_ptr<IMG> InImg, int Level)
{
   shared_ptr<IMG> In;
   if (InImg==nullptr) In = Img; else In = InImg;   
   OutImg = make_shared<IMG>(In->width, In->height); 
   auto out = OutImg->begin();  
   for (auto it = In->begin(); it < In->end(); it++)
       if (it->Blue + it->Green + it->Red > Level) { out->Blue = 255; out->Green = 255; out->Red = 255; out++;}
       else { out->Blue = 0; out->Green = 0; out->Red = 0; out++;}
   return OutImg;
}*/

shared_ptr<IMG> Segmentation::Convolution(shared_ptr<IMG> InImg, int Kernel_width, int Kernel_height, std::vector<int> convKernel)
{
    auto Res = make_shared<IMG>(InImg->width, InImg->height);

    int grX = (Kernel_width - 1) / 2;
    int grY = (Kernel_height - 1) / 2;
    
    for (int y = 0; y < InImg->height; y++)
    {
        for (int x = 0; x < InImg->width; x++)
        {
            int pix = 0;
            
            for (int ky = -grY; ky <= grY; ky++)
            {
                for (int kx = -grX; kx <= grX; kx++)
                {
                    int vP = 0;
                    if (x + kx < 0 || y + ky < 0
                        || x + kx >= InImg->width
                        || y + ky >= InImg->height) vP = 0;
                    else vP = InImg->PixToInt(x + kx, y + ky) * convKernel.at(kx + grX + (ky+grY) * Kernel_width);
                    pix += vP;
                    
                }
            }
            Res->Set(x, y, *reinterpret_cast<TColorPix*> (&pix) ); 
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

TArea Segmentation::FindColorArea(shared_ptr<IMG> InImg)
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
            int dif =  CompareColors(Cp, fon);   
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
}

shared_ptr<IMG> Segmentation::Difference(shared_ptr<IMG> Img1, shared_ptr<IMG> Img2, int &count)
{
    shared_ptr<IMG> Res = make_shared<IMG>(Img1->width, Img1->height);
    auto im1 = Img1->begin();
    auto im2 = Img2->begin();
    count = 0;
    for(auto r=Res->begin(); r < Res->end(); r++) 
    {
        if(CompareColors(*im1, *im2)>50) {*r = {0xff, 0xff, 0xff, 127}; count++;}
        else *r = {0, 0, 0, 127};
     //   r->Red = abs(im1->Red - im2->Red);
     //   r->Green = abs(im1->Green - im2->Green);
     //   r->Blue = abs(im1->Blue - im2->Blue);
        im1++; im2++;
    }
    return Res;
}
