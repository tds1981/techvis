#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <memory>
#include <cstddef>
#include <windows.h>
//#include <GLFW/glfw3.h>

using namespace std;

#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER1
{
	uint16_t   bfType;
	uint32_t   bfSize;
	uint16_t   bfReserved1;
	uint16_t   bfReserved2;
	uint32_t   bfOffBits; //bfOffBits показывает, где начинается сам битовый массив относительно начала файла
} BITMAPFILEHEADER1;

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER1 // В зависимости от размера структуры меняются размеры полей ??? biWidth и biHeight  могут быть 4 или 8 байтов
{
	uint32_t   biSize;	//biSize - это размер самой структуры
	uint32_t   biWidth; // biWidth и biHeight задают соответственно ширину и высоту картинки в пикселях
	uint32_t   biHeight;
	uint16_t   biPlanes;		//biPlanes задает количество плоскостей. Пока оно всегда устанавливается в 1.
	uint16_t   biBitCount;		//biBitCount - Количество бит на один пиксель.
	uint32_t  biCompression;	//обозначает тип сжатия
	uint32_t  biSizeImage;		// biSizeImage обозначает размер картинки в байтах.
	uint32_t   biXPelsPerMeter;
	uint32_t  biYPelsPerMeter;
	uint32_t  biClrUsed;
	uint32_t  biClrImportant;
} BITMAPINFOHEADER1;

BITMAPINFOHEADER1 Load_Bmp24(string NameFile, vector<char>& buf, bool printParametrs);
void Save_Bmp24b(string NameFile, char* buf, unsigned int  Width, unsigned int  Height);
void PaintInConsol(char* buf, unsigned int  Width, unsigned int  Height, unsigned int sizePix = 4);
//void Monochrom(IMG *InImg, IMG *OutImg, int Level = 380);
//void TuningMonochrom(IMG *bitArray24);

