#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "IMG.h"

using namespace std;

typedef struct TPointInVector
{
	unsigned int   x;
	unsigned int   y;
	int   dx;
	int   dy;
} PointInVector;

class VectorPoints : public std::vector<TPointInVector>
{
public:
    VectorPoints(shared_ptr<IMG> Img, TColorPix baseColor);
    void Get1(shared_ptr<IMG> Img, TColorPix baseColor); 
    void GetDirectionsForPoints(shared_ptr<IMG> Img, TColorPix baseColor); 
    void MapStrToLines();
    void DeleteShortLines(unsigned int filterLength);    
    void PrintLog(char typeSort = 'x');
    shared_ptr<IMG> GetImg(bool lineOrpoints = true);
    inline void SortX() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.x < b.x;}); sortY=false; };
    inline void SortY() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.y < b.y;}); sortY=true;  };
    inline void SortLength() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.dx*a.dx + a.dy*a.dy < b.dx*b.dx + b.dy*b.dy;});  };
private:
   map <unsigned int, vector<TPointInVector>> strPoints; 
   unsigned int width;
   unsigned int height; 
   bool sortY=true; 
};

