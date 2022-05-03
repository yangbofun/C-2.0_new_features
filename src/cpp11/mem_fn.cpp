#include <iostream>
#include <functional>

using namespace std;

class Num{
public:
    int add(int a, int b){return a+b;} 
    //static int sub(int a, int b){return a+b;} 
};
int main(){
    Num n{};
    auto add = mem_fn(&Num::add);
    //auto sub = mem_fn(&Num::sub);
    add(n,1,2);
}