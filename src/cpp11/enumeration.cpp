#include <iostream>
using namespace std;

enum class Values {A, B, C};
//enum Values {A, B, C};  // default type is int for the scoped enumeration's members
//enum class Color: long   // error, IntValues has been declared as the scoped enumeration
enum Color {A, B, C};
int main(){
    cout << static_cast<int>(Values::B) <<endl;
    cout << C <<endl;
}
