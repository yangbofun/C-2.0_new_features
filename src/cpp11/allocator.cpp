#include<iostream>
#include<memory>
using namespace std;

class A {
public:
    A()=default;
    A(int a, int b) : a(a), b(b) {};
    void print() const { cout << "a: " << a << " b: " << b <<endl; };
private:
    int a;
    int b;
};

int main(){
    A* p = (A*) malloc(sizeof(A));
    allocator<A>().construct(p, 2, 4);
    p->print();   // a: 2  b: 4 
    allocator<A>().destroy(p);
}
