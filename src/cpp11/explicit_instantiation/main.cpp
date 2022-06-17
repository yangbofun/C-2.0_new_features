#include <iostream>
#include "dog.hpp"
using namespace std;

//extern template class Dog<float>; 
int main() {
    Dog<float> dog("wangcai", 0.72);
    cout << dog.getHeight() << endl;
}

