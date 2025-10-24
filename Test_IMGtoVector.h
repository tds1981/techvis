#include "IMGtoVector.h"
#include "Segmentation.h"
#include <direct.h>

const double pi = 3.14159265;

void OneLine()
{
    printf("\n----------- TEST 1: One line -------------\n");

   // const char* dir_name = "data/TestVector"; // Имя папки
   // if (_mkdir(dir_name) ==0)
  //  {
        TColorPix f = Yellow;
        for (double i = 0; i < 2 * pi; i += 0.1 * pi)
        {
            shared_ptr<IMG> img1 = make_shared<IMG>(500, 500, f);
            unsigned int X = (unsigned int)((int)250 + (int)round(200 * sin(i)));
            unsigned int Y = (unsigned int)((int)250 + (int)round(200 * cos(i)));
            TColorPix ColorPix = { 255, (uint8_t)X, (uint8_t)Y, 0 };
            img1->Line1({ 250, 250 }, { X, Y }, ColorPix);
            img1->Save_Bmp("data/TestVector/" + to_string(X) + "_" + to_string(Y) + ".bmp");

            VectorPoints vP1(img1, ColorPix);
            vP1.GetVector();
            vP1.ConnectNeighborsLines();
            printf("L: x1 = %d, y1 = %d, x2 = %d, y2 = %d \n", 250, 250, X, Y);
            vP1.Print();
            //printf("TraceLine ctn = %d \n", vP1.ctn);
            vP1.GetImg(true)->Save_Bmp("data/TestVector/" + to_string(X) + "_" + to_string(Y) + "_vector.bmp");
          //  vP1.PrintLog("data/TestVector/" + to_string(X) + "_" + to_string(Y) + "_vector.txt"); 
        }
  //  }
}

void letterA()
{
   printf("\n----------- TEST 3: letterA ---------------\n");
   shared_ptr<IMG> img3 = make_shared<IMG>("data/A_367.bmp"); 
   shared_ptr<IMG> img4 = Segmentation::FastDetectBorders(img3, 100);
   img4->Save_Bmp("data/A.bmp");
   VectorPoints vP3(img4);
   vP3.presIx(3);
   //vP3.presIy(2);
   vP3.GetVector();
   vP3.ConnectNeighborsLines();
   vP3.DeleteShortLines(5);
   vP3.Print();
   vP3.GetImg(true)->Save_Bmp("data/vector_leterA.bmp");
  // vP2.PrintLog("data/vector_kv.txt");
   vP3.PixtoImg()->Save_Bmp("data/img_I.bmp");
}

void letters()
{
    printf("\n----------- TEST 4: letters ---------------\n");
    shared_ptr<IMG> file = make_shared<IMG>("data/leters2.bmp");

    vector<TArea> Clusters;
    Segmentation::GetClusters(file, 1, Clusters);
    for (auto it = Clusters.begin(); it < Clusters.end(); it++)
    {
        it->resize(-5, -5, 5, 5);
        shared_ptr<IMG> img1 = file->GetSubImg(*it);
       // img1->Monochrom();
        img1->Save_Bmp("data/Leters/" + to_string(it->x1) + "_s.bmp");
       // shared_ptr<IMG> imgB = Segmentation::FastDetectBorders(img1, 382);
        shared_ptr<IMG> imgB = Segmentation::FastDetectBorders(img1);
        imgB->Save_Bmp("data/Leters/" + to_string(it->x1) + "_B1.bmp");
        imgB->Monochrom();
        imgB->Save_Bmp("data/Leters/" + to_string(it->x1) + "_B2.bmp");
        VectorPoints vP(imgB);
       // vP.presIx(3);  vP.presIy(2);
       // vP.PixtoImg()->Save_Bmp("data/Leters/" + to_string(it->x1) + "_I.bmp");
      //  vP.GetVector();
      //  vP.ConnectNeighborsLines();
      //  vP.DeleteShortLines(3);
     //   printf("--------------------- \n");
      //  vP.Print();
       // vP.PrintLog("data/Leters/" + to_string(it->x1) + ".txt");
       // vP.GetImg(true)->Save_Bmp("data/Leters/" + to_string(it->x1) + "_vector.bmp");
        
    }
}

void square()
{
    printf("\n----------- TEST 3: square ---------------\n");
    shared_ptr<IMG> img2 = make_shared<IMG>(500, 500);
    //    TColorPix ColorPix = { 255, (uint8_t)X, (uint8_t)Y, 0 };
    img2->Line1({ 50,  100 }, { 350, 100 });
    img2->Line1({ 350, 100 }, { 350, 400 });
    img2->Line1({ 350, 400 }, { 50, 400 });
    img2->Line1({ 50, 400 }, { 50, 100 });
    img2->Save_Bmp("data/rhombus/square.bmp");
    VectorPoints vP2(img2);
    vP2.GetVector();
    vP2.Print();
    vP2.GetImg(true)->Save_Bmp("data/rhombus/square_vector.bmp");
    vP2.PrintLog("data/rhombus/square_vector.txt");
    //printf("TraceLine ctn = %d \n", vP2.ctn);
}

void RhombusToVector()
{
    printf("\n----------- TEST 2: rhombus ---------------\n");
        shared_ptr<IMG> img = make_shared<IMG>("data/rhombus.bmp");
        img->Monochrom({ 200, 0, 0, 0 });
        img->Save_Bmp("data/rhombus/m.bmp");

        VectorPoints vP(img, { 0, 0, 0, 0 });
        vP.PixtoImg()->Save_Bmp("data/rhombus/I.bmp");
        //  vP.presIx(3); vP.presIy(2);
        vP.GetVector();
        vP.PixtoImg()->Save_Bmp("data/rhombus/I1.bmp");
        //vP.ConnectNeighborsLines();
        vP.DeleteShortLines(2);
        printf("--------------------- \n");
        vP.Print();
        vP.PrintLog("data/rhombus/rhombus.txt");
        vP.GetImg()->Save_Bmp("data/rhombus/vector.bmp");

        square();
}

void test_IMGtoVector()
{
     int Ntest;
   
   OneLine();
   cin.get();
  // RhombusToVector();
  // cin.get();
  // letters();
  // cin.get();
}