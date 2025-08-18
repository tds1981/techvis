#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <memory>
#include <map>
#include <cmath>
#include <algorithm>

#include "IMG.h"

using namespace std;

class Segmentation
{
public:
 Segmentation (shared_ptr<IMG> imgP = nullptr) : Img(imgP) {};   
 shared_ptr<IMG> Img;
 shared_ptr<IMG> OutImg = nullptr;   
 map<int, int> TuningBackground(Area A, bool printParametrs = false);
 //shared_ptr<IMG> Monochrom(shared_ptr<IMG> InImg = nullptr, int Level = 380);
 shared_ptr<IMG> Convolution(shared_ptr<IMG> InImg, int Kernel_width, int Kernel_height, std::vector<int> convKernel);
 shared_ptr<IMG> DetectBorders(shared_ptr<IMG> InImg=nullptr);
 TArea FindColorArea(shared_ptr<IMG> InImg=nullptr);     
 shared_ptr<IMG> Difference(shared_ptr<IMG> Img1, shared_ptr<IMG> Img2, int &count);
 TColorPix fon; 
 TPoint SizeSegment;  

private: 
inline TColorPix MediumColorAreal(shared_ptr<IMG> In, TPoint P0, int width, int height) 
 {  
    double r=0; double g=0; double b=0; 
    for(int y = P0.y; y < P0.y+height; y++) 
     for(int x = P0.x; x < P0.x+width; x++)  
        { r += (double)In->Pix(x, y).Red; 
          g += (double)In->Pix(x, y).Green; 
          b += (double)In->Pix(x, y).Blue; 
        }
     return {round(r/(width*height)), round(g/(width*height)), round(b/(width*height)), 0};
 };

inline TColorPix MediumColorContur(shared_ptr<IMG> In, TPoint P0, int width, int height) 
 {  
    double r=0; double g=0; double b=0; 
    int Ncount = 2*(width + height);
    TPoint P = P0;
    for(int i = 0; i < Ncount; i++)
    {
        TColorPix Pix = In->Pix(P.x, P.y);
        r += (double)Pix.Red; 
        g += (double)Pix.Green;
        b += (double)Pix.Blue;
        P = Img->GetPointKontur(P, {P.x, P.y, P.x+width, P.y+height});
    } 
     return {round(r/Ncount), round(g/Ncount), round(b/Ncount), 0};
 }; 

inline int CompareColors(TColorPix P1, TColorPix P2)  
{
     int dif_r = 100*abs(P1.Red - P2.Red)/0xff;
     int dif_g = 100*abs(P1.Green - P2.Green)/0xff;
     int dif_b = 100*abs(P1.Blue - P2.Blue)/0xff; 
     return dif_r + dif_g + dif_b;   
};
};
