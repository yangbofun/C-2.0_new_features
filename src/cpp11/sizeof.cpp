#include <iostream>
using namespace std;

template <typename... Args>
void func(Args... args){
    cout << sizeof...(Args) << endl;
    cout << sizeof...(args) << endl;
}

int main(){
    func(1, 1, 1);
    func(1, 1.0, "1");
}