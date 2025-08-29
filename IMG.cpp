#include "IMG.h"

inline void IMG::Set(unsigned int x, unsigned int y, TColorPix value) {  at(x + y * width) = value; }

inline TColorPix IMG::Pix(unsigned int x, unsigned int y)  { return at(x + y * width);}

TColorPix IMG::PixLim(unsigned int x, unsigned int y)
{
    unsigned int x_l = x;
    unsigned int y_l = y;
    if(x_l >= width)  x_l = width-1;
    if(y_l >= height) y_l = height-1;
    return at(x_l + y_l * width);
}

void IMG::ReSize(unsigned int Sx, unsigned int Sy, int mset)
{
	width = Sx; height = Sy;
	resize(Sx * Sy);
	memset(data(), mset, size());
}

shared_ptr<IMG> IMG::GetSubImg(Area c)  
{
    shared_ptr<IMG> buf = make_shared<IMG>(c.x2 - c.x1, c.y2 - c.y1);
	//TMatrix src = new TMatrix(c.x2 - c.x1, c.y2 - c.y1);
	//SubIMG->ReSize(c.x2 - c.x1, c.y2 - c.y1);

	for (uint32_t y = c.y1; y < c.y2; y++)
		for (uint32_t x = c.x1; x < c.x2; x++)
			buf->Set(x - c.x1, y - c.y1, Pix(x, y));
	
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
    for(auto it = begin(); it < end(); it++)
    {
        it->Red &= M.Red; it->Green &= M.Green;  it->Blue &= M.Blue;   it->A &= M.A;
    }
}

void IMG::Monochrom(TColorPix Level)
{  
   for (auto it = begin(); it < end(); it++)
       if (it->Blue + it->Green + it->Red + it->A > Level.Blue + Level.Green + Level.Red + Level.A) { *it = {0xff, 0xff, 0xff, 0xff};}
       else { *it = {0, 0, 0, 0};}
}

shared_ptr<IMG> IMG::GetCopy()
{
    shared_ptr<IMG> buf = make_shared<IMG>(width, height);
    memcpy(buf->data(), data(), sizeof(TColorPix)*buf->size());
    return buf;
}

double IMG::Brightness(int correction)
{
  double bright = 0;
  for (auto it = begin(); it < end(); it++)  { bright +=  ((double)it->Red*0.299 + (double)it->Green*0.587 + (double)it->Blue*0.114)/ (double)width*(double)height;  } 
  int bright_int = round(bright); 
     
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
   for (auto it = begin(); it < end(); it++) {*it = {d_mean[it->Blue], d_mean[it->Green], d_mean[it->Red], 0xff}; } 
   return  bright;
}

double IMG::BrightnessArea(TArea A)
{
  double bright = 0;
  double S = (A.x2 - A.x1)*(A.y2 - A.y1);  
  for (int y = A.y1; y <= A.y2; y++)
     for (int x = A.x1; x <= A.x2; x++)
     {
        bright +=  (double)Pix(x, y).Red*0.299 + (double)Pix(x, y).Green*0.587 + (double)Pix(x, y).Blue*0.114;
     } 
  bright /= S;
  return bright; 
}

void IMG::Contrast(TColorPix Level, int correction)
{
     for (auto it = begin(); it < end(); it++)
     {   
       int b; int g; int r; 

       if (it->Blue >= Level.Blue) { b = (int)it->Blue + correction;} else { b = (int)it->Blue - correction;}
       if (it->Green >= Level.Green) { g = (int)it->Green + correction;} else { g = (int)it->Green - correction;} 
       if (it->Red >= Level.Red) { r = (int)it->Red + correction;} else { r = (int)it->Red - correction;} 
       it->IntsToColor(b, g, r); 
     }
}



