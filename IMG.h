#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <memory>
#include "BMP.h"

using namespace std;

#pragma pack(push,1)
typedef struct TArea
{
	uint32_t   x1;
	uint32_t   y1;
	uint32_t   x2;
	uint32_t   y2;

	char name[8];
} Area;

#pragma pack(push,1)
typedef struct TPoint
{
	unsigned int   x;
	unsigned int   y;

    bool operator==(const TPoint& other) {return x == other.x && y == other.y ;} 
} Point;

#pragma pack(push,1)
typedef struct TColorPix
{
	uint8_t   Blue;
	uint8_t   Green;
    uint8_t   Red;
    uint8_t   A;
    
    bool operator==(const TColorPix& other) {return Blue == other.Blue && Green == other.Green && Red == other.Red && A == other.A ;} 
    bool operator!=(const TColorPix& other) {return Blue != other.Blue || Green != other.Green || Red != other.Red || A != other.A ;} 
    TColorPix operator+=(const TColorPix& other) {return {Blue + other.Blue, Green + other.Green, Red + other.Red, A + other.A} ;} 
    int toInt(){return *reinterpret_cast<int*>(this);} 
    inline TColorPix IntToPix(int v){return *reinterpret_cast<TColorPix*>(&v);}
    inline void IntsToColor(int b, int g, int r)
    {
        if(b<0) Blue = 0; else if(b>0xff) Blue = 0xff; else Blue = static_cast<uint8_t>(b);
        if(g<0) Green = 0; else if(g>0xff) Green = 0xff; else Green = static_cast<uint8_t>(g);
        if(r<0) Red = 0; else if(r>0xff) Red = 0xff; else Red = static_cast<uint8_t>(r);
    }
} ColorPix;

class IMG :
	public std::vector<TColorPix>
{
public:
    IMG(unsigned int Sx = 1, unsigned int Sy = 1, const unsigned char* Data = nullptr, unsigned int N = 0, int mset = 0)
	{ 
		width = Sx; height = Sy; Nimg = N;
		resize(Sx * Sy);
        if (Data == nullptr) memset(data(), mset, size());
        else memcpy(data(), Data, sizeof(TColorPix) * size());
	}
    inline const unsigned char* GetPtr(){return reinterpret_cast<const unsigned char*>(data());}
    inline void Set(unsigned int x, unsigned int y, TColorPix value);
    inline TColorPix Pix(unsigned int x, unsigned int y);
    TColorPix PixLim(unsigned int x, unsigned int y);
    inline int PixToInt(unsigned int x, unsigned int y)
	{
		TColorPix v = Pix(x, y);
		return *reinterpret_cast<int*>(&v);
	}
    shared_ptr<IMG> GetCopy();
    void ReSize(unsigned int Sx = 1, unsigned int Sy = 1, int mset = 0);
    shared_ptr<IMG> GetSubImg(Area c); 
    TPoint GetPointKontur(TPoint oldP, Area c);
    void PutMask(TColorPix M);
    void Monochrom(TColorPix Level);
    inline void Save_Bmp(string NameFile) {Save_Bmp24b(NameFile, reinterpret_cast<char*>(data()), width, height);}; 
    double Brightness(int correction);
    double BrightnessArea(TArea A);
    void Contrast(TColorPix Level, int correction);
    unsigned int width;
	unsigned int height;
    unsigned int Nimg = 0; //номер кадра
};

