#include "BMP.h"

/*BITMAPINFOHEADER Load_Bmp24(string NameFile, IMG* bitAraay,  bool printParametrs)
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    memset(&bih, 0, sizeof(bih));
 
    ifstream f;
    f.open(NameFile, ios::binary);
    if (!f.is_open()) { printf("ERROR: File not open \n"); return bih; }
   // else printf("Open File: %s \n", NameFile.c_str());

    f.read(reinterpret_cast<char*> (&bfh), sizeof(bfh));
    
    if (printParametrs)
    {
        printf("bfType: %c %c \n", bfh.bfType, bfh.bfType >> 8);
        printf("bfSize: %lu \n", bfh.bfSize);
        printf("bfOffBits: %lu \n", bfh.bfOffBits);
    }

    f.read(reinterpret_cast<char*> (&bih), sizeof(bih));
    if (printParametrs)
    {
        printf("biSize: %ld \n", bih.biSize);
        printf("biWidth: %ld \n", bih.biWidth);
        printf("biHeight: %ld \n", bih.biHeight);
        printf("biPlanes: %ld \n", bih.biPlanes);
        printf("biBitCount: %d \n", bih.biBitCount);
        printf("biCompression: %d \n", bih.biCompression);
        printf("biSizeImage: %d \n", bih.biSizeImage);
        printf("biXPelsPerMeter: %d \n", bih.biXPelsPerMeter);
        printf("biYPelsPerMeter: %d \n", bih.biYPelsPerMeter);
        printf("biClrUsed: %d \n", bih.biClrUsed);
        printf("biClrImportant: %d \n", bih.biClrImportant);
        printf("bfh: %d,  bih: %d \n", sizeof(bfh), sizeof(bih));
    }

   // vector<char>* buf = reinterpret_cast<vector<char>*>(bitAraay);
    vector<char> buf;
    buf.resize(bfh.bfSize - bfh.bfOffBits); //Width * Height
    f.read(buf.data(), buf.size());
    f.close();
    
    //if (printParametrs) printf("bitAraay.size(): pixel: %d, byte: %d \n", bitAraay.size(), buf.size());*/

/*
    unsigned int lenStr = bih.biWidth * (bih.biBitCount / 8);
    if (lenStr % 4)
    {
        unsigned int nulByte = 4 * (1 + lenStr / 4) - lenStr;
        auto iter = buf.cbegin() + lenStr;
        while (iter < buf.end() - 1)
        {
            iter = buf.erase(iter, iter + nulByte);
            if (iter < buf.end() - 1) iter += lenStr;
        }
    }*/
 

   // IMG *bitAraay = new IMG();
   // bitAraay->width = bih.biWidth;
   // bitAraay->height = bih.biHeight;
   // bitAraay->resize(bitAraay->width * bitAraay->height);

   // auto b = buf.end();
   // for(auto it = bitAraay->begin(); it < bitAraay->end(); it++)
   // {
      //  it->Blue = *(--b);
      //  it->Green = *(--b);
      //  it->Red = *(--b);
   //     it->A = 0xff; 
   // }
    //memcpy(bitAraay->data(), buf.data(), buf.size());
  //  return bih; 
//}


void Save_Bmp24b(string NameFile, char* buf, unsigned int  Width, unsigned int  Height) // c - 24-битный цвет, 
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    //unsigned int  Width = bitAraay->width;
    //unsigned int  Height = bitAraay->height;

    memset(&bfh, 0, sizeof(bfh));
    memset(&bih, 0, sizeof(bih));
    bfh.bfType = 0x4D42;                           // Обозначим, что это bmp 'BM'
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih);    // начало изображения относительно начала файла
    bih.biSize = sizeof(bih);                      // Так положено
    bih.biClrUsed = 0;
    // bih.biCompression = BI_RGB;                    // Без сжатия
    bih.biHeight = Height;
    bih.biWidth = Width;
    bih.biPlanes = 1;     // Должно быть 1
    bih.biBitCount = 24; //BitCount;

    //char* buf = reinterpret_cast<char*>(bitAraay->data());
    unsigned int lstr = Width * 3;
    unsigned int nulByte = (lstr % 4) ? 4 - lstr % 4 : 0;  //nulByte = 4 * (1 + lenStr / 4) - lenStr;
   
    bfh.bfSize = bfh.bfOffBits + Height * (lstr + nulByte);
    bih.biSizeImage = Height * (lstr + nulByte);

    printf("Save file: %s \n", NameFile.c_str());
   // printf("lstr %d, nulByte %d \n", lstr, nulByte);

    ofstream out;
    out.open(NameFile, ios::binary);
    out.write(reinterpret_cast<char*>(&bfh), sizeof(bfh));
    out.write(reinterpret_cast<char*>(&bih), sizeof(bih));

    char *nulstr = new char[nulByte]; memset(nulstr, 0, sizeof(nulstr));
   // const char *buf = reinterpret_cast<char*>(bitAraay->data());
    for (unsigned int y = Height; y > 0; y--)
    {
        //out.write(&buf->data()[i * lstr], lstr);
        for (unsigned int x = 0; x < Width; x++) out.write(&buf[x*4 + Width*y*4], 3);
        if (nulByte) out.write(nulstr, nulByte);
    }

    out.close();
}
/*
void Monochrom(IMG *InImg, IMG *OutImg, int Level)
{
   auto out = OutImg->begin();  
   for (auto it = InImg->begin(); it < InImg->end(); it++)
       if (it->Blue + it->Green + it->Red > Level) { out->Blue = 255; out->Green = 255; out->Red = 255; out++;}
       else { out->Blue = 0; out->Green = 0; out->Red = 0; out++;}
}

void TuningMonochrom(IMG *bitArray24)
{
    for(int L = 50; L<1000; L+=25)  
    {  
        IMG *OutImg = new IMG(bitArray24->width, bitArray24->height, 0);
        Monochrom(bitArray24, OutImg, L);
        int result = system("mkdir -p Tuning");
        std::string NameFile = "Tuning/Result" + std::to_string(L) + ".bmp";
        Save_Bmp24b(NameFile, OutImg);
        delete OutImg; 
    }
}*/


