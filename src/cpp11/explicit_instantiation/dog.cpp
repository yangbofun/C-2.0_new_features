#include "dog.hpp"

template<typename T>
T Dog<T>::getHeight(){
    return height;
}

template class Dog<float>; // explicit instantiation
