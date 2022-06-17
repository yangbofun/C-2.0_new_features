#include <iostream>
using namespace std;

class Foo{
public:
    Foo() = default;
    Foo(int a) : a(a) {}
    Foo& operator=(const Foo& other) &&  { 
        if (this == &other){
            return *this;
        }
        a = other.a; 
        return *this;
    }

    Foo operator+(const Foo & other) { 
        Foo ret;
        ret.a = other.a + this->a;
        return ret;
    }
private:
    int a;
};

int main(){
    Foo foo1(2);
    Foo foo2(3);
    Foo foo3(3);
    foo1 + foo2 = foo3;
    //foo2 = foo3; // compile-time error
}