#include <iostream>
#include <initializer_list>
using namespace std;

void func(initializer_list<int> l){
    for (auto i : l) {
        cout << i << endl;
    }
}

int main(){
    initializer_list<int> l {1,2,3,4,5};
    func({1,2,3,4});
    func({1,2,3});

}