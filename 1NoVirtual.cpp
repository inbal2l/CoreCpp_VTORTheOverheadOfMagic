/******************************************************************************

Example of Base, Derived : public Base, no virtual 

*******************************************************************************/
/******************************************************************************
Results:

Start:
Base Ctor
Derived Ctor
Base Dtor

*******************************************************************************/



#include <iostream>

using namespace std;

class Base
{
public:
    int m_i;
    Base()
    {
        cout<<"Base Ctor"<<endl;
    }
    
    ~Base()
    {
        cout<<"Base Dtor"<<endl;
    }
    void printMe()
    {
        cout<<"Hi, Base"<<endl;
    }
};

class Derived : public Base
{
    
public:
    int m_i;
    
    Derived()
    {
        cout<<"Derived Ctor"<<endl;    
    }
    
    ~Derived()
    {
        cout<<"Derived Ctor"<<endl;    
    }
    
    void printMe()
    {
        cout<<"Hi, Derived"<<endl;
    }
};


int main()
{
    cout<<"Start:"<<endl;
    Base *b = new Derived;
    delete b;
    
    return 0;
}
