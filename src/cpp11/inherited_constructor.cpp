#include <iostream>
using namespace std;

struct B1 { B1(int, ...) {} };
struct B2 { B2(double)   {} };
 
int get();
 
struct D1 : B1
{
    using B1::B1; // inherits B1(int, ...)
    int x;
    int y = get();
};
 
void test()
{
    D1 d(2, 3, 4); // OK: B1 is initialized by calling B1(2, 3, 4),
                   // then d.x is default-initialized (no initialization is performed),
                   // then d.y is initialized by calling get()
 
    //D1 e;          // Error: D1 has no default constructor
}
 
struct D2 : B2
{
    using B2::B2; // inherits B2(double)
    B1 b;
};
 
//D2 f(1.0); // error: B1 has no default constructor


struct C1{
    C1(int a) {cout << "C1" << endl;}
};
struct C2{ 
    C2(int a)  {cout << "C2" << endl;}
};
struct CC:C1,C2{
    using C1::C1;
    using C2::C2;
};
int main(){
    CC cc(2); // compiler-time error, more than one constructor.
}