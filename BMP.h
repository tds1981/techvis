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
//#include <GLFW/glfw3.h>

using namespace std;

#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER
{
	uint16_t   bfType;
	uint32_t   bfSize;
	uint16_t   bfReserved1;
	uint16_t   bfReserved2;
	uint32_t   bfOffBits; //bfOffBits ����������, ��� ���������� ��� ������� ������ ������������ ������ �����
} BITMAPFILEHEADER;

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER // � ����������� �� ������� ��������� �������� ������� ����� ??? biWidth � biHeight  ����� ���� 4 ��� 8 ������
{
	uint32_t   biSize;	//biSize - ��� ������ ����� ���������
	uint32_t   biWidth; // biWidth � biHeight ������ �������������� ������ � ������ �������� � ��������
	uint32_t   biHeight;
	uint16_t   biPlanes;		//biPlanes ������ ���������� ����������. ���� ��� ������ ��������������� � 1.
	uint16_t   biBitCount;		//biBitCount - ���������� ��� �� ���� �������.
	uint32_t  biCompression;	//���������� ��� ������
	uint32_t  biSizeImage;		// biSizeImage ���������� ������ �������� � ������.
	uint32_t   biXPelsPerMeter;
	uint32_t  biYPelsPerMeter;
	uint32_t  biClrUsed;
	uint32_t  biClrImportant;
} BITMAPINFOHEADER;


//BITMAPINFOHEADER Load_Bmp24(string NameFile, IMG* bitAraay,  bool printParametrs);
void Save_Bmp24b(string NameFile, char* buf, unsigned int  Width, unsigned int  Height);
//void Monochrom(IMG *InImg, IMG *OutImg, int Level = 380);
//void TuningMonochrom(IMG *bitArray24);

