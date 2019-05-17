/******************************************************************************

Example of Base, Derived : public Base, no virtual 

*******************************************************************************/
/******************************************************************************
Results:

Start:
Base Ctor
Derived1 Ctor
Derived2 Ctor
DerivedBoth Ctor
Hi, DerivedBoth
DerivedBoth Dtor
Derived2 Dtor
Derived1 Dtor
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
    
    virtual ~Base()
    {
        cout<<"Base Dtor"<<endl;
    }
    virtual void printMe()
    {
        cout<<"Hi, Base"<<endl;
    }
};

class Derived1 : virtual public Base
{
    
public:
    int m_i;
    
    Derived1()
    {
        cout<<"Derived1 Ctor"<<endl;    
    }
    
    ~Derived1()
    {
        cout<<"Derived1 Dtor"<<endl;    
    }
    
    void printMe()
    {
        cout<<"Hi, Derived1"<<endl;
    }
};


class Derived2 : virtual public Base
{
    
public:
    int m_i;
    
    Derived2()
    {
        cout<<"Derived2 Ctor"<<endl;    
    }
    
    ~Derived2()
    {
        cout<<"Derived2 Dtor"<<endl;    
    }
    
    void printMe()
    {
        cout<<"Hi, Derived2"<<endl;
    }
};


class DerivedBoth : public Derived1, public Derived2
{
    
public:
    int m_i;
    
    DerivedBoth()
    {
        cout<<"DerivedBoth Ctor"<<endl;    
    }
    
    ~DerivedBoth()
    {
        cout<<"DerivedBoth Dtor"<<endl;    
    }
    
    void printMe()
    {
        cout<<"Hi, DerivedBoth"<<endl;
    }
};



int main()
{
    cout<<"Start:"<<endl;
    Base *b = new DerivedBoth;
    b->printMe();
    delete b;
    
    return 0;
}