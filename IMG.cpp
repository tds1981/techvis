#include "IMG.h"


TColorPix IMG::PixLim(unsigned int x, unsigned int y)
{
    unsigned int x_l = x;
    unsigned int y_l = y;
    if(x_l >= width)  x_l = width-1;
    if(y_l >= height) y_l = height-1;
    return v(x_l, y_l);
}

shared_ptr<IMG> IMG::GetSubImg(Area c)  
{
    shared_ptr<IMG> buf = make_shared<IMG>(c.x2 - c.x1, c.y2 - c.y1);
	//TMatrix src = new TMatrix(c.x2 - c.x1, c.y2 - c.y1);
	//SubIMG->ReSize(c.x2 - c.x1, c.y2 - c.y1);

	for (uint32_t y = c.y1; y < c.y2; y++)
		for (uint32_t x = c.x1; x < c.x2; x++)
			buf->v(x - c.x1, y - c.y1) = v(x, y);
	
	return buf;
}
TPoint IMG::GetPointKontur(TPoint oldP, Area c)
{
    TPoint R = oldP;
    if (oldP.y == c.y1 && oldP.x < c.x2){R.x++; return R;}
    else if (oldP.x == c.x2 && oldP.y < c.y2 )  {R.y++; return R;} 
     else if (oldP.y == c.y2 && oldP.x > c.x1 )  {R.x--; return R;} 
        else if (oldP.x == c.x1 && oldP.y > c.y1 )  {R.y--; return R;} 
    if(R.x >= width)  R.x = width-1;
    if(R.y >= height) R.y = height-1;
    return R;
}

void IMG::PutMask(TColorPix M)
{
    for(auto it = array.begin(); it < array.end(); it++)
    {
        it->Red &= M.Red; it->Green &= M.Green;  it->Blue &= M.Blue;   it->A &= M.A;
    }
}

void IMG::Monochrom(TColorPix Level)
{  
   for (auto it = array.begin(); it < array.end(); it++)
       if (it->Blue + it->Green + it->Red + it->A > Level.Blue + Level.Green + Level.Red + Level.A) { *it = {0xff, 0xff, 0xff, 0xff};}
       else { *it = {0, 0, 0, 0};}
}

shared_ptr<IMG> IMG::GetCopy()
{
    shared_ptr<IMG> buf = make_shared<IMG>(width, height);
    std::copy(array.begin(), array.end(), buf->array.begin());
    //memcpy(buf->data(), data(), sizeof(TColorPix)*buf->size());
    return buf;
}

double IMG::Brightness(int correction)
{
  double bright = 0;
  for (auto it = array.begin(); it < array.end(); it++)  { bright +=  ((double)it->Red*0.299 + (double)it->Green*0.587 + (double)it->Blue*0.114)/ (double)width*(double)height;  } 
  int bright_int = (int)round(bright); 
     
  double k = 1.0 + correction/100; 
  unsigned char d_mean[256]; 
  for(unsigned char i=0; i<255; i++)  
  {  
    int delta = (int)i - bright_int;
    int t = (int)(bright_int + k*delta);
    if(t < 0) d_mean[i] = 0;
    else if (t >= 255) d_mean[i] = 255;
    else d_mean[i] = (unsigned char) t;
  }   
   for (auto it = array.begin(); it < array.end(); it++) {*it = {d_mean[it->Blue], d_mean[it->Green], d_mean[it->Red], 0xff}; } 
   return  bright;
}

double IMG::BrightnessArea(TArea A)
{
  double bright = 0;
  double S = (A.x2 - A.x1)*(A.y2 - A.y1);  
  for (unsigned int y = A.y1; y <= A.y2; y++)
     for (unsigned int x = A.x1; x <= A.x2; x++)
     {
        bright +=  (double)v(x, y).Red*0.299 + (double)v(x, y).Green*0.587 + (double)v(x, y).Blue*0.114;
     } 
  bright /= S;
  return bright; 
}

void IMG::Contrast(TColorPix Level, int correction)
{
     for (auto it = array.begin(); it < array.end(); it++)
     {   
       int b; int g; int r; 

       if (it->Blue >= Level.Blue) { b = (int)it->Blue + correction;} else { b = (int)it->Blue - correction;}
       if (it->Green >= Level.Green) { g = (int)it->Green + correction;} else { g = (int)it->Green - correction;} 
       if (it->Red >= Level.Red) { r = (int)it->Red + correction;} else { r = (int)it->Red - correction;} 
       it->IntsToColor(b, g, r); 
     }
}
void IMG::Line(TPoint P1, TPoint P2, TColorPix Color)
{
    double x = (double)P1.x;
    double y = (double)P1.y;
    double dx = (double)P2.x - (double)P1.x;
    double dy = (double)P2.y - (double)P1.y;
    double CountT = round(sqrt(pow(dx, 2) + pow(dy, 2))); //ceil 
    for (double t = 0; t < CountT; t++)
    {
       // double x = ((CountT - t) * P1.x + t * P2.x) / CountT;
        //double y = ((CountT - t) * P1.y + t * P2.y) / CountT;
        x += dx / CountT;
        y += dy / CountT;
        v((unsigned int)round(x), (unsigned int)round(y)) = Color;
    }
}

void IMG::Line1(TPoint P1, TPoint P2, TColorPix Color)
{
    int D;  int d;
    int A;  int b;
    int dx = (int)P2.x - (int)P1.x;
    int dy = (int)P2.y - (int)P1.y;
    
    if (abs(dx) >= abs(dy)) 
         { D = dx;  d = dy;  A = (int)P1.x;  b = (int)P1.y; }
    else { D = dy;  d = dx;  A = (int)P1.y;  b = (int)P1.x; }

    double error = 0;
    double k = (abs((double)d) + 1) / (abs((double)D) + 1);

   // printf("-- ( %d,  %d) -------- > ( %d,  %d) --- k = %f d=%d  D = %d  d = %d \n", P1.x, P1.y, P2.x, P2.y, k, d, D, d);
    v(P1.x, P1.y) = Color;
    for (int t = 0; t < abs(D); t++)
    {
        if (d != 0)
        {
            A += Sing(D);
            error += k;
            if (error >= 1) { error--;  b += Sing(d); } //if (d >= 0) b++; else b--;
        }
        else A += Sing(D);
        if (abs(dx) >= abs(dy)) v((unsigned int)A, (unsigned int)b) = Color; else v((unsigned int)b, (unsigned int)A) = Color;
    }
}

