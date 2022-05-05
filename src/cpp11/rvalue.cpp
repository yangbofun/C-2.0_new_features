#include <iostream>
using namespace std;

struct A {
    A() = default;
    A(const A& oth) { cout << "left value reference" << endl; }
    A(const A&& oth) { cout << "right value reference" << endl; }
};

int main() {
    A a;
    A a1(static_cast<A&&>(a));
    A a2(move(a));
    A a3(a);
}