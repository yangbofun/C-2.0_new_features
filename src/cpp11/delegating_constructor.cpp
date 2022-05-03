#include <string>
using namespace std;

class Dog {
   public:
    Dog(string name, int age, int height)
        : name(name), age(age), height(height) {}
    Dog(string name) : Dog(name, 0, 0) {}  // using delegating constructor
    Dog() : Dog("wangcai") {}  // using delegating constructor

   private:
    string name;
    int age;
    int height;
};