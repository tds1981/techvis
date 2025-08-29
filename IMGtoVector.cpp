#include "IMGtoVector.h"

VectorPoints::VectorPoints(shared_ptr<IMG> Img, TColorPix baseColor) 
{
     width = Img->width;  height = Img->height; 
     GetDirectionsForPoints(Img, baseColor);
     MapStrToLines(); 
     DeleteShortLines(2);  
}

void VectorPoints::Get1(shared_ptr<IMG> Img, TColorPix baseColor) 
{    
     bool seria = false;  
     for (int y = 0; y < height; y++)
     {
       int countSeria=0;  
       for (int x = 0; x < width; x++) 
       {  
          if (Img->Pix(x, y) == baseColor) { countSeria++; };          
          if ((x+1==width || Img->PixLim(x+1, y)!= baseColor) && countSeria != 0)  {push_back({x-countSeria-1, y, countSeria-1, 0}); countSeria=0;};
          
       } 
     }   
}

void VectorPoints::GetDirectionsForPoints(shared_ptr<IMG> Img, TColorPix baseColor) 
{    
    shared_ptr<IMG> out = make_shared<IMG>(width, height); 
    TColorPix ColorCl = {255, 255, 255, 255};
    uint32_t deep = 1;

    for (int k = 0; k < width * height ; k++)
      if(Img->at(k) == baseColor && out->at(k).toInt() == 0)
      {
         uint32_t x0 = k % width;
         uint32_t y0 = k / width;
         vector<TPoint> loc;
         loc.push_back({x0, y0}); 
           
         while(!loc.empty())
         {
            auto it = loc.begin();
            x0 = it->x;
            y0 = it->y;
            loc.erase(it);  
            uint32_t Xbegin = (x0 >= deep) ? x0 - deep : 0;
            uint32_t Ybegin = (y0 >= deep) ? y0 - deep : 0;
            uint32_t Xend = (x0 + deep < width) ?  x0 + deep : width-1;
            uint32_t Yend = (y0 + deep < height) ? y0 + deep : height-1;
    
            for(int y = Ybegin; y <= Yend; y++)    
                for(int x = Xbegin; x <= Xend; x++) 
                    if(Img->Pix(x, y) == baseColor && out->Pix(x, y).toInt() == 0 ) 
                    {  out->Set(x, y, ColorCl); 
                       loc.push_back({x, y});
                       strPoints[y0].push_back({x0, y0, x-(int)x0, y-(int)y0}); 
                    }         
         }     
    }  
}

void VectorPoints::MapStrToLines()
{
   for(auto& Y1 : strPoints)
    {  
      for(auto& Y2 : strPoints)
      {
        if(Y1.first < Y2.first)
        {
            for(auto v1 = Y1.second.begin(); v1 < Y1.second.end(); v1++)
                for(auto v2 = Y2.second.begin(); v2 < Y2.second.end(); )
                    if(v1->x + v1->dx == v2->x && v1->y + v1->dy == v2->y) 
                       {  TPointInVector PV;
                          PV.x = v1->x ; 
                          PV.y = v1->y ;
                          PV.dx = v1->dx + v2->dx; 
                          PV.dy = v1->dy + v2->dy;
                          *v1 = PV;   
                          v2 = Y2.second.erase(v2);
                       } else v2++;
            // if(Y2.second.empty()) Y2=strPoints.erase(Y2.first); 
        } 
      }
    }
    
    for(auto& S : strPoints) insert(end(), S.second.begin(), S.second.end());
}

shared_ptr<IMG> VectorPoints::GetImg(bool lineOrpoints)
{
    shared_ptr<IMG> R = make_shared<IMG>(width, height); 
    for(auto it = begin(); it < end(); it++)
    if (lineOrpoints)
    { 
       unsigned int CountT = static_cast<unsigned int>(ceil( sqrt ( pow(1+it->dx, 2) + pow(1+it->dy, 2) )));   
       for(unsigned int t=0; t < CountT; t++)
       {
           unsigned int x = ((CountT - t)*it->x + t*(it->x + it->dx)) / CountT; 
           unsigned int y = ((CountT - t)*it->y + t*(it->y + it->dy)) / CountT; 
           R->Set(x, y, {0xff, 0xff, 0xff, 0xff});
       }
    }
    else R->Set(it->x, it->y, {0xff, 0xff, 0xff, 0xff});
    return R;
} 

void VectorPoints::DeleteShortLines(unsigned int filterLength)
{
    SortLength();
    auto it = begin(); 
    while (pow(it->dx, 2)+pow(it->dy, 2) < filterLength * filterLength) it++; 
    erase(begin(), it);
}

void VectorPoints::PrintLog(char typeSort)
{
  //  if (typeSort == 'x') std::sort(begin(*this), end(*this))

    std::fstream outputFile;
    outputFile.open("f/seg/LogVector.txt", ios::app);
    if(outputFile.is_open())
    {
        for (auto it = begin(); it < end(); it++) 
        outputFile << "x= "<< to_string(it->x) << " y= "<< to_string(it->y) << " dx= "<< to_string(it->dx) << " dy= "<< to_string(it->dy) <<std::endl;
    }
    outputFile.close(); 
}

