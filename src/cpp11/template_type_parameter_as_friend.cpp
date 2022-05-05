#include <iostream>
using namespace std;

template <typename T>
class Foo {
friend T;

public:
    Foo() = default;

private:
    void print() { cout << "test...." << endl; }
};

class A {
public:
    A() = default;
    void func() { foo_a.print(); }
private:
    Foo<A> foo_a;
};

int main() {
    A a;
    a.func();
}