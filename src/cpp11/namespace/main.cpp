#include<iostream>
using namespace std;
namespace L{
    extern int aa;
    inline namespace M{}
    namespace M{
        extern int inline_bb;
    }
    extern int anonymous_b;  // link error
}
int main(){
    cout << L::aa  <<endl;
    cout << L::inline_bb << endl;
    cout << L::anonymous_b << endl;
}