/******************************************************************************

			Virtual Table OR The overhead of magic
			======================================
			
	Author: 		Inbal Levi
	Description:	This file contains test you can run in order to do VT 
			Benchmarking, is free for use, with open license.
	NOTES:		Try running with flags -O0, -O1, -O2, -O3, -Os
			Try running with PHOTO_SIZE 100, 1000, 10000 etc.
			(USE "ulimit -s unlimited" to increase stack size)

*******************************************************************************/

#include <iostream>
using namespace std;
#include <ctime>

#define PHOTO_SIZE 100
typedef unsigned char PIXEL;


void initPhoto(PIXEL photo[][PHOTO_SIZE])
{
    int i,j;
    for (i=0;i<PHOTO_SIZE;++i)
    {
        for (j=0;j<PHOTO_SIZE;++j)
        {
           photo[i][j]= (PIXEL) rand();
        }
    }
}

void printPhoto(PIXEL photo[][PHOTO_SIZE])
{
    int i,j;
    for (i=0;i<PHOTO_SIZE;++i)
    {
        for (j=0;j<PHOTO_SIZE;++j)
        {
           printf(" %d ", photo[i][j]);
        }
        printf("\n");
    }
}



template <class DerivedFilter>
class BaseFilter
{
public:
    inline void Activate(PIXEL *pixel)
    {
        static_cast<DerivedFilter*>(this)->ImplementFilter(pixel);
    }
};

class FilterA : public BaseFilter<FilterA>
{
public:
    inline void ImplementFilter(PIXEL *pixel)
    {
        *pixel-=1;
    }
};




class BaseFilterVirtual
{
public:
    virtual inline void Activate(PIXEL *pixel)
    {
        cout << "BaseFilterVirtual Activate" <<endl;
    }
};


class FilterB : public BaseFilterVirtual
{
public:
    virtual inline void Activate(PIXEL *pixel)
    {
        *pixel-=1;
    }

};



int main()
{
    PIXEL photo[PHOTO_SIZE][PHOTO_SIZE];
    
    initPhoto(photo);
    // printPhoto(photo);
    
    FilterA f1;  
    clock_t begin = clock();	// CPU Tickes 

    int i,j;
    for (i=0;i<PHOTO_SIZE;++i)
    {
        for (j=0;j<PHOTO_SIZE;++j)
        {
            f1.Activate(&photo[i][j]);  // Prints "Derived implementation"
        }
    }
    
    clock_t end = clock();	// CPU Tickes 
      
    printf("CRTP implementation: %lu\n", end - begin);

    
    BaseFilterVirtual *f2 = new FilterB();  
    clock_t Vbegin = clock();

    int vi,vj;
    for (vi=0;vi<PHOTO_SIZE;++vi)
    {
        for (vj=0;vj<PHOTO_SIZE;++vj)
        {
            f2->Activate(&photo[vi][vj]);  // Prints "Derived implementation"
        }
    }
    
    clock_t Vend = clock();
      
    printf("Vtable implementation: %lu\n", Vend - Vbegin);



    
    return 0;
}
