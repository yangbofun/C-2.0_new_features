#include <string>

template <typename T>
class Dog {
   public:
    Dog(std::string name, T height) : name(name), height(height) {}
    T getHeight();

   private:
    std::string name;
    T height;
};
