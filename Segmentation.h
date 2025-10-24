#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "IMG.h"

using namespace std;

#pragma pack(push,1)
typedef struct TColorStat
{
    TColorPix  Color;
    int count;
};

class Segmentation
{
public:
 Segmentation (shared_ptr<IMG> imgP = nullptr) : Img(imgP) {};   

 //shared_ptr<IMG> Monochrom(shared_ptr<IMG> InImg = nullptr, int Level = 380);
    static shared_ptr<IMG> Convolution(shared_ptr<IMG> InImg, int Kernel_width, int Kernel_height, std::vector<int> convKernel);
    static shared_ptr<IMG> DetectBorders(shared_ptr<IMG> InImg=nullptr);
    TArea FindColorArea(shared_ptr<IMG> InImg=nullptr);     
    shared_ptr<IMG> Difference(shared_ptr<IMG> Img1, shared_ptr<IMG> Img2, int &count);
    static vector<TColorStat> Histogram(shared_ptr<IMG> InImg, Area A, bool print = false);
    static shared_ptr<IMG> GetClusters(shared_ptr<IMG> InImg, uint32_t deep, vector<TArea>& Clusters, TColorPix findColor={0, 0, 0, 0});
    //vector<TLine> GetVectorLine(shared_ptr<IMG> InImg, uint32_t deep, TColorPix findColor); 
    static shared_ptr<IMG> FastDetectBorders(shared_ptr<IMG> InImg=nullptr, int Level = 0);
    //vector<TArea> FindColorClusters(shared_ptr<IMG> InImg); 
    bool FilterBrightnessClusters(TArea A);
    bool FilterSizeClusters(TArea A);   
    static TColorPix RandomColor();     
    int CompareFrames();
 
    shared_ptr<IMG> Img;
    shared_ptr<IMG> FonImg = nullptr;   
    shared_ptr<IMG> OutImg = nullptr;   
    vector<TColorStat> TuningBackground(Area A, bool printParametrs = false);
    TColorPix fon; 
    TPoint FindSegmentSize;  
    double FindSegmentBrightness;
    TColorPix LevelMonohrom = {90, 90, 90, 90}; 

private: 
inline TColorPix MediumColorAreal(shared_ptr<IMG> In, TPoint P0, int width, int height) 
 {  
    double r=0; double g=0; double b=0; 
    for(int y = P0.y; y < P0.y+height; y++) 
     for(int x = P0.x; x < P0.x+width; x++)  
        { r += (double)In->v(x, y).Red; 
          g += (double)In->v(x, y).Green; 
          b += (double)In->v(x, y).Blue; 
        }
     return {(uint8_t)round(r/(width*height)), (uint8_t)round(g/(width*height)), (uint8_t)round(b/(width*height)), 0};
 };

inline TColorPix MediumColorContur(shared_ptr<IMG> In, TPoint P0, int width, int height) 
 {  
    double r=0; double g=0; double b=0; 
    int Ncount = 2*(width + height);
    TPoint P = P0;
    for(int i = 0; i < Ncount; i++)
    {
        TColorPix Pix = In->v(P.x, P.y);
        r += (double)Pix.Red; 
        g += (double)Pix.Green;
        b += (double)Pix.Blue;
        P = Img->GetPointKontur(P, {P.x, P.y, P.x+width, P.y+height});
    } 
     return { (uint8_t)round(r/Ncount), (uint8_t)round(g/Ncount), (uint8_t)round(b/Ncount), 0};
 }; 

static inline TColorPix CompareColors(TColorPix P1, TColorPix P2)  
{
     int dif_r = abs(P1.Red - P2.Red);
     int dif_g = abs(P1.Green - P2.Green);
     int dif_b = abs(P1.Blue - P2.Blue); 
     TColorPix pixDif; pixDif.IntsToColor(dif_b, dif_g, dif_r);   
     return pixDif;   
};
};
