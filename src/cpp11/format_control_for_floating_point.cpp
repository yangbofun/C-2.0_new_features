#include <iostream>
#include <cmath>
using namespace std;

int main(){
    cout << "default format: " << 100 * sqrt(2) << '\n'                 // 141.421 
        << "scientific: " << scientific << 100 * sqrt(2) << '\n'        // 1.414214e+02 
        << "fixed decimal: " << fixed << 100 * sqrt(2) << '\n'          // 141.421356
        << "hexfloat: " << hexfloat << 100 * sqrt(2) << '\n'            // 0x1.1ad7bc01366b8p+7
        << "use defaults: " << defaultfloat << 100 * sqrt(2) << '\n';   // 141.421 
    return 0;
}