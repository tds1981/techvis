#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <utility>

#include "IMG.h"

using namespace std;

#pragma pack(push,1)
typedef struct TPointInVector
{
	unsigned int   x;
	unsigned int   y;
	int   dx;
	int   dy;
    bool full = false;  // флаг если true то отрезок закончился
    unsigned int hash() { return  x * x + y * y; }; //квадрат растояния от начала координат
    unsigned int x2() { return  (unsigned int)( (int)x + dx ); }
    unsigned int y2() { return  (unsigned int)( (int)y + dy );  }
    double k() { if (dx != 0) return (double)dy / (double)dx; else 0xffffffff; }
    double l() { return sqrt(dx*dx + dy*dy); } // длина вектора
} PointInVector;

#pragma pack(push,1)
typedef struct TJump
{
    uint32_t   x1;
    uint32_t   y1;
    uint32_t   x2;
    uint32_t   y2;
} Jump;

#pragma pack(push,1)
struct StatusPoint 
{ 
    bool PointInBuf = false; 
    bool PointChecked = false; 
};

class VectorPoints : public std::vector<TPointInVector>
{
public:
    VectorPoints(shared_ptr<IMG> Img, TColorPix baseColor = { 255, 255, 255, 255 }, char compresLine = 0); // baseColor - цвет пикселей включаемых в вектор
    void GetVector();
    void MapStrToLines();
    void ConnectNeighborsLines();
    void presIx(int count);
    void presIy(int count);
    void DeleteShortLines(unsigned int filterLength);    
    void PrintLog(string NameFile);
    void Print(bool typeprint = true, string str = "");
    shared_ptr<IMG> GetImg(bool lineOrpoints = true);
    shared_ptr<IMG> PixtoImg();
    inline void SortX() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.x < b.x;}); sortY=false; };
    inline void SortY() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.y < b.y;}); sortY=true;  };
    inline void SortLength() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.l() < b.l(); }); };  //a.dx*a.dx + a.dy*a.dy < b.dx*b.dx + b.dy*b.dy;
    inline void Sort() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.y < b.y && a.x < b.x; }); sortY = true; };
    inline void SortZero() { std::sort(begin(), end(), [](TPointInVector a, TPointInVector b) {return a.x * a.x + a.y * a.y < b.x * b.x + b.y * b.y; }); };
    int ctn = 0;
private:
   bool checkLine(TPointInVector a, TPointInVector b);
   void TraceLine(TPoint P0);
   inline auto itP(vector<TPointInVector>::iterator a, unsigned int x, unsigned int y) //
   {
       //auto a = begin();
       while (a < end()) { if (x == a->x2() && y == a->y2()) return a;  a++; }
       return end();
   }
   vector<TPointInVector> LocalControl(TPoint P0);
   int PrintI(bool printMatr = false);
   vector<char> I;
   char Pix(unsigned int x, unsigned int y) { if (x >= 0 && x < width && y >= 0 && y < height) return I.at(x + y * width); else - 1; }
   void SetPix(unsigned int x, unsigned int y, char v) { if (x >= 0 && x < width && y >= 0 && y < height) I.at(x + y * width) = v; }
  // map <unsigned int, vector<TPointInVector>> strPoints;
   unsigned int width;
   unsigned int height; 
   bool sortY=true; 
};

