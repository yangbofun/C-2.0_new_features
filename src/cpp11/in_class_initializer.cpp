#include <iostream>
#include <string>

using namespace std;

struct Dog {
    Dog() : name("shiyi"), height(0.4) {}

    string name = "wangcai";
    float height = 0.7;
};
int main() {
    Dog dog;
    cout << dog.name << endl;    // shiyi
    cout << dog.height << endl;  // 0.4

    return 0;
}