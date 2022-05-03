#include <iostream>

class SmartInt {
public:
    SmartInt(int i = 0):val(i){
        if ( i < -128 || i > 127) {
            throw std::out_of_range("Bad SmartInt value");
        }
    }
    explicit operator int() { return val; }  // explicit
private:
    char val;
};

int main() {
    SmartInt a(1);
    //a + 3;  // error, right if no explicit declaration above.
    int c = static_cast<int>(a) + 3;  // correct 
}