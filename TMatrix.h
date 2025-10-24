#pragma once
#include <vector>
#include <stdio.h>
#pragma pack(push,1)
typedef struct TArea
{
    uint32_t   x1;
    uint32_t   y1;
    uint32_t   x2;
    uint32_t   y2;

    char name[8];
    void resize(int dx1, int dy1, int dx2, int dy2) { x1 += dx1;  y1 += dy1;  x2 += dx2;  y2 += dy2; }
} Area;
#pragma pack(pop)

template <typename Type>
class TMatrix 
{
public:
    TMatrix(unsigned int w = 1, unsigned int h = 1)
    {
        width = w; height = h;
        array.resize(width * height);
       // for (auto it = begin(); it < end(); it++) *it = { 0, 0, 0, 0 };
    }
    unsigned int width;
    unsigned int height;

//protected:
    inline char* data() { return reinterpret_cast<char*>(array.data()); }
    size_t size() const { return array.size(); } // Метод для получения размера контейнера
    void InitData(const Type& value) { for (auto it = array.begin(); it < array.end(); it++) *it = value;  };
    
    Type& v(unsigned int x, unsigned int y) { // Метод для доступа к элементу по индексу
        try
        {
            //if (x>=0 && y>=0 && x<width && y<height) 
            return array[x + y * width];
           // else return array[0];
        }
        catch (...)
        {
            printf("Error in v(%d, %d)  \n", x, y); //cin.get();
        }
    }

    void resize(unsigned int Sx = 1, unsigned int Sy = 1)
    {
        width = Sx; height = Sy;
        array.resize(width * height);
    }

    TMatrix<Type> GetSub(Area c)
    {
        TMatrix<Type> buf = TMatrix<Type>(c.x2 - c.x1, c.y2 - c.y1);
        for (uint32_t y = c.y1; y < c.y2; y++)
            for (uint32_t x = c.x1; x < c.x2; x++)
                buf.v(x - c.x1, y - c.y1) = v(x, y);

        return buf;
    }

//private:
    std::vector<Type> array; // Внутреннее хранилище элементов типа T
};

