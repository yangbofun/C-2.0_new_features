#include <iostream>
using namespace std;

template<typename T>
void print(const T& t){ cout << t << endl << "end" <<endl;}  // 1
void print() {cout << "end" << endl;}  // 2
template <typename T, typename... Args>
void print(const T& t, const Args&... rest){
    cout << t << " ";
    print(rest...);
}

int main() {
    print(1, 1.1, "wangcai");
    return 0;
}