#include <iostream>
#include <tuple>     // std::tuple, std::get, std::tie, std::ignore


template <class T>
void test(T value) {
    int a[std::tuple_size<T>::value];  // can be used at compile time

    std::cout << std::tuple_size<T>{} << ' '  // or at run time
              << sizeof a << ' ' << sizeof value << '\n';
}

// tuple example

int main() {
    std::tuple<int, char> foo(10, 'x');
    auto bar = std::make_tuple("test", 3.1, 14, 'y');

    std::get<2>(bar) = 100;  // access element

    int myint;
    char mychar;

    std::tie(myint, mychar) = foo;  // unpack elements
    std::tie(std::ignore, std::ignore, myint, mychar) =
        bar;  // unpack (with ignore)

    mychar = std::get<3>(bar);

    std::get<0>(foo) = std::get<2>(bar);
    std::get<1>(foo) = mychar;

    std::cout << "foo contains: ";
    std::cout << std::get<0>(foo) << ' ';
    std::cout << std::get<1>(foo) << '\n';
    test(std::make_tuple(1, 2, 3.14));

    return 0;
}