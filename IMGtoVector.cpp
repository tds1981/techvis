#include "IMGtoVector.h"

VectorPoints::VectorPoints(shared_ptr<IMG> Img, TColorPix baseColor, char compresLine) 
{
    
     width = Img->width;  height = Img->height; 
     I.resize(width * height);
     memset(I.data(), 0, sizeof(char) * I.size());
     for (unsigned int k = 0; k < width * height; k++) if (Img->array.at(k) == baseColor) I.at(k) = 1;
     //PrintI();
     if (compresLine > 1) { presIx(compresLine); presIy(compresLine); }
     //PrintI();
    // printf("Size Vector: %d \n", size());
    
     SortZero();  
}

void VectorPoints::GetVector()
{
    for (unsigned int y = 0; y < height; y++)
        for (unsigned int x = 0; x < width; x++)
        {
            try
            {
                if (Pix(x, y) == 1)
                {
                    TraceLine({x, y}); // получаем направления для текущей точки (x0, y0)
                    unsigned int all=0;
                    while (all < size()) 
                    {
                        printf("GetVector begin \n");
                        for(auto a = begin(); a<end(); a++)
                            if (!a->full) TraceLine({ a->x2(), a->y2() }); else  all++;
                        printf("GetVector end \n");

                    } 
                }
            }
            catch (...)
            {
                printf("Error in GetVector \n"); cin.get();
            }
        }
    //PrintI();
}

void VectorPoints::presIx(int count)
{  
   // printf(" presIx : \n ");
    for (unsigned int y = 0; y < height; y++)
    {
        int n = 0;
        for (unsigned int x = 0; x < width - count; x++)
        {
          // if(n) printf(" n= %d ", n);
            n += I.at(x + y * width);
            if (I.at(x + 1 + y * width) == 0)
            {
                if (n > 1 && n <= count) 
                    for (int i = 0; i < n - 1; i++) { I.at(x - i + y * width) = 0;  }
                
                n = 0;
            }
        }
    }
}

void VectorPoints::presIy(int count)
{
    // printf(" presIx : \n ");
    for (unsigned int x = 0; x < width; x++)
    {
        int n = 0;
        for (unsigned int y = 0; y < height - count; y++)
        {
            // if(n) printf(" n= %d ", n);
            n += I.at(x + y * width);
            if (I.at(x + (1 + y) * width) == 0)
            {
                if (n > 1 && n <= count)
                    for (int i = 0; i < n - 1; i++) { I.at(x + (y-1) * width) = 0; }

                n = 0;
            }
        }
    }
}


vector<TPointInVector> VectorPoints::LocalControl(TPoint P0)
{
    vector<TPointInVector> R;
    try
    {
        
        SetPix(P0.x, P0.y, 2);
        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
             // if ( P0.x + dx >= 0 && P0.x + dx < width && P0.y + dy >=0 && P0.y + dy < height )
                if (Pix(P0.x + dx, P0.y + dy) == 1)
                {
                    SetPix(P0.x + dx, P0.y + dy, 2);
                    R.push_back({ P0.x, P0.y, dx, dy });
                }
    }
    catch (...)
    {
        printf("Error in LocalControl \n"); cin.get();
    }
    printf("LocalControl: X= %d Y= %d, sizev= %d \n", P0.x, P0.y, R.size());
    return R;
}

void VectorPoints::TraceLine(TPoint P0)
{
         ctn++;
         printf("\033[1;32m start TraceLine %d \033[1;0m  x=%d, y=%d \n", ctn, P0.x, P0.y);
        // Print(false, "TraceLine "+to_string(ctn));
        vector<TPointInVector> loc = LocalControl({ P0.x, P0.y }); // получаем направления для текущей точки (x0, y0)
        if (loc.size() == 0) 
        {
            try
            {
                SortZero();
                auto a = begin();
                while (a < end()) { if (P0.x == a->x2() && P0.y == a->y2()) a->full = true;  a++; }
                return;
            }
            catch (...)
            {  printf("Error in TraceLine  loc.size() \n"); cin.get(); }
        } 
        else  
        {
          try
          {
             SortZero();
             auto a = begin();     // поиск вектора с конечной точкой равной текущей точке (x0, y0) //itP(begin(), P0.x, P0.y );  
             while (a < end())    
             {
                 if ( P0.x == a->x2() && P0.y == a->y2() && !a->full )    // точка (x0, y0) является продолжением отрезка a
                 {
                     auto b = loc.begin();      // b = (x0, y0)
                     while (b < loc.end())      // наращиваем существующий вектор одним из направлений
                         if (
                              (Sing(a->dx) == b->dx && Sing(a->dy) == b->dy) ||
                              (b->dx == 0 && Sing(a->dy) == b->dy) ||
                              (Sing(a->dx) == b->dx && b->dy == 0)
                            )
                         {
                             a->dx += b->dx;
                             a->dy += b->dy;
                             //  TraceLine({ b->x + b->dx, b->y + b->dy });
                             b = loc.erase(b); 
                             break;
                         }
                         else b++;
                 }
                 a++;
             }
          }
          catch (...)
          {
              printf("Error in TraceLine \n"); cin.get();
          }

            //if (rekurs) TraceLine( { a->x2(), a->y2() } );
            if (loc.size() != 0) insert(end(), loc.begin(), loc.end()); //направления которые не присоединились к существующему вектору добавляются как новые вектора 
           // for (auto b = loc.begin(); b < loc.end(); b++) TraceLine( { b->x2(), b->y2() }  );
            printf("\033[1;32m END TraceLine %d \033[1;0m  x=%d, y=%d \n", ctn, P0.x, P0.y);
            
        }
        //Print(false, "TraceLine " + to_string(ctn));
}

void VectorPoints::ConnectNeighborsLines()
{
     SortZero();
     if (size() >= 2)
         for (auto a = rbegin(); a < rend();)
             if ((a + 1) != rend() && abs((int)a->x - (int)(a + 1)->x2()) <= 1 && abs((int)a->y - (int)(a + 1)->y2()) <= 1)
             {
                // printf("(%d, %d, %d, %d)  ", a->x, a->y, a->dx, a->dy);
                 (a + 1)->dx += a->dx;
                 (a + 1)->dy += a->dy;
                 //a = erase(a);
                 a->x = 0; a->y = 0; a->dx = 0; a->dy = 0;
             }
             else a++;
     DeleteShortLines(0);
    // printf(" \n ");
}

bool VectorPoints::checkLine(TPointInVector a, TPointInVector b)
{
        auto Sing{[](int a) { return (a >= 0) ? (true) : (false); }};

        auto Sing1{[](int a) { return (a > 0) ? (1) : ( (a < 0) ? (-1) : (0) ); } };

        if ( Sing(a.dy) == Sing(b.dy) && Sing(a.dx) == Sing(b.dx) ) return true;
        if ( Sing(a.dy) == Sing(b.dy) && (a.dx == 0 || b.dx == 0) ) return true;
        if ( (a.dy == 0 || b.dy == 0) && Sing(a.dx) == Sing(b.dx) ) return true;
        return false;

       //double k1 = (a.dx != 0) ? ((double)a.dy / (double)a.dx) : ((double)a.dy / (double)a.dx);
};


shared_ptr<IMG> VectorPoints::GetImg(bool lineOrpoints)
{
    shared_ptr<IMG> R = make_shared<IMG>(width, height); 
    for(auto it = begin(); it < end(); it++)
    if (lineOrpoints)
         R->Line1( { it->x, it->y }, { it->x + it->dx, it->y + it->dy }, {0xff, 0xff, 0xff, 0xff} );
    else R->v(it->x, it->y) = White;
    return R;
} 

void VectorPoints::DeleteShortLines(unsigned int filterLength)
{
    SortLength();
    auto it = begin(); 
    while (pow(it->dx, 2)+pow(it->dy, 2) <= filterLength * filterLength) it++; 
    erase(begin(), it);
}

void VectorPoints::PrintLog(string NameFile)
{
  //  if (typeSort == 'x') std::sort(begin(*this), end(*this))

    std::fstream outputFile;
    outputFile.open(NameFile, ios::app);
    if(outputFile.is_open())
    {
        for (auto it = begin(); it < end(); it++) 
        outputFile << " " << to_string(sqrt(it->hash()))
                          << " x1= "<< to_string(it->x) << " y1= "<< to_string(it->y) 
                          << " x2= "<< to_string(it->x + it->dx) << " y2= "<< to_string(it->y + it->dy) 
                          << " dx= " << to_string(it->dx) << " dy= " << to_string(it->dy) 
                          << " k= " <<  to_string(it->k()) << " l= " <<  to_string(it->l())
                          << std::endl;
    }
    outputFile.close(); 
}

void VectorPoints::Print(bool typeprint, string str)
{
    int n = 1;
    if (typeprint)
    {
        for (auto it = begin(); it < end(); it++)
            printf(" x1 = %d, y1 = %d, x2 = %d, y2 = %d,  l = %f \n", it->x, it->y, it->x + it->dx, it->y + it->dy, it->l());
        printf("  -----------------------  \n");
    }
    else {
            printf("\n \033[1;31m %s : \033[1;0m ", str.c_str());
            for (auto it = begin(); it < end(); it++)
            {
                printf("(%d, %d, %d, %d)  ", it->x, it->y, it->dx, it->dy);
                if(n%7 ==0) printf("  \n ");
                n++;
            }   
        }
}

int VectorPoints::PrintI(bool printMatr)
{
    int stat[3] = {0};
    for (unsigned int y = 0; y < height; y++)
    {
        if (printMatr) printf(" \n ");
        for (unsigned int x = 0; x < width; x++)
        {
            if (Pix(x, y) == 0) stat[0]++;
            if (Pix(x, y) == 1) stat[1]++;
            if (Pix(x, y) == 2) stat[2]++;
            if (printMatr) printf("%d", Pix(x, y));
        }
    }
    printf(" \n Count_0 = %d, Count_1 = %d, Count_2 = %d \n", stat[0], stat[1], stat[2]);
    return stat[1] + stat[2];
}

shared_ptr<IMG> VectorPoints::PixtoImg()
{
    shared_ptr<IMG> img1 = make_shared<IMG>(width, height);
    for (unsigned int k = 0; k < width * height; k++)
        if (I.at(k) == 1) img1->array.at(k).Green = 255; else if (I.at(k) == 2) img1->array.at(k).Blue = 255;
    return img1;
}