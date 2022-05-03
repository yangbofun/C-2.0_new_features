#include <iostream>
#include <vector>
#include <functional>
using namespace std;

template <typename T>
bool isLess(T a, T b) {
    return a < b;
}

int main() {
    int x = 5;
    vector<int> vec{13,12,4,5,2,10};
    auto isLessThanX = bind(isLess<int>, placeholders::_1, x);
    auto it = find_if(vec.begin(), vec.end(), isLessThanX);
    if (it != vec.end()) {
        cout << "target:" << *it << endl;
    }
    else {
        cout << "the number less than " << x << "does not exit" << endl;
    }
    return 0;
}