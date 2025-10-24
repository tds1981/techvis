#include "IMG.h"
#include <filesystem>

std::filesystem::path dir1 = "data/test_IMG/";

void star()
{
    shared_ptr<IMG> img1 = make_shared<IMG>(500, 500);
    const double pi = 3.14159265;
    for (double i = 0; i < 2 * pi; i += 0.05 * pi)
    {
        // printf(" %d, %d \n", 250 + (int)round(100*cos(i)), 250 + (int)round(100*sin(i)) );
        unsigned int X = (unsigned int)((int)250 + (int)round(200 * sin(i)));
        unsigned int Y = (unsigned int)((int)250 + (int)round(200 * cos(i)));
        //  printf(" %d, %d \n", X, Y);
        img1->Line1({ 250, 250 }, { X, Y }, { 255, (uint8_t)X, (uint8_t)Y, 0 });
    }
    img1->Save_Bmp(dir1.string() + "Test_IMG1.bmp");

}

void rhombus()
{
    TColorPix f = { 255, 255, 255, 0 };
    shared_ptr<IMG> img2 = make_shared<IMG>(500, 500, f);
    for (unsigned int i = 0; i < 180; i += 20)
    {
        img2->Line1({ 250, 50 + i }, { 450 - i, 250 }, { (uint8_t)i, 0, (uint8_t)(190 - i), 0 });
        img2->Line1({ 450 - i, 250 }, { 250, 450 - i }, { (uint8_t)i, 0, (uint8_t)(190 - i), 0 });
        img2->Line1({ 250, 450 - i }, { 50 + i,  250 }, { (uint8_t)i, 0, (uint8_t)(190 - i), 0 });
        img2->Line1({ 50 + i, 250 }, { 250, 50 + i }, { (uint8_t)i, 0, (uint8_t)(190 - i), 0 });
    }
    img2->Save_Bmp(dir1.string() + "rhombus.bmp");
}

void imgMask(string P)
{
    shared_ptr<IMG> foto = make_shared<IMG>(P);
    foto->Save_Bmp(dir1.string() + "f.bmp");
    unsigned char c = 0xf0;
    for (unsigned char i = 1; i <= 4; i += 1)
    {
        shared_ptr<IMG> img_r = foto->GetCopy();
        img_r->PutMask({c, 0, 0, 0});
        img_r->Save_Bmp(dir1.string() + to_string(c) + "_r.bmp");

        shared_ptr<IMG> img_g = foto->GetCopy();
        img_g->PutMask({ 0, c, 0, 0 });
        img_g->Save_Bmp(dir1.string() + to_string(c) + "_g.bmp");

        shared_ptr<IMG> img_b = foto->GetCopy();
        img_b->PutMask({ 0, 0, c, 0 });
        img_b->Save_Bmp(dir1.string() + to_string(c) + "_b.bmp"); 

        c = c << 1;
       // c -= 8;
    } 
  //  shared_ptr<IMG> img_1 = foto->GetCopy();
  //  img_1->PutMask({ 0xf0, 0xf0, 0xf0, 0xf0 });
   // img_1->Save_Bmp(dir1.string() + to_string(0xf0) + ".bmp");
}

void test_IMG()
{
    std::filesystem::remove_all(dir1);
    std::filesystem::create_directory(dir1);

    star();
    rhombus();
   // imgMask("C:/My_VC/TechVis/data/foto1.bmp");
}