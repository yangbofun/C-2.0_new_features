#include <iostream>

using namespace std;

void f(int&& a){
    cout << a << endl;
}

template<typename T>
void g(T a) {
    int c = 10;
    T cc = c;
    cout << a<<endl;
}

template<typename T>
void h(T&& a) {
    T& c = 10;
    cout << c<<endl;
}


void e(int&& a){
    cout << a<<endl;
}
template<typename T>

void ee(T&& a){
    //e(a);
    e(std::forward<T>(a));
}

int main(){
    int b = 0;
    int& a = b;
    forward<int>(move(b));
    f(move(a));
    const int c = 1;
    //h(c);   
    int && d = 7;
    g(std::move(d));


    ee(2);
    ee(b);
    return 0;
}