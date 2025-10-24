#include "BMP.h"

BITMAPINFOHEADER1 Load_Bmp24(string NameFile, vector<char> &buf,  bool printParametrs)
{
    BITMAPFILEHEADER1 bfh;
    BITMAPINFOHEADER1 bih;
    memset(&bih, 0, sizeof(bih));
 
    ifstream f;
    f.open(NameFile, ios::binary);
    if (!f.is_open()) { printf("ERROR: File %s not open \n", NameFile.c_str()); return bih; }
    else printf("Open File: %s \n", NameFile.c_str());

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
    buf.resize(bfh.bfSize - bfh.bfOffBits); //Width * Height
    f.read(buf.data(), buf.size());
    f.close();
    
    if (printParametrs) printf("byte: %d \n", buf.size());


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
    }
 
    //memcpy(bitAraay->data(), buf.data(), buf.size());
    return bih; 
}


void Save_Bmp24b(string NameFile, char* buf, unsigned int  Width, unsigned int  Height) // c - 24-битный цвет, 
{
    BITMAPFILEHEADER1 bfh;
    BITMAPINFOHEADER1 bih;
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

   // printf("lstr %d, nulByte %d \n", lstr, nulByte);

    ofstream out;
    out.open(NameFile, ios::binary);
    out.write(reinterpret_cast<char*>(&bfh), sizeof(bfh));
    out.write(reinterpret_cast<char*>(&bih), sizeof(bih));

    char *nulstr = new char[nulByte]; memset(nulstr, 0, sizeof(nulstr));
   // const char *buf = reinterpret_cast<char*>(bitAraay->data());
    for (unsigned int y = 0; y < Height; y++)
    {
        //out.write(&buf->data()[i * lstr], lstr);
        for (unsigned int x = 0; x < Width; x++) out.write(buf + (x + Width*y)*4, 3);
        if (nulByte) out.write(nulstr, nulByte);
    }
    out.close();
    printf("Save file: %s \n", NameFile.c_str());
    //delete[] nulstr;
}

void PaintInConsol(char* buf, unsigned int  Width, unsigned int  Height, unsigned int sizePix)
{
    Sleep(1000);             // Задержка 100 мс, без этого, иногда, перерисовка окна затирает рисунок

    HWND hwnd = GetConsoleWindow(); // Находим дескриптор (handle) консольного окна

    RECT cons_rc;
    GetClientRect(hwnd, &cons_rc);

    HDC hdc = GetDC(hwnd);  // Находим контекст устройства DC - device context

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
           // ColorPix24 P = bitArray24->Array.at(x + (bih.biHeight - 1 - y) * bih.biWidth);
            int i = sizePix*(x + y * Width);
            COLORREF color;
            if(sizePix == 4) color = RGB(buf[i], buf[i+1], buf[i+2]);
            if(sizePix == 1) color = RGB(buf[i], buf[i], buf[i]);
            HPEN hPen = CreatePen(PS_SOLID, 1, color);  // Создаем перо
            SelectObject(hdc, hPen);
            SetPixel(hdc, x + cons_rc.right / 2, y, color);
            DeleteObject(hPen);
        }
    }
    ReleaseDC(hwnd, hdc);                           // Освобождаем ресурсы

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


