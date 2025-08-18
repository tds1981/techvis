#include "IMG.h"

inline void IMG::Set(unsigned int x, unsigned int y, TColorPix value)
{
		at(x + y * width) = value;
}
inline TColorPix IMG::Pix(unsigned int x, unsigned int y)
{
		return at(x + y * width);
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
       if (it->Blue + it->Green + it->Red > Level.Blue + Level.Green + Level.Red) { it->Blue = 255; it->Green = 255; it->Red = 255;}
       else { it->Blue = 0; it->Green = 0; it->Red = 0;}
}

shared_ptr<IMG> IMG::GetCopy()
{
    shared_ptr<IMG> buf = make_shared<IMG>(width, height);
    memcpy(buf->data(), data(), sizeof(TColorPix)*buf->size());
    return buf;
}





