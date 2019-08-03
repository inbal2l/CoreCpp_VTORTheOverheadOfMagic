/******************************************************************************
			        Virtual Table OR The overhead of magic
			        ======================================
			
	Author: 		  Inbal Levi
	Description:	This file contains test you can run in order to do VT 
			          Benchmarking, is free for use, with open license.
	NOTES:		    Try running with flags -O0, -O1, -O2, -O3, -Os
			          Try running with PHOTO_SIZE 100, 1000, 10000 etc.
			          (USE "ulimit -s unlimited" to increase stack size)
*******************************************************************************/
#include <iostream>
#include <ctime>
#include <variant>

#define PHOTO_SIZE 2

using namespace std;        // shouldn't


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
struct FilterBright { };
struct FilterDark { };

struct VisitActivate
{
    constexpr void operator()(FilterBright& filter, int& pixel) const { pixel+=1; }
    constexpr void operator()(FilterDark&, int& pixel) const { pixel+=1; }
};


int main()
{
    int photo[PHOTO_SIZE][PHOTO_SIZE];
    
    // printPhoto(photo);

    std::variant<int> photoS[PHOTO_SIZE][PHOTO_SIZE];
    std::variant<FilterBright, FilterDark> MyFilter { FilterBright() };

    int Si,Sj;
    clock_t Sbegin = clock();

    for (Si=0;Si<PHOTO_SIZE;++Si)
    {
        for (Sj=0;Sj<PHOTO_SIZE;++Sj)
        { 
            std::visit(VisitActivate(), MyFilter, photoS[Si][Sj]);
        }
    }
    
    clock_t Send = clock();

    cout <<"Visitor implementation: "<< Send - Sbegin << endl;
}
