#pragma once
#include <cstdint>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <memory>
#include "TMatrix.h"
#include "BMP.h"

using namespace std;

#pragma pack(push,1)
typedef struct TPoint
{
	unsigned int   x;
	unsigned int   y;

    bool operator==(const TPoint& other) {return x == other.x && y == other.y ;} 
} Point;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct TColorPix
{
	uint8_t   Blue;
	uint8_t   Green;
    uint8_t   Red;
    uint8_t   A;
    
    bool operator==(const TColorPix& other) {return Blue == other.Blue && Green == other.Green && Red == other.Red && A == other.A ;} 
    bool operator!=(const TColorPix& other) {return Blue != other.Blue || Green != other.Green || Red != other.Red || A != other.A ;} 
    TColorPix operator+=(const TColorPix& other) {return {(uint8_t)(Blue + other.Blue), (uint8_t)(Green + other.Green),(uint8_t)(Red + other.Red), (uint8_t)(A + other.A)} ;}
    int toInt(){ return *reinterpret_cast<int*>(this);  } 
    int sum()  { return Red + Green + Blue; }
    inline TColorPix IntToPix(int v){return *reinterpret_cast<TColorPix*>(&v);}
    inline void IntsToColor(int b, int g, int r)
    {
        if(b<0) Blue = 0; else if(b>0xff) Blue = 0xff; else Blue = static_cast<uint8_t>(b);
        if(g<0) Green = 0; else if(g>0xff) Green = 0xff; else Green = static_cast<uint8_t>(g);
        if(r<0) Red = 0; else if(r>0xff) Red = 0xff; else Red = static_cast<uint8_t>(r);
    }
} ColorPix;
#pragma pack(pop)
const TColorPix Black = { 0, 0, 0, 0 };
const TColorPix White = { 0xff, 0xff, 0xff, 0xff };
const TColorPix Blue = { 0xff, 0, 0, 0 };
const TColorPix Green = { 0, 0xff, 0, 0 };
const TColorPix Red = { 0, 0, 0xff, 0 };
const TColorPix Yellow = { 0, 0xff, 0xff, 0 };
//------------------------------------------------------------------------------------------------

inline int Sing(int a) { return (a > 0) ? (1) : ((a < 0) ? (-1) : (0));  };

class IMG :
	public TMatrix<TColorPix>
{
public:
    IMG(unsigned int w=1, unsigned int h=1)
    {
        width = w; height = h;
        resize(width, height);
        TColorPix fon = { 0, 0, 0, 0 }; 
        InitData(fon);
    }

    IMG(unsigned int w, unsigned int h, TColorPix fon)
	{ 
		width = w; height = h;
        resize(width, height);
        InitData(fon);
	}

    IMG(unsigned int w, unsigned int h, const unsigned char* Data, unsigned int N = 0)  //const unsigned char* Data = nullptr,
    {
        width = w; height = h; Nimg = N;
        resize(width, height);
        if (Data != nullptr)  memcpy(array.data(), Data, sizeof(TColorPix) * size());
    }

    IMG(string NameFile)
    {
        vector<char> buf;
        BITMAPINFOHEADER1 bmh = Load_Bmp24(NameFile, buf, false);
        width = bmh.biWidth; height = bmh.biHeight; Nimg = 0;
        resize(width, height);
        int k = 0;
        for (auto it = array.begin(); it<array.end(); it++)
        {
            it->Blue = buf.at(k);
            it->Green = buf.at(++k);
            it->Red = buf.at(++k);
            if (k < buf.size()) k++;
        }
    }

 //  inline const unsigned char* GetPtr(){return reinterpret_cast<const unsigned char*>(data());}
 //  inline void Set(unsigned int x, unsigned int y, TColorPix value) { v(x, y) = value; } //  array.at(x + y * width);
 //  inline TColorPix Pix(unsigned int x, unsigned int y) { return v(x, y); }
    TColorPix PixLim(unsigned int x, unsigned int y);
   // void ReSize(unsigned int Sx = 1, unsigned int Sy = 1, int mset = 0);

    shared_ptr<IMG> GetCopy();
    shared_ptr<IMG> GetSubImg(Area c); 
    TPoint GetPointKontur(TPoint oldP, Area c);
    void PutMask(TColorPix M);
    void Monochrom(TColorPix Level = { 127, 127, 127, 127 });
    inline void Save_Bmp(string NameFile) {Save_Bmp24b(NameFile, reinterpret_cast<char*>(data()), width, height);}; 
   // inline void PaintInConsol() { PaintInConsol(reinterpret_cast<char*>(data()), width, height, 4 ); };
    double Brightness(int correction);
    double BrightnessArea(TArea A); 
    void Contrast(TColorPix Level, int correction);
    void Line(TPoint P1, TPoint P2, TColorPix Color = {255, 255, 255, 255});
    void Line1(TPoint P1, TPoint P2, TColorPix Color = { 255, 255, 255, 255 });
    unsigned int Nimg = 0; //номер кадра
};

