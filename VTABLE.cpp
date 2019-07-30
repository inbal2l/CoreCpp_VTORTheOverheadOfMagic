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
#include <ctime>

using namespace std;        // (shouldn't...)

#define PHOTO_SIZE 1000

void initPhoto(int photo[][PHOTO_SIZE])
{
    int i,j;
    for (i=0;i<PHOTO_SIZE;++i)
    {
        for (j=0;j<PHOTO_SIZE;++j)
        {
           photo[i][j]= (unsigned char) rand();
        }
    }
}

void printPhoto(int photo[][PHOTO_SIZE])
{
    int i,j;
    for (i=0;i<PHOTO_SIZE;++i)
    {
        for (j=0;j<PHOTO_SIZE;++j)
        {
           cout<<" %d "<< photo[i][j];
        }
        cout <<"\n";
    }
}

template <class DerivedFilter>
class BaseFilter
{
public:
    constexpr void Activate(int *pixel)
    {
        static_cast<DerivedFilter*>(this)->ImplementFilter(pixel);
    }
};

class FilterDerived : public BaseFilter<FilterDerived>
{
public:
    constexpr void ImplementFilter(int *pixel)
    {
        //cout << "FilterA implementation" << endl;
        *pixel-=1;
    }
};

class BaseFilterVirtual 
{
public:
    virtual inline void Activate(int *pixel) const
    {
        cout << "BaseFilterVirtual Activate" <<endl;
    }
    virtual ~BaseFilterVirtual() = default;
};


class FilterV : public BaseFilterVirtual
{
public:
    virtual inline void Activate(int *pixel) const override
    {
        //cout << "FilterV implementation" << endl;
        *pixel-=1;
    }
};

int main()
{
    int photo[PHOTO_SIZE][PHOTO_SIZE];
    
    initPhoto(photo);
    // printPhoto(photo);

    BaseFilterVirtual * pt_fv = new FilterV();
    clock_t Vbegin = clock();

    int vi,vj;
    for (vi=0;vi<PHOTO_SIZE;++vi)
    {
        for (vj=0;vj<PHOTO_SIZE;++vj)
        {
            pt_fv->Activate(&photo[vi][vj]);
        }
    }
    
    clock_t Vend = clock();
    delete ft_fv;      
    cout <<"Vtable implementation: "<< Vend - Vbegin << endl;
    
    FilterDerived fc;
    FilterDerived * pt_fc = &fc;

    clock_t Cbegin = clock();

    int i,j;
    for (i=0;i<PHOTO_SIZE;++i)
    {
        for (j=0;j<PHOTO_SIZE;++j)
        {
            pt_fc->Activate(&photo[i][j]);  
        }
    }
    
    clock_t Cend = clock();

    cout <<"CRTP implementation: "<< Cend - Cbegin << endl;
	
	
    pixel_t photoS[PHOTO_SIZE][PHOTO_SIZE];		// Recreating the photo as pixel_t type
    
    var_t filter{FilterBright()};
    int Si,Sj;
    clock_t Sbegin = clock();

    for (Si=0;Si<PHOTO_SIZE;++Si)
    {
        for (Sj=0;Sj<PHOTO_SIZE;++Sj)
        {
            std::visit (overload{
        [](FilterBright f, int& pixel)  { pixel+=1; cout << "Filter Bright "; },
        [](FilterDark f, int& pixel)  {  pixel-=1; cout << "Filter Bright ";  },
        }, filter, photoS[Si][Sj]);
        }
    }
    
    clock_t Send = clock();

    cout <<"Visitor implementation: "<< Send - Sbegin << endl;
}
