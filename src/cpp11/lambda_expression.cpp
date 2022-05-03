#include <iostream>
using namespace std;

void fcn() {
    int v = 42;
    auto f = [v]() mutable { return ++v; };
    cout << v << endl;  // 42
    v = 0;
    auto j = f();  // j == 43；
    cout << j << endl;
}

void fcn1() {
    int v = 42;
    auto f = [&v]() { return ++v; };
    cout << v << endl;  // 42
    v = 0;
    auto j = f();  // j == 1；
    cout << j << endl;
}

int main(){
    fcn();
    cout << "==============" <<endl;
    fcn1();

}