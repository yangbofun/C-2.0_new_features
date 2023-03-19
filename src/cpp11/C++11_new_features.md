# C++11新增加特性

## 1.C++11新增加特性

1. =default，delete
- =default 如果我们没有定义构造函数，C++编译器会自动为我们创建一个默认构造函数。但是如果我们定义了一个构造函数，那么编译器就不会为我们再生成默认构造函数，=default可以达到这两者都存在。
- =delete. 如果用户没有定义类的4大函数（构造函数、copy构造函数、赋值运算符、析构函数）的话，C++编译器会为我们提供默认版本。但是如果我们不想要其中的某一个函数，那么我们可以用=delete来指定编译器不比为我们生成此默认函数。如从，则对应的函数不能使用。如果基类使用了delete，则子类对应的函数编译器不会去在生成，及子类和基类保持一致。
Class A{
public:
    A() = default; //提供了默认的构造函数
    A(int a):a(a){};
    A& operator=(const A&) = delete; //删除赋值构造。
private:
    int a;
};

int main(){
    A a(1);
    A aa(2);
    aa = a; //编译报错，赋值运算符被删除了。
}

2. allocator.construct 可以转到 任何构造函数
- allocator分配的内存是未构造的，我们需要在分配的内存中自己来构造对象。在新标准中其成员函数construct可以接受一个指针和零个或多个额外的参数，在指针指向的位置构造一个对象，额外参数则用来初始化构造的对象。这些额外的参数必须是与构造对象的类型相匹配的合法的初始化器。（deprecated in c++17, removed in c++20）
- 在早期版本construct只接受2个参数，一个是指针，一个是元素对象值，使用的是copy构造函数（推测），问不是任意参数的构造函数来构造对象。不过使用clang13，-std=c++03或-std=c++98，使用新增特性依旧可以编译通过。
#include<iostream>
#include<memory>
using namespace std;

class A {
public:
    A()=default;
    A(int a, int b) : a(a), b(b) {};
    void print() const { cout << "a: " << a << " b: " << b <<endl; };
private:
    int a;
    int b;
};

int main(){
    A* p = (A*) malloc(sizeof(A));
    allocator<A>().construct(p, 2, 4);
    p->print();   // a: 2  b: 4 
    allocator<A>().destroy(p); // destroy memory pointed by p
}

3. array container
- array 固定大小数组，支持
- 随机访问，不支持增加和删除元素
4. auto
- auto是让编译器帮我们分析表达式的类型。值得注意的是，auto 是让编译器通过初始值来推算变量的类型。显然auto定义的变量必须有初始值。
int a = 0;
int b = 1;
auto c = a + b;

- 使用auto也可以在一条语句中声明多个变量。因为一条语句只有一个基本类型，所以所有变量初始值的类型要保持一致。
auto i = 0, *p = &i;  // correct
auto sz = 0, pi = 3.14 // error, the types of sz and pi are different. 

- 编译器推断出来的auto类型有些时候和初始值类型并不完全一样，编译器会适当地改变结果类型使其更符合初始化规则。
1） 使用引用一般是使用引用的对象，当引用的对象作为auto关键字声明的对象初始值时，真正初始化的是引用的对象。所以引用类型会被忽略掉。
int i = 0, &r = i;  // r -> int&
auto a = r;  // a -> int,   

2) auto一般会忽略掉顶层const，而保留底层const。（顶层const是指指针是一个常量，底层const指指针指向的一个对象是const）
int i = 2;
const int ci = 3;
const int ci = i, &cr = ci;
auto b = ci;  // b -> int
auto c = cr;  // c-> int
auto d = &i;  // d -> int * 
auto e = &ci; // e -> const int *
*d = 5; 
*e = 5; //error

5. begin and end function
- 尽管能计算得到尾后指针，但这种用法极易出错。为了让指针的使用更简单、更安全， C++11新标准引入了两个名为begin和end的函数，定义在iterator头文件中。这两个函数与容器中的begin和end的成员函数功能类似。不过begin和end的形参可以是数组。
int ia[] = {0, 1, 2, 3};
int* beg = begin(ia);
int* last= end(ia);

6. bind function
- bind函数可以为函数绑定部分参数，并返回一个绑定参数后的新的函数。可以将其看为一个函数适配器。其一般形式为：
auto newCallable = bind(callable, arg_list);

假设一下场景，我们想使用find_if找到容器中一个小于x的数。
定义一个比较比较大小的函数 
bool isLess(T a, T b){return a < b;} 

但此函数不能作为find_if的参数来使用，因为find_if需要一个一元谓词。那么bind的作用就出现了，可以用bind函数来创建出一个一元谓词。
auto isLessThanX = bind(isLess, _1, x)；

如此，就可以使用find_if来找到小于x的第一个数了。
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

template <typename T>
bool isLess(T a, T b) {
    return a < b;
}

int main() {
    int x = 5;
    vector<int> vec{13,12,4,5,2,10};
    auto isLessThanX = bind(isLess<int>, placeholders::_1, x);
    auto it = find_if(vec.begin(), vec.end(), isLessThanX);
    if (it != vec.end()) {
        cout << "target:" << *it << endl;
    }
    else {
        cout << "the number less than " << x << "does not exit" << endl;
    }
    return 0;
}


7. bitset enhancements
- 增加的bitset的内置函数all.当所有位置置位时all()返回true。
- 增加to_ullong, 把bitset转为unsigned long long的数值
8. constexpr
8.1 constexpr变量
- 在一个复杂系统中，很难确定一个初始值是否是常量表达式。虽然可以定义const类型，但是有些时候const声明的并非常量表达式；(编译时而不是运行时能够确定其值的都是常量表达式)
int a = 1;  // variable，not constant expression
const int b = 2;  // constant expression
const int c = getSize();  // not constant expression

为了解决以上问题，提出了constexpr变量，让编译器来验证变量的值是否为常量表达式。
constexpr int a = 1;  // constant expression
constexpr int b = a+1;  // const expression
constexpr int c = func();  // error if func is not a constexpr func

- 注意：如果在constexpr声明中定义的是一个指针，那么限定符constexpr仅对指针有效，对指针指向的的对象无关。
const int *p = nullptr;  // p is a pointer point to const int variable.
constexpr int *p = nullptr;  // p is a const pointer point to int variable

8.2 constexpr 函数
- constexpr函数是指能用于常量表达式的函数。要求为：函数的返回值及所有形参的类型都必须是字面值类型，而且函数体中有且只有一条return语句（既然一切都是确定的，在逻辑上自然也就只有一条return语句）
constexpr int func() { return 10; }
const int a = func();

- 编译器会把constexpr表示的函数替换成函数结果。为了能在编译过程中随时展开，constexpr函数被隐式地定义为内联函数，所以通常放在头文件中。
  8.3 constexpr类
  数据成员都是字面值类型的聚合类是字面值常量类。（聚合类是一种没有用户定义的构造函数，没有私有（private）和保护（protected）非静态数据成员，没有基类，没有虚函数。这样的类可以由初始化列表初始化）。如果一个类不是聚合类，但他复合下面的要求，则它也是一个字面值常量类：
  - 数据成员都必须是字面值类型
  - 类必须有一个constexpr构造函数。
  - 如果一个数据成员含有类内初始值，则其初始值必须为常量表达式。或者，如果成员属于某种类型，则初始值必须使用成员自己的constexpr构造函数。
  - 类必须使用析构函数的默认定义，改成员负责销毁类的对象。
9. decltype
- 有些时候会遇到这种情况：希望从表达式的类型推断出要定义的变量的类型，但是不想用该表达式的值初始化变量。为了满足这一要求，c++11引入了第二种类型说明符decltype，编译器会分析表达式并得到它的类型，却不实际计算他的值。
decltype（f()）sum = x;  // the type of sum is same as the return type of f()

编译器并没有调用f()，而是使用当调用发生时，f()的返回值类型作为sum的类型。
- decltype在处理顶层const时和auto不同。如果decltype使用的是一个变量，则decltype返回该变量的类型（包括顶层const和引用在内）。
  const int ci = 0, &cj = ci;
  decltype(ci) x = 0;  // x -> const int
  decltype(cj) y = x;  // y -> const int&
  decltype(cj> z;  // z -> const int&. error, because of no initialization.

  - 值得注意的是引用从来都是作为一个所指对象的同义词出现，只有在decltype处是一个例外。
- decltype使用的表达式不是一个变量，则decltype返回表达式结果对应的类型。
  - 注意，当返回的结果可以作为左值时，decltype返回的是一个引用。
  int i = 42, *p = &i, &r = i;
  decltype(r+0) b; // b -> int
  decltype(*p) c; // c -> int&, error with not initialization

- decltype的结果类型与表达式形式密切相关。有一种形式需要特别注意，如果变量名上加上小括号，则得到的是引用，如果不加小括号则得到的结果是变量的类型。即decltype((variable))的结果永远是引用，decltype(variable)的结果为variable本身的类型，除非variable是引用时，其结果才是引用。其原因是加上一层或多层小括号，编译器就会认为他是一个表达式。变量是一个可以作为赋值语句左值的特殊表达式。所以decltype就会得到引用类型和上面的*p类似，*p就是一个表达式。
- 对于函数返回类型的推导，我们可以使用尾置返回类型，尾置返回类型可以使用decltype推导。
int func(int* a, int b) { return *a + b; }
auto f1(int) -> int (*) (int*, int);  // 1.
auto f1(int) -> decltype(func);  // same as 1.

10. delegating constructor
- C++11扩展了委托构造函数初始值的功能，使得我们可以定义所谓的委托构造函数。一个委托构造函数可以使用它所属类的其他构造函数执行它自己的初始化过程。
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

  - 注意使用了委托构造函数后，“:”后不能再用初始化列表初始化成员变量。因为构造过程已经委托给了其他构造函数。否则编译会报错。
- 执行顺序：先执行委托构造函数初始化列表初始化，然后是委托函数的函数体，至此委托执行的部分已经完毕，然后开始执行委托者自己的函数体。
11.  division rounding
- 对于整数除法取整，C++早期版本允许结果为负值的上向上或向下取整。C++11新标准规定商一律向0取证，（即直接去掉小数部分）
12. enumeration
12.1 scoped enumeration
- C++11新引入了限定作用域的枚举类型（scoped enumeration)。至此，枚举类型包含两种：限定作用域的和非限定作用域的。
- 定义限定作用域的枚举类型的一般形式是：关键字enum class（也可等价的使用enum struct)，随后枚举类型的名字以及用花括号括起来以逗号分隔的枚举成员列表。
enum class OpenModes { INPUT, OUTPUT, APPEND };

- 定义非限定的枚举类型，只需吧class删除掉就行。
enum OpenModes { INPUT, OUTPUT, APPEND };

- 非限定枚举和限定枚举两者的区别：作用域是否被限定
enum Color { RED, YELLOW, GREEN }  // not scoped enumeration
enum StopLight { RED, YELLOW, GREEN}  // error, duplicated the members of enumeraton
enum class Peppers { RED, YELLOW, GREEN }  // right, scoped enumeration

Color eyes = GREEN;  // right
Peppers p = GREEN;  // error

 Color hair = Color::RED;  // right, explicitly
 Peppers p2 = Peppers::RED;  // right 


12.2 forward declaration
- 在C++11中我们可以在enum的名字后面加上冒号以及我们想在改enum中使用的类型来指定enum是用哪种整数类型表示的。
enum Color : unsigned char {
    RED=8, YELLOW=20, GREEN=30
}

12.3 controlling representation
- C++11新增可以提前声明枚举类型。enum 的前置声明（无论显示地还是隐式地）必须指定其成员大小。限定类型的枚举隐式的使用int作为其成员类型。
enum class Values;  // default type is int for the scoped enumeration's members
enum Values;  // error, IntValues has been declared as the scoped enumeration
enum class Color : long;  // error, type different.

13. explicit conversion operator
- C++11引入了显示类型转化运算符。可解决由于隐式类型转换导致的意想不到的错误。(隐式转化要谨慎定义，虽然提供了方便，但却带来了隐患)。在早期的C++版本中，如果想定一个bool类型转化，则是隐式的。那么会出现下面的问题。假设cin内定义了一个bool的隐式类型转化。
int i = 42；
cin << i;  // should be illegal.

如果cin想作为istream，输入流不存在运算符 << ,本应该编译报错。但是cin会隐式的转化为bool，而bool会提升为int，从而变成了0<<i;与预期结果相差很大。那么早期的C++是使用operator void*()来避免这中问题，从而达到与operator bool()相似的效果。而在C++11中，由于出现了explicit，直接定义为explicit operator bool()就行了。所以放弃了早期版本中istream内的operator void*()。
- 用法：
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
    a + 3;  // error, right if no explicit declaration above.
    int c = static_cast<int>(a) + 3;  // correct 
}

当类型转化运算符是显式的时候，必须使用显式的强制类型转化才可以。不过存在一个意外：如果表达式被用作条件，则编译器会自动使用显式类型转化，话句话说，当表达式出现在下列位置时，显示类型转化将会被隐式的执行：
  - if、while及do语句的条件部分
  - for语句头的条件表达式
  - 逻辑非运算符（!）、逻辑或运算符（||）、逻辑与运算符（&&）的运算对象
  - 条件运算符的条件表达式
向bool类型的转化一般用在条件部分，因此operator bool()一般定义为explicit。
14. explicit instantiation
- 在早期C++版本，模板在使用时进行实例化，那么就会出现一个问题，模板在多处使用，就会实例化好多次，这无形中增加了编译的时间以及生成的二进制文件的大小。为解决此问题，C++11新增了extern template功能，即extern template declaration。那么再结合模板的显示实例化就可以解决上面出现的问题。
// dog.hpp
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


// dog.cpp
#include "dog.hpp"

template<typename T>
T Dog<T>::getHeight(){
    return height;
}

template class Dog<float>;  // explicit instantiation

// main.cpp
#include <iostream>
#include "dog.hpp"
using namespace std;

extern template class Dog<float>; // means Dog<float> definition in other file.
int main() {
    Dog<float> dog("wangcai", 0.72);  // not instantiation Dog with float, because of extern declaration above.
    cout << dog.getHeight() << endl;
}

- 补充一个模板的小细节，模板的声明与定义一般放在一起，放在头文件中。因为模板的编译是在实例化时，产生对应的实例化版本，如果声明放在头文件，定义放在cpp文件，那么在main.cpp中由于只include了头文件，从而会找不到定义，编译会报错。但是我们也可以在定义的cpp文件中进行显示的声明，来达到在编译cpp文件时，实例化对应的版本。由此就变成了函数或变量在其他文件中定义的情况了。
15. final class
- C++11新增了一种防止继承发生的方法，即在类名后价格final关键字。
16. format control for floating-point
- C++11新增了hexfloat和defaultfloat。在流中控制字符串格式时，hexfloat操纵符表示浮点数使用十六进制格式，defaultfloat操纵符则会把流恢复到默认状态。
cout << "default format: " << 100 * sqrt(2) << '\n'
     << "scientific: " << scientific << 100 * sqrt(2) << '\n'
     << "fixed decimal: " << fixed << 100 * sqrt(2) << '\n'
     << "hexfloat: " << hexfloat << 100 * sqrt(2) << '\n'
     << "use defaults: " << defaultfloat << 100 * sqrt(2) << '\n';
/**
the output:
default format: 141.421
scientific: 1.414214e+02
fixed decimal: 141.421356
hexfloat: 0x1.1ad7bc01366b8p+7
use defaults: 141.421
*/

17. forward function
- C++11标准库添加了一个forward函数。其声明如下
template <class T> T&& forward (typename remove_reference<T>::type& arg) noexcept;
template <class T> T&& forward (typename remove_reference<T>::type&& arg) noexcept;

注意观察此函数的特点，从简单到复杂，先抛去完美转发等概念。单看这一个函数。我们能得到如下信息
  - 这是一个模板函数，模板参数为T
  - 此函数的返回值类型为T&&
  - 此函数重载了两个版本，一个是形参为左值，一个是形参为右值。
我们看下他的具体实现（clang libc++）
template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
_Tp&&
forward(typename remove_reference<_Tp>::type& __t) _NOEXCEPT    // 1.
{
    return static_cast<_Tp&&>(__t);  
}

template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
_Tp&&
forward(typename remove_reference<_Tp>::type&& __t) _NOEXCEPT    // 2.
{
    static_assert(!is_lvalue_reference<_Tp>::value,
                  "can not forward an rvalue as an lvalue");
    return static_cast<_Tp&&>(__t);  
}

可以看出forward函数只是个类型转化函数而已。先看第一个函数，此函数把左值引用转化为了_Tp&&的类型。由于引用会保留底层const，所以参数的const属性会保存在_Tp中，
int main(){
    int a = 10;
    forward<int>(a);   // return type: int&& -> int&&
    forward<int&>(a);   // return type: int& && -> int&
    forward<int&&>(a);  // return type: int&& && -> int&& 
}


(引用折叠，只有一种情况导致最终的折叠类型是T&&, 那就是T&& &&. 其余情况全是int&
  - T & 、 T& &、 T& && 、T&& &  --> T&
  - T&& &&  --> T&&
)
再看第二个函数：他是个右值引用版本不过里面多个static_assert（可以在编译期来检查错误）。这个static_assert语句，解释起来就是，调用右值引用版本的forward时，forward的模板参数不能实例化为左值引用，也就是返回值不能是左值。转为一句话即为：无法使用forward函数把类型由右值引用转化为左值引用。
int main(){
    int a = 10;
    std::forward<int&>(std::move(a));  // compile-time error 
}  


总结起来就是：
  - 左值可以转化为左值或右值
  - 右值不能转化为左值。
那么这个forward函数有什么作用呢？
我们知道在模板函数推导参数的过程中，会出现丢失参数的部分类型信息（比如const信息以及左值引用信息）
template<typename T>
void func(T a){
    cout << a <<endl;
}
int main(){
    int a = 10;
    func(a);   // T -> int
    const int a = 10;
    func(a)   // T -> int
    int& b = a;
    func(b)  // T -> int 
    func(std::move(b) // T -> int&& ， ok
}

为了解决这个问题，引用折叠便排上了用场。
template<typename T>
void func(T&& a){
    cout << a <<endl;
}
int main(){
    int a = 10;
    func(a);   // T -> int&
    const int a = 10;
    func(a)   // T -> const int&
    int& b = a;
    func(b)  // T -> int&
    func(std::move(b)) // T -> int
}

forward是在利用引用折叠的基础上，实现其功能的。
假设两个函数f和g， 为了实现上面提到的，在模板推导中保留参数的const和左值引用信息。我们的f定义为右值引用的模板函数.然后f调用了g函数。
template <typename T>
void f(T&& a){
    g(a);    // mark for compile-error
    g(std::forward<T>(a));  // right
}
void g(int&& c){
    cout << c << endl;
}
int main(){
    f(4); // T->int,  compile-time error on g(a). No g(int a) function.
}

由此可见forward实现了一种完美转发的机制。他完美在哪，就是在我们能把f函数的实参类型传递给f函数内部调用的其他函数，只要我们用forward处理下被调用的内部函数额实参。
18. forward_list container
- C++新增了一个容器类型forward_list用来表示单向链表。
19. function interface to callable objects
- C++11标准库提供了一个function的函数，std::function可以存储、赋值、调用可调用对象（函数、lambda表达式、bind表达式、函数对象、以及指向成员函数和成员数据的指针(成员函数的话，调用时第一个参数是类的对象其他参数为成员函数的参数)。
#include <functional>
#include <iostream>

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};

void print_num(int i)
{
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};

int main()
{
    // store a free function
    std::function<void(int)> f_display = print_num;
    f_display(-9);

    // store a lambda
    std::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();
     
    // store the result of a call to std::bind
    std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();
     
    // store a call to a member function
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);
     
    // store a call to a data member accessor
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';
     
    // store a call to a member function and object
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    f_add_display2(2);
     
    // store a call to a member function and object ptr
    std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    f_add_display3(3);
     
    // store a call to a function object
    std::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);
     
    auto factorial = [](int n) {
        // store a lambda object to emulate "recursive lambda"; aware of extra overhead
        std::function<int(int)> fac = [&](int n){ return (n < 2) ? 1 : n*fac(n-1); };
        // note that "auto fac = [&](int n){...};" does not work in recursive calls
        return fac(n);
    };
    for (int i{5}; i != 8; ++i) { std::cout << i << "! = " << factorial(i) << ";  "; }
}

之所以引出function，是为了可以统一上述的调用对象到一个function类型，这样方便函数统一调用。
20. in-class initializer
- C++11新规定，可以为类内数据成员提供一个类内初始值，用于初始化成员变量，没有初始值的成员变量将被默认初始化。如此便可以对引用类型进行初始化。
- 关于类内初始值和初始化列表初始化的顺序问题，由下例可知，最终执行的是初始化列表初始化。其实类内初始化并没有执行，因为类内初始化和初始化列表初始化都是初始化动作，只能执行一个。
#include <iostream>
#include <string>

using namespace std;

struct Dog {
    Dog() : name("shiyi"), height(0.4) {}

    string name = "wangcai";
    float height = 0.7;
};
int main() {
    Dog dog;
    cout << dog.name << endl;    // shiyi
    cout << dog.height << endl;  // 0.4
    return 0;
}

21. inherited constructor
- C++11 新标准中，派生类能够重用其基类定义的构造函数。
struct B1 { B1(int, ...) {} };
struct B2 { B2(double)   {} };
 int get();
 struct D1 : B1{
    using B1::B1; // inherits B1(int, ...)
    int x;
    int y = get();
};
 void test()
{
    D1 d(2, 3, 4); // OK: B1 is initialized by calling B1(2, 3, 4),
                   // then d.x is default-initialized (no initialization is performed),
                   // then d.y is initialized by calling get()

    D1 e;          // Error: D1 has no default constructor
}
 struct D2 : B2{
    using B2::B2; // inherits B2(double)
    B1 b;
};

D2 f(1.0); // error: B1 has no default constructor

using语句只是告诉编译器，在此处生成子类的构造函数，对于基类的每个构造函数，子类都会生成一个对应的构造函数。换句话说，对于基类的每个都早函数，编译器都会在子类中生成一个形参列表完全相同的构造函数。但是如果是多继承，且基类的构造函数相同（既具有完全相同的形参列表），则会编译报错。
struct C1{
    C1(int a) {cout << "C1" << endl;}
};
struct C2{ 
    C2(int a)  {cout << "C2" << endl;}
};
struct CC:C1,C2{
    using C1::C1;
    using C2::C2;
};
int main(){
    CC cc(2); // compiler-time error, more than one constructor.
}

22. initializer_list
- C++11提供了std::initializer_list<T>类型， std::initializer_list<T> is a lightweight proxy object that provides access to an array of objects of type const T. 
  - initializer_list的非空构造函数由编译器提供，会使用copy-initailize底层array<T, N>的值（数据类型根据实际情况会可能发生narrow）
  - initializer_list的copy操纵不会copy底层的数据结构。它只是提供了一种访问底层数组的方式。
  - initialzier_list提供了begin()和end()，所以可以使用range-for来遍历元素。
  initializer_list<int> l{1,2,3,4};

- 当函数行参的类型相同，但是数量不定时，可以用initializer_list作为形参。
void func(initializer_list<int> l){
    for (auto i : l) {
        cout << i << endl;
    }
}
int main() {
    func({1,2,3})
    func({1,2})
}

- C++11类内提供了参数为initializer_list的构造函数。
23. inline namespace
- C++11引入了一种新的命名空间，叫做内联命名空间（inline namespace）。和普通的嵌套命名空间不同，内联命名空间中的名字可以被外层命名空间直接使用。也就是说我们无需在内联命名空间的名字前面加上表示该命名空间的前缀，通过外层命名空间的名字就可以直接访问到它。
namespace M{
    inline namespace N{
        class Cls{};
    }
}
int main(){
    using namespace M;
    Cls cls;  // replace N::Cls.
}

- 内联命名空间inline必须出现在第一次定义的地方，后续再打开命名空间的时候可以写inline，也可以不写。
namespace M{
    inline namespace N{}
    namespace N{           // the keyword "inline" is omitted
        class Cls{};
    }
}

24. lambda expression
- lambda表达式表示的是一个可调用的代码单元。我们可以将其理解为一个未命名的内联函数。一个lambda表达式具有如下形式：
[capture list] (parameter list) -> return type {function body}
其中parameter list 和 return type是可选的。但必须包含capture list和function body。
auto f = []{return 42;}
auto f1 = [&] (int a) -> int { return a+1;}

- lambda表达式不能有默认参数，因此一个lambda表达式实参数目永远与形参数目相等。
- capture list可以让lambda表达式得到其所在函数中局部变量的值。包括值捕获、引用捕获、隐式捕获
int v = 0;
int w = 1;
auto f =[v] () {return v;}  // value capture
auto f1 = [&v] () {return v;}  // reference capture

auto f2 = [=] () {return v + w;}  //implicit value capture
auto f2 = [&] () {return v + w;}  // implicit reference capture

- 可变lambda。默认情况下，对于一个值copy变量，lambda不会去改变其值。如果我们希望能改变一个值捕获变量的值，就必须在形参列表后添加mutable。对于一个引用捕获的变量是否可以修改，依赖于此引用指向的是一个const类型还是非const类型。

void fcn() {
    int v = 42;
    auto f = [v]() mutable { return ++v; };
    cout << v << endl;  // 42
    v = 0;
    auto j = f();  
    cout << j << endl;  // 43;
}

void fcn1() {
    int v = 42;
    auto f = [&v]() { return ++v; };
    cout << v << endl;  // 42
    v = 0;
    auto j = f();  
    cout << j << endl;  // 1
}

25. list initializatoin
- C++11允许使用花括号括起来的初始值列表作为赋值语句的右侧运算对象。
- 可以使用初始化列表初始化容器。
- 初始化动态分配数组。
- 初始化动态对象。
- 初始化pair
// initialization
int k = {3};

// initialze container
vector<int> vi = {1,2,3,4,5};
map<int, string> mp {{1,"one"}, {2, "two"}, {3, "three"}};
set<int> st {1, 2, 3}

//dynamic array
int*p = new int[5]{1,2,3,4,5};

//dynamic object
int *p = new int (5)  // older method
int *n = new int {5}  // new method. C++11
vector<int> * vv = new vector<int>{1,2,3,4,5} // new method. C++11

// pair
pair<int, float> p {1, 1.2};   // initialization
//the other methods of creating pair.
pair<int, float> p1(1,1.2);
pair<int, flaot> p2 = make_pair<int, float>(1,1.2);

- C++11允许使用花括号括起来的值的列表作为函数返回值。
vector<int> func(){
    return {1,2,3};
}

pair<int, string> func1(){
    return {1, "one"};
}

26. long long
- C++11新增long long数据类型。另外，C++规定：一个int至少和一个short一样大。一个long至少和一个int一样大，一个long long至少和一个long一样大。
27. mem_fn function
- C++11，新增加了mem_fn函数，可以将成员函数作为调用对象。它基于成员函数生成了一个可调用对象。function也可以实现生成成员函数调用对象的功能，不过我们必须提供成员的调用形式。和function一样，mem_fn也定义在头文件functional中。与function不同的是，mem_fn来让编译器推断成员的类型，并且mem_fn只能用于成员函数生成调用对象。
class Num{
public:
    int add(int a, int b){return a+b;} 
    //static int sub(int a, int b){return a+b;}  //static
};
int main(){
    Num n{};
    auto add = mem_fn(&Num::add);
    //auto sub = mem_fn(&Num::sub);  // error, can't be static
    add(n,1,2);
}

28. move function
- C++11新标准库添加了一个move函数，用来将一个左值引用类型转化为右值引用。
- move的返回类型可以表述为static_cast<typename std::remove_reference<T>::type&&>(t)
int a = 10;
int &&c = std::move(a);


- move调用告诉编译器：“我们有一个左值，但我们希望像一个右值一样处理它”。因此我们必须认识到调用move就意味着承诺：除了对a的赋值和销毁它外，我们将不在使用它。在调用完move后，我们不能对移动后的源对象的值做任何假设。
- 其实move很简单，本质上就是转化左值类型为一个右值类型。别的什么都没做。那么为什么说move后的对象不要再进行除了赋值和销毁外的其他操作呢。其实是可以的，但是不推荐，因为有时会报错。这是因为我们把move返回的右值引用类型的对象，作为形参传给其他函数，其他函数就会按照右值引用约定俗成的条例来执行，那就是这个右值引用的值，其他人已经不在使用了，那么为了效率，我就可以对它内部用指针执行的内存块，通过浅copy获得，并把原来的指针设为nullptr。标准库的很多右值引用版本的构造函数的具体实现就是这么处理的。
#include <iostream>
#include <utility>
#include <string>
#include <list>
#include<vector>


using namespace std;

int main1(){
    int a = 10;
    int && c = move(a);
    cout << a <<endl;  // 10
    a = 2;
    cout << c <<endl;   // 2
    int* p = new int (3);
    move(p);
    cout << *p <<endl;  // 3
    list<int> l {1,2,3,4};
    move(l);
    cout << l.front() << endl;  // 1
    string s{"wangcai"};
    move(s);
    cout << s<<endl;  // wangcai
    return 0;
}

int main()
{
    std::string str = "Salut";
    std::vector<std::string> v;

    //std::cout << "After copy, str is " << str << '\n';
    // uses the push_back(const T&) overload, which means 
    // we'll incur the cost of copying str
    v.push_back(str);
    std::cout << "After copy, str is " << str << '\n';  // "Sault"
     
    // uses the rvalue reference push_back(T&&) overload, 
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is " << str << '\n';   // ""
     
    std::cout << "The contents of the vector are { " << v[0]   // "Sault"
              << ", " << v[1] << " }\n";                       // "Sault"
}

29. move constructor
- C++11新增加了移动构造函数和移动赋值运算符 。
vector( vector&& other )；
vector& operator=( vector&& other )；

- 在移动构造函数中，要避免使用深copy。要严格按照移动语义约定俗称的规则来执行，进行浅copy。
30. move iterator
- C++11添加了一种新的迭代器，移动迭代器。用于将就内存中的元素快速copy到新内存中。一般来说一个迭代器返回的是一个左值引用，但是移动迭代器返回的是一个右值引用。我们可以通过使用标准库的make_move_iterator将一个普通迭代器转化为一个移动迭代器。
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <string>
#include <iterator>
 auto print = [](auto const rem, auto const& seq) {
    std::cout << rem;
    for ( auto const& str : seq)
            std::cout << str << ' ';
    std::cout << '\n';
};
 int main()
{
    std::list<std::string> s{"one", "two", "three"};

    std::vector<std::string> v1(s.begin(), s.end()); // copy

    std::vector<std::string> v2(std::make_move_iterator(s.begin()),
                                std::make_move_iterator(s.end())); // move

    print("v1 now holds: ", v1);   // v1 now holds: "one" "two" "three" 
    print("v2 now holds: ", v2);  // v2 now holds: "one" "two" "three"
    print("original list now holds: ", s);  // original list now holds: "" "" ""
}


31. move-enabled this pointer
- 先看一个例子：
int a = 1;
int b = 2;
a+b = 5; // error, rvalue can't be assigned

// but
string str1 = "111";
string str2 = "222";
str1 + str2 = "555";  // no error

上面例子中str1 + str2是一个右值，但是却可以对一个右值进行赋值操作。˙不太符和人们的想法。在旧标准中我们无法阻止人们使用这种赋值方式。那么为了兼容性，新标准库仍然允许对右值进行赋值。但是，我们可能希望自己的类中组织这种用法。在这种情况下，我们强制等号左侧的运算对象（即this之指向的对象）是一个左值。
- 指定this是左值的方法和和指定this指向的对象是const的方法类似，即在参数列表后面放一个引用限定符。
#include <iostream>
using namespace std;

class Foo{
public:
    Foo() = default;
    Foo(int a) : a(a) {}
    Foo& operator=(const Foo& other) &&  {   // mark1
        if (this == &other){
            return *this;
        }
        a = other.a; 
        return *this;
    }

    Foo operator+(const Foo & other) { 
        Foo ret;
        ret.a = other.a + this->a;
        return ret;
    }
private:
    int a;
};

int main(){
    Foo foo1(2);
    Foo foo2(3);
    Foo foo3(3);
    foo1 + foo2 = foo3;  // mark2
}

- 经实验，mark1位置，写&&和不写&&都可以被调用，都表示this是个右值。当为&时，表示this是左值，此时mark2处，编译就会报错。
- 当const限定符和&同时存在使，const必须放在&的前面。
32. noexcept
32.1. noexcept说明符
- C++11新标准中，可以通过noexcept说明指定某个函数不会抛出异常。其形式是关键字noexcept紧跟在函数的参数列表后面，用于标识该函数不会抛出异常。
- 对于告诉编译器此函数不会抛出异常是大有裨益的，首先，知道函数不会抛出异常有利于简化调用该函数的代码；其次，有利于编译器执行某些特殊的优化操作，而这些优化操作可能并不适用于可能出错的代码。
- 对于一个函数来说，noexcept说明要么出现在该函数的所有声明语句和定义中，要么一次也不出现。该说明应该在函数的尾置返回类型之前。也可以在函数指针的声明和定义中指定noexcept。在typedef或类型别名中则不能出现noexcept。在成员函数中，noexcept说明符需要在跟在const及引用限定符之后，而在final、override或虚函数的=0之前。
- 读者需要清楚的一个事实，编译器并不会在编译时检查noexcept说明。实际上，如果在一个函数在说明了noexcept的同时又含有throw语句或者调用了可能抛出异常的其他函数。编译器将顺利编译通过，并不会因为违反这中说明情况而报错（不排除个别编译器会对这种用法提出警告）
void f() noexcept {
    throw exception();
}

因此可能出现一种情况：尽管函数声明了它不会抛出异常，但实际上还是抛出了。一旦一个noexcept函数抛出了异常，程序就会调用terminate以确保遵守在运行时不抛出异常的承诺。上述过程是否执行站展开未做约定，因此noexcept可以用在两种情况下：一种是我们确认函数不会抛出异常，二是我们根本不知道如果处理异常。
- 指明某个函数不会抛出异常可以令该函数的调用者不必再考虑如果处理异常。无论是函数确实不抛出异常，还是程序终止，调用者都无须为此负责。
32.2 noexcept运算符
- noexcept是一个一元运算符。它的返回值是一个bool类型的右值常量表达式。用于表示给定的表达式是否会抛出异常。
noexcept(func(i)); 

- 如果func(i)在运算中，抛出异常则返回false，否则返回true。
33. nullptr
- C++11新引入了nullptr字面值，来初始化一个指针为空指针。
34. random-number library
- C++11提供了用于生成随机数的random库，放在random头文件中。内容比较独立，在此省略，具体请见https://en.cppreference.com/w/cpp/header/random
35. range for statement
- C++11提供了范围for语句。
vector<int> v{1,2,3,4,5};
for (auto& i : v) {
    cout << i << " ";
}

- 一个容器能否使用范围for遍历，取决于这个容器内是否实现了返回迭代器的成员函数begin()和end()。范围for等价于下面的传统形式：
for(auto i = v.begin(); i != v.end(); ++i){}

36. regular expression-library
- C++11提供了正则表达式库，定义在regex头文件中。内容表独立，具体用法请见https://en.cppreference.com/w/cpp/header/regex。
37. rvalue reference
- C++11新增了右值引用类型。使用&&来获取右值引用。
- 虽然不能隐式的将一个左值引用转为右值引用，但可以使用static_cast显式地将一个左值引用转为右值引用。使用std::move可以达到同样的目的，我们推荐使用std::move。这样方便统一查找左值转右值的代码位置（通过搜索move即可）
#include <iostream>
using namespace std;

struct A {
    A() = default;
    A(const A& oth) { cout << "left value reference" << endl; }
    A(const A&& oth) { cout << "right value reference" << endl; }
};

int main() {
    A a;
    A a1(static_cast<A&&>(a)); 
    A a2(move(a));
    A a3(a);
}
/**
right value reference
right value reference
left value reference
*/

38. sizeof data member
- C++11允许我们使用sizeof Foo::m来获取Foo类内成员m的大小，而不需去创建一个Foo的对象。这是因为sizeof运算符无需我们提供一个具体的对象，毕竟想获取一个成员的大小是无需真的获取该成员的。
- sizeof不会实际求运算对象的值。sizeof空指针的解引用是不会报错的，应为空指针的解引用并未真正的运行。不需要真的解引用也能知道其所指对象的类型。
39. sizeof... operator
- 当我们想知道一个包中有多少个元素时，可以使用sizeof...运算符。和sizeof类似，sizeof...也返回一个常量表达式
#include <iostream>
using namespace std;

template <typename... Args>
void func(Args... args){
    cout << sizeof...(Args) << endl;  // the number of type of parameter 
    cout << sizeof...(args) << endl;  // the number of func parameter 
}

int main(){
    func(1, 1, 1);   // 3, 3
    func(1, 1.0, "1");  // 3, 3
}

40. smart pointer
- C++11新增了三个智能指针：share_ptr、unique_ptr、weak_ptr。智能指针的主要作用是为了使用裸指针指向动态申请申请内存，由于程序员的疏忽所带来的内存泄漏问题。
40.1 shared_ptr
- share_ptr，内部使用了引用计数的原理。构造时引用计数加1，析构时，应用计数减一，当计数为0时，真正的析构指向的底层数据，并释放指向的底层内存。
- 创建shared_ptr。可以使用make_shared函数来创建，此函数调用了shared_ptr所要指向对象的构造函数。其形参的组织和emplace函数类似。也可以直接调用构造函数来创建。
shared_ptr<int> p = make_shared<int> (42); 
shared_ptr<string> pstr = make_shared<string>(4, 's') // "ssss"
shared_ptr<int> p1(new int{10}) // constructor

40.2 unique_ptr
- unique_ptr的具体实现和shared_ptr类似。但是它只允许一个指针指向对象。它不允许进行copy和赋值操作，实际上是删除了非右值版本的copy构造和赋值运算符操作。
- unique_ptr的初始化必须采用直接初始化的方式。即在初始化时，必须得到指向要管理内存的指针。
unique_ptr<int> p(new int{5});

- unique_ptr可以move。把管理的对象交个另一个unique_ptr,调用的是另一个unique_ptr的右值构造。
unique_ptr<int> p(new int{5});
unique_ptr<int> p1(std::move(p));

- 构造unique_ptr时，可以提供一个删除器，用来删除要管理的对象。删除器也可以是std::default_delete类型，这样使用的默认的删除器。
struct Foo { // object to manage
    Foo() { std::cout << "Foo ctor\n"; }
    Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
    Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
    ~Foo() { std::cout << "~Foo dtor\n"; }
};

struct D { // deleter
    D() {};
    D(const D&) { std::cout << "D copy ctor\n"; }
    D(D&) { std::cout << "D non-const copy ctor\n";}
    D(D&&) { std::cout << "D move ctor \n"; }
    void operator()(Foo* p) const {
        std::cout << "D is deleting a Foo\n";
        delete p;
    };
};

int main(){
    D d;
    {  // deleter type is not a reference
       std::unique_ptr<Foo, D> up3(new Foo, d); // deleter copied
    }
}

/**
Foo ctor
D copy ctor
D is deleting a Foo
~Foo dtor
*/

40.3 weak_ptr
- weak_ptr是一种不控制所指对象生存期的智能指针，它指向一个shared_ptr管理的对象。将一个weak_ptr绑定到shared_ptr不会改变shared_ptr的引用计数。一旦最后一个指向对象的shared_ptr被销毁，指向的对象就会被释放。即使有weak_ptr指向对象，对象还是会被释放。
- weak_ptr的构造函数：(Y 应该可以隐式转化为T)
// weak_ptr<T>::weak_ptr
constexpr weak_ptr() noexcept;  // (1)(since C++11)

weak_ptr( const weak_ptr& r ) noexcept;  // (2)(since C++11)
template< class Y >
weak_ptr( const weak_ptr<Y>& r ) noexcept;  //  (2)(since C++11)
template< class Y >
weak_ptr( const std::shared_ptr<Y>& r ) noexcept;  // (2)(since C++11)

weak_ptr( weak_ptr&& r ) noexcept;  // (3)(since C++11)
template< class Y >
weak_ptr( weak_ptr<Y>&& r ) noexcept;

- 部分成员函数
  - use_count()可以得到共享对象的shared_ptr的对象。
  - expired()， 返回值等价于use_count() == 0。
  - lock(),如果expired()返回true,则返回一个空的shared_ptr， 否则返回一个指向对象的shared_ptr.
- 在使用weak_ptr得到对象时，需要用lock来获取，然后判断是否为空，非空的话则可以直接使用lock返回的shared_ptr.
41. string
- C++11新增了数值转换，添加了多个转换函数。
to_string(val); // val type cant be any arithmetic operation type

/**
 * s is string. 
 * p: the size_t pointer for save the index of the first nonnumeric char in s.
 * b: the base, default is 10.
*/
stoi(s, p, b);   
stol(s, p, b);
stoul(s, p, b);
stoll(s, p, b);
stoull(s, p, b);
// float, double, long double.
stof(s, p)
stod(s, p)
stold(s, p)

- C++11新增了文件流对象使用string作为参数构造，之前只能使用C字符串来构造.
string ifile = "/home/wangcai/test.text";
ifstream in(ifile);

42. template 
42.1 template function default template argument
- C++11新增加了为函数提供默认实参，之前的C++只允许为类提供默认模板实参。
template<typename T, typename F = less<T> >
int compare(const T& v1, const T& v2, F f=F()){
    if (f(v1, v2)) return -1;
    if (f(v2, v1)) return 1;
    return 0;
}

42.2 type alias
- C++11新标准允许我们为类模板定义一个类别名
template<typename T> using twin = pair<T, T>;
twin<double> area;

42.3 type parameter as friend
- C++11新标准允许我们将模板类型参数声明为友元
template<typename Type>
class Bar {
friend Type;
}

这样就可以在Type类型内来使用Bar内的所有成员函数和成员变量。
#include <iostream>
using namespace std;

template <typename T>
class Foo {
friend T;

public:
    Foo() = default;

private:
    void print() { cout << "test...." << endl; }
};

class A {
public:
    A() = default;
    void func() { foo_a.print(); }
private:
    Foo<A> foo_a;
};

int main() {
    A a;
    a.func();   // print "test...." 
}

42.4 variadic 
- C++11新增了可变参数模板。可以定义一个可变参数的模板类或模板函数。
- 可变数目的参数被称为参数包。存在两个参数包，一个是模板参数包，表示零个或多个模板参数；一个是函数参数包，表示零个或多个函数参数。用省略号来指出一个模板参数或函数参数表示一个包。使用class...或typename...指出接下来的参数表示零个或多个类型的列表。
#include <iostream>
using namespace std;

template<typename T>
void print(const T& t){ cout << t << endl << "end" <<endl;}  // mark 1

void print() {cout << "end" << endl;}  // mark 2, 

template <typename T, typename... Args>
void print(const T& t, const Args&... rest){
    cout << t << " ";
    print(rest...);
}

int main() {
    print(1, 1.1, "wangcai");
    return 0;
}
/* output:(mark1 or mark2, or both exist)
1 1.1 wangcai
end
*/

42.5 variadics and forwarding
- 可变参数与转发。可以使用forward达到将实参不变的传递给其他函数。
 template<typename.. Args>
  void func(Args&&... args){
     func1(std::forward<Args>(args)...);
  }

43. trailing return type
- C++11新增了尾置返回类型。任何函数的定义都可以使用尾置返回类型。不过最好是比较复杂的返回类型，使用尾置返回。尾置返回类型可以是decltype推导出的类型。
auto func(int i) -> int (*) [10];
auto func1(int i) -> decltype(func());

- 一些情况必须使用尾置返回类型
  - 尾置返回类型是decltype推导出来，但是decltype的实参在之前没定义，而是定义在了函数形参中
  template<typename It>
  auto fcn(It beg, It end) -> decltype(*beg){
    return *beg;
  }

  - lambda表达式的返回类型必须是尾置类型
  auto f = [](int i) -> int {}

44. tuple
- C++11新增了tuple类型，tuple类似有pair，不同之处在于tuple打破了只能有两个元素的限制，tuple可以有任意数量的成员。和pair类似，成员的数据类型不必一致。
using Type = tuple<int, double, string, list<int>>;
Type tt{1, 1.1, "wangcai", {1,2,3,4} };  

- 由于tuple的形参为tuple内存的各个元素的构造函数是explict的。且未提供初始化列表版的构造函数。说以上例中如果这么写Type tt = {1, 1.1, "wangcai", {1,2,3,4} }，将会报错。  
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
/**output:
foo contains: 100 y
3 12 16
*/

  - 可以使用构造函数、make_tuple来创建对象
  - 使用get<n>来获取tuple中的第n个元素。返回这可以使左值引用、右值引用或const左值引用。具体何种类型和形参保持一致。
// the definition of std::get
template <size_t I, class... Types>
typename tuple_element< I, tuple<Types...> >::type& get(tuple<Types...>& tpl) noexcept;

template <size_t I, class... Types>
typename tuple_element< I, tuple<Types...> >::type&& get(tuple<Types...>&& tpl) noexcept;
template <size_t I, class... Types>
typename tuple_element< I, tuple<Types...> >::type const& get(const tuple<Types...>& tpl) noexcept;

  - std::tie 创建一个tuple，此tuple内存的是各个源元素的左值引用。所以把foo的值赋给tie的对象，可以达到拆解获取tuple中各个元素的目的。
  - std::tuple_size可以在编译时获取tuple的大小。
45. type alias declaration
- C++11 新规定了一种新的方法用来定义类型的别名，using方法。
class Dog{};
using T = Dog; // typedef Dog T

46. union member of class type
- 在早期版本中，union中不能含有定义了构造函数或者copy控制成员（copy构造函数和赋值运算符函数）的类类型。C++11新标准则取消了这一限制。不过如果union的成员类型定义了自己的构造函数和copy构造函数的话，改union的用法要比只含有内置类型成员的union复杂的多。
  - 当union包含的是内置类型的成员时，我们可以使用普通的赋值语句来改变union保存的值。而对于含有特殊类型成员的union，则我们必须调用其构造函数或copy构造函数才能达到改变union保存值的目的。反之当我们将改类型成员的值改为其他值时，必须先调用改类型成员的析构函数。
  - 当union包含的是内置类型成员时，编译器将按照成员的次序依次合成默认构造函数和copy控制成员。但是如果union含有类类型的成员，且改类型自定义了默认构造函数或copy控制成员，则编译器将为union合成对应的版本并将其声明为delete。原因很简单，既然成员中含有类类型，且此类类型自定了默认构造函数和copy控制成员。那么union在编译器生成默认版本时，由于成员类没有使用编译器生成的默认构造函数和copy控制成员，所以编译器要求用户自己定义union的对应版本，因为如果用户自己不定义的话，编译器会自己生成一个版本，但是此版本不能保证程序正确（因为是用户自定义的和编译器默认生成的可能性很低），所以索性声明为delete，以达到在用为未为union自定义默认构造函数和copy控制成员时，禁止使用这些默认构造或copy控制成员。
47. unordered containers
- C++ 11新增了4种无序关联容器。之前的C++的关联容器一共有四种，且都是有序的，分别是map、set、multimap、multiset。与之对应，新增了四个无序关联容器unordered_map、unordered_set、unordered_multimap、unordered_multiset。这些容器不是使用比较运算符来组织元素，而是使用哈希函数和关键字类型的==符号来管理元素。其底层个哈希表。
- 对于某些不需要知道元素顺序的场景，使用无序关联容器是有必要的。因为在某些情况下维护有序的代价是非常高昂的，因此使用无序容器是很有必要的。
48. virtual function
48.1 final
- C++11新标准允许派生类的的虚函数声明为final，其位置和override一样。如此做，则派生类的派生类不能重写此虚函数。
48.2 override
- C++11新标准允许派生类显式地注明它使用某个成员函数覆盖了它继承的虚函数。具体做法就是在形参后面、或者const成员函数的const关键字后面、或者在引用成员函数的引用限定符后面加一个override。
- 其目的是，我们想要子类重写基类的某个某个虚函数。但是一不小心把形参写作了，导致重写失败。相当于子类又定义了一个函数，此为子类函数的重载版本，但编译能够通过。那么加上override关键字后，编译器就会去检查是否是重写，如果不是，则编译报错。
END
- 根据C++ primer一书把C++11的新增特性都总结了出来。可以参看cppreference来研究具体细节。

##  =default，delete

- =default 如果我们没有定义构造函数，C++编译器会自动为我们创建一个默认构造函数。但是如果我们定义了一个构造函数，那么编译器就不会为我们再生成默认构造函数，=default可以达到这两者都存在。
- =delete. 如果用户没有定义类的4大函数（构造函数、copy构造函数、赋值运算符、析构函数）的话，C++编译器会为我们提供默认版本。但是如果我们不想要其中的某一个函数，那么我们可以用=delete来指定编译器不比为我们生成此默认函数。如从，则对应的函数不能使用。如果基类使用了delete，则子类对应的函数编译器不会去在生成，及子类和基类保持一致。

```C++
Class A{
public:
    A() = default; //提供了默认的构造函数
    A(int a):a(a){};
    A& operator=(const A&) = delete; //删除赋值构造。
private:
    int a;
};

int main(){
    A a(1);
    A aa(2);
    aa = a; //编译报错，赋值运算符被删除了。
}
```

2. ## allocator.construct 可以转到 任何构造函数

- allocator分配的内存是未构造的，我们需要在分配的内存中自己来构造对象。在新标准中其成员函数construct可以接受一个指针和零个或多个额外的参数，在指针指向的位置构造一个对象，额外参数则用来初始化构造的对象。这些额外的参数必须是与构造对象的类型相匹配的合法的初始化器。（deprecated in c++17, removed in c++20）
- 在早期版本construct只接受2个参数，一个是指针，一个是元素对象值，使用的是copy构造函数（推测），问不是任意参数的构造函数来构造对象。不过使用clang13，-std=c++03或-std=c++98，使用新增特性依旧可以编译通过。

```C++
#include<iostream>
#include<memory>

using namespace std;

class A {
public:
    A()=default;
    A(int a, int b) : a(a), b(b) {};
    void print() const { cout << "a: " << a << " b: " << b <<endl; };

private:
    int a;
    int b;
};



int main(){

    A* p = (A*) malloc(sizeof(A));

    allocator<A>().construct(p, 2, 4);

    p->print();   // a: 2  b: 4 

    allocator<A>().destroy(p); // destroy memory pointed by p

}
```

1. ## array container

- array 固定大小数组，支持
- 随机访问，不支持增加和删除元素

1. ## auto

- auto是让编译器帮我们分析表达式的类型。值得注意的是，auto 是让编译器通过初始值来推算变量的类型。显然auto定义的变量必须有初始值。

```C%2B%2B
int a = 0;

int b = 1;

auto c = a + b;
```

- 使用auto也可以在一条语句中声明多个变量。因为一条语句只有一个基本类型，所以所有变量初始值的类型要保持一致。

```C%2B%2B
auto i = 0, *p = &i;  // correct

auto sz = 0, pi = 3.14 // error, the types of sz and pi are different. 
```

- 编译器推断出来的auto类型有些时候和初始值类型并不完全一样，编译器会适当地改变结果类型使其更符合初始化规则。

1） 使用引用一般是使用引用的对象，当引用的对象作为auto关键字声明的对象初始值时，真正初始化的是引用的对象。所以引用类型会被忽略掉。

```C%2B%2B
int i = 0, &r = i;  // r -> int&

auto a = r;  // a -> int,   
```

\2) auto一般会忽略掉顶层const，而保留底层const。（顶层const是指指针是一个常量，底层const指指针指向的一个对象是const）

```C%2B%2B
int i = 2;

const int ci = 3;

const int ci = i, &cr = ci;

auto b = ci;  // b -> int

auto c = cr;  // c-> int

auto d = &i;  // d -> int * 

auto e = &ci; // e -> const int *

*d = 5; 

*e = 5; //error
```

1. ## begin and end function

- 尽管能计算得到尾后指针，但这种用法极易出错。为了让指针的使用更简单、更安全， C++11新标准引入了两个名为begin和end的函数，定义在iterator头文件中。这两个函数与容器中的begin和end的成员函数功能类似。不过begin和end的形参可以是数组。

```C%2B%2B
int ia[] = {0, 1, 2, 3};

int* beg = begin(ia);

int* last= end(ia);
```

1. ## bind function

- bind函数可以为函数绑定部分参数，并返回一个绑定参数后的新的函数。可以将其看为一个函数适配器。其一般形式为：

```C%2B%2B
auto newCallable = bind(callable, arg_list);
```

假设一下场景，我们想使用find_if找到容器中一个小于x的数。

定义一个比较比较大小的函数 

```C%2B%2B
bool isLess(T a, T b){return a < b;} 
```

但此函数不能作为find_if的参数来使用，因为find_if需要一个一元谓词。那么bind的作用就出现了，可以用bind函数来创建出一个一元谓词。

```C%2B%2B
auto isLessThanX = bind(isLess, _1, x)；
```

如此，就可以使用find_if来找到小于x的第一个数了。

```C%2B%2B
#include <iostream>

#include <vector>

#include <functional>



using namespace std;



template <typename T>

bool isLess(T a, T b) {

    return a < b;

}



int main() {

    int x = 5;

    vector<int> vec{13,12,4,5,2,10};

    auto isLessThanX = bind(isLess<int>, placeholders::_1, x);

    auto it = find_if(vec.begin(), vec.end(), isLessThanX);

    if (it != vec.end()) {

        cout << "target:" << *it << endl;

    }

    else {

        cout << "the number less than " << x << "does not exit" << endl;

    }

    return 0;

}
```



1. ## bitset enhancements

- 增加的bitset的内置函数all.当所有位置置位时all()返回true。
- 增加[to_ullong](https://en.cppreference.com/w/cpp/utility/bitset/to_ullong), 把bitset转为unsigned long long的数值

1. ## constexpr

### 8.1 constexpr变量

- 在一个复杂系统中，很难确定一个初始值是否是常量表达式。虽然可以定义const类型，但是有些时候const声明的并非常量表达式；(编译时而不是运行时能够确定其值的都是常量表达式)

```C%2B%2B
int a = 1;  // variable，not constant expression

const int b = 2;  // constant expression

const int c = getSize();  // not constant expression
```

为了解决以上问题，提出了constexpr变量，让编译器来验证变量的值是否为常量表达式。

```C%2B%2B
constexpr int a = 1;  // constant expression

constexpr int b = a+1;  // const expression

constexpr int c = func();  // error if func is not a constexpr func
```

- 注意：如果在constexpr声明中定义的是一个指针，那么限定符constexpr仅对指针有效，对指针指向的的对象无关。

```C%2B%2B
const int *p = nullptr;  // p is a pointer point to const int variable.

constexpr int *p = nullptr;  // p is a const pointer point to int variable
```

### 8.2 constexpr 函数

- constexpr函数是指能用于常量表达式的函数。要求为：函数的返回值及所有形参的类型都必须是字面值类型，而且函数体中有且只有一条return语句（既然一切都是确定的，在逻辑上自然也就只有一条return语句）

```C%2B%2B
constexpr int func() { return 10; }

const int a = func();
```

- 编译器会把constexpr表示的函数替换成函数结果。为了能在编译过程中随时展开，constexpr函数被隐式地定义为内联函数，所以通常放在头文件中。

### 8.3 constexpr类

数据成员都是字面值类型的聚合类是字面值常量类。（聚合类是一种没有用户定义的[构造函数](https://so.csdn.net/so/search?q=构造函数&spm=1001.2101.3001.7020)，没有私有（private）和保护（protected）非静态数据成员，没有基类，没有虚函数。这样的类可以由初始化列表初始化）。如果一个类不是聚合类，但他复合下面的要求，则它也是一个字面值常量类：

- 数据成员都必须是字面值类型
- 类必须有一个constexpr构造函数。
- 如果一个数据成员含有类内初始值，则其初始值必须为常量表达式。或者，如果成员属于某种类型，则初始值必须使用成员自己的constexpr构造函数。
- 类必须使用析构函数的默认定义，改成员负责销毁类的对象。

1. ## decltype

- 有些时候会遇到这种情况：希望从表达式的类型推断出要定义的变量的类型，但是不想用该表达式的值初始化变量。为了满足这一要求，c++11引入了第二种类型说明符decltype，编译器会分析表达式并得到它的类型，却不实际计算他的值。

```C%2B%2B
decltype（f()）sum = x;  // the type of sum is same as the return type of f()
```

编译器并没有调用f()，而是使用当调用发生时，f()的返回值类型作为sum的类型。

- decltype在处理顶层const时和auto不同。如果decltype使用的是一个变量，则decltype返回该变量的类型（包括顶层const和引用在内）。

```C%2B%2B
const int ci = 0, &cj = ci;

decltype(ci) x = 0;  // x -> const int

decltype(cj) y = x;  // y -> const int&

decltype(cj> z;  // z -> const int&. error, because of no initialization.
```

- 值得注意的是引用从来都是作为一个所指对象的同义词出现，只有在decltype处是一个例外。

- decltype使用的表达式不是一个变量，则decltype返回表达式结果对应的类型。
  - 注意，当返回的结果可以作为左值时，decltype返回的是一个引用。

```C%2B%2B
int i = 42, *p = &i, &r = i;

decltype(r+0) b; // b -> int

decltype(*p) c; // c -> int&, error with not initialization
```

- decltype的结果类型与表达式形式密切相关。有一种形式需要特别注意，如果变量名上加上小括号，则得到的是引用，如果不加小括号则得到的结果是变量的类型。即decltype((variable))的结果永远是引用，decltype(variable)的结果为variable本身的类型，除非variable是引用时，其结果才是引用。其原因是加上一层或多层小括号，编译器就会认为他是一个表达式。变量是一个可以作为赋值语句左值的特殊表达式。所以decltype就会得到引用类型和上面的*p类似，*p就是一个表达式。
- 对于函数返回类型的推导，我们可以使用尾置返回类型，尾置返回类型可以使用decltype推导。

```C%2B%2B
int func(int* a, int b) { return *a + b; }

auto f1(int) -> int (*) (int*, int);  // 1.

auto f1(int) -> decltype(func);  // same as 1.
```

1. ## delegating constructor

- C++11扩展了委托构造函数初始值的功能，使得我们可以定义所谓的委托构造函数。一个委托构造函数可以使用它所属类的其他构造函数执行它自己的初始化过程。

```C%2B%2B
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
```

- 注意使用了委托构造函数后，“:”后不能再用初始化列表初始化成员变量。因为构造过程已经委托给了其他构造函数。否则编译会报错。

- 执行顺序：先执行委托构造函数初始化列表初始化，然后是委托函数的函数体，至此委托执行的部分已经完毕，然后开始执行委托者自己的函数体。

1. ##  division rounding

- 对于整数除法取整，C++早期版本允许结果为负值的上向上或向下取整。C++11新标准规定商一律向0取证，（即直接去掉小数部分）

1. ## enumeration

### 12.1 scoped enumeration

- C++11新引入了限定作用域的枚举类型（scoped enumeration)。至此，枚举类型包含两种：限定作用域的和非限定作用域的。
- 定义限定作用域的枚举类型的一般形式是：关键字enum class（也可等价的使用enum struct)，随后枚举类型的名字以及用花括号括起来以逗号分隔的枚举成员列表。

```C%2B%2B
enum class OpenModes { INPUT, OUTPUT, APPEND };
```

- 定义非限定的枚举类型，只需吧class删除掉就行。

```C%2B%2B
enum OpenModes { INPUT, OUTPUT, APPEND };
```

- 非限定枚举和限定枚举两者的区别：作用域是否被限定

```C%2B%2B
enum Color { RED, YELLOW, GREEN }  // not scoped enumeration

enum StopLight { RED, YELLOW, GREEN}  // error, duplicated the members of enumeraton

enum class Peppers { RED, YELLOW, GREEN }  // right, scoped enumeration



Color eyes = GREEN;  // right

Peppers p = GREEN;  // error



 Color hair = Color::RED;  // right, explicitly

 Peppers p2 = Peppers::RED;  // right 
```

### 12.2 forward declaration

- 在C++11中我们可以在enum的名字后面加上冒号以及我们想在改enum中使用的类型来指定enum是用哪种整数类型表示的。

```C%2B%2B
enum Color : unsigned char {

    RED=8, YELLOW=20, GREEN=30

}
```

### 12.3 controlling representation

- C++11新增可以提前声明枚举类型。enum 的前置声明（无论显示地还是隐式地）必须指定其成员大小。限定类型的枚举隐式的使用int作为其成员类型。

```C%2B%2B
enum class Values;  // default type is int for the scoped enumeration's members

enum Values;  // error, IntValues has been declared as the scoped enumeration

enum class Color : long;  // error, type different.
```

1. ## explicit conversion operator

- C++11引入了显示类型转化运算符。可解决由于隐式类型转换导致的意想不到的错误。(隐式转化要谨慎定义，虽然提供了方便，但却带来了隐患)。在早期的C++版本中，如果想定一个bool类型转化，则是隐式的。那么会出现下面的问题。假设cin内定义了一个bool的隐式类型转化。

```C%2B%2B
int i = 42；

cin << i;  // should be illegal.
```

如果cin想作为istream，输入流不存在运算符 << ,本应该编译报错。但是cin会隐式的转化为bool，而bool会提升为int，从而变成了0<<i;与预期结果相差很大。那么早期的C++是使用operator void*()来避免这中问题，从而达到与operator bool()相似的效果。而在C++11中，由于出现了explicit，直接定义为explicit operator bool()就行了。所以放弃了早期版本中istream内的operator void*()。

- 用法：

```C%2B%2B
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

    a + 3;  // error, right if no explicit declaration above.

    int c = static_cast<int>(a) + 3;  // correct 

}
```

当类型转化运算符是显式的时候，必须使用显式的强制类型转化才可以。不过存在一个意外：如果表达式被用作条件，则编译器会自动使用显式类型转化，话句话说，当表达式出现在下列位置时，显示类型转化将会被隐式的执行：

- if、while及do语句的条件部分
- for语句头的条件表达式
- 逻辑非运算符（!）、逻辑或运算符（||）、逻辑与运算符（&&）的运算对象
- 条件运算符的条件表达式

向bool类型的转化一般用在条件部分，因此operator bool()一般定义为explicit。

1. ## explicit instantiation

- 在早期C++版本，模板在使用时进行实例化，那么就会出现一个问题，模板在多处使用，就会实例化好多次，这无形中增加了编译的时间以及生成的二进制文件的大小。为解决此问题，C++11新增了extern template功能，即extern template *declaration。*那么再结合模板的显示实例化就可以解决上面出现的问题。

```C%2B%2B
// dog.hpp

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
// dog.cpp

#include "dog.hpp"



template<typename T>

T Dog<T>::getHeight(){

    return height;

}



template class Dog<float>;  // explicit instantiation
// main.cpp

#include <iostream>

#include "dog.hpp"

using namespace std;



extern template class Dog<float>; // means Dog<float> definition in other file.

int main() {

    Dog<float> dog("wangcai", 0.72);  // not instantiation Dog with float, because of extern declaration above.

    cout << dog.getHeight() << endl;

}
```

- 补充一个模板的小细节，模板的声明与定义一般放在一起，放在头文件中。因为模板的编译是在实例化时，产生对应的实例化版本，如果声明放在头文件，定义放在cpp文件，那么在main.cpp中由于只include了头文件，从而会找不到定义，编译会报错。但是我们也可以在定义的cpp文件中进行显示的声明，来达到在编译cpp文件时，实例化对应的版本。由此就变成了函数或变量在其他文件中定义的情况了。

1. ## final class

- C++11新增了一种防止继承发生的方法，即在类名后价格final关键字。

1. ## format control for floating-point

- C++11新增了hexfloat和defaultfloat。在流中控制字符串格式时，hexfloat操纵符表示浮点数使用十六进制格式，defaultfloat操纵符则会把流恢复到默认状态。

```C%2B%2B
cout << "default format: " << 100 * sqrt(2) << '\n'

     << "scientific: " << scientific << 100 * sqrt(2) << '\n'

     << "fixed decimal: " << fixed << 100 * sqrt(2) << '\n'

     << "hexfloat: " << hexfloat << 100 * sqrt(2) << '\n'

     << "use defaults: " << defaultfloat << 100 * sqrt(2) << '\n';

/**

the output:

default format: 141.421

scientific: 1.414214e+02

fixed decimal: 141.421356

hexfloat: 0x1.1ad7bc01366b8p+7

use defaults: 141.421

*/
```

1. ## forward function

- C++11标准库添加了一个forward函数。其声明如下

```C%2B%2B
template <class T> T&& forward (typename remove_reference<T>::type& arg) noexcept;

template <class T> T&& forward (typename remove_reference<T>::type&& arg) noexcept;
```

注意观察此函数的特点，从简单到复杂，先抛去完美转发等概念。单看这一个函数。我们能得到如下信息

- 这是一个模板函数，模板参数为T
- 此函数的返回值类型为T&&
- 此函数重载了两个版本，一个是形参为左值，一个是形参为右值。

我们看下他的具体实现（clang libc++）

```C%2B%2B
template <class _Tp>

inline _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR

_Tp&&

forward(typename remove_reference<_Tp>::type& __t) _NOEXCEPT    // 1.

{

    return static_cast<_Tp&&>(__t);  

}



template <class _Tp>

inline _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR

_Tp&&

forward(typename remove_reference<_Tp>::type&& __t) _NOEXCEPT    // 2.

{

    static_assert(!is_lvalue_reference<_Tp>::value,

                  "can not forward an rvalue as an lvalue");

    return static_cast<_Tp&&>(__t);  

}
```

可以看出forward函数只是个类型转化函数而已。先看第一个函数，此函数把左值引用转化为了_Tp&&的类型。由于引用会保留底层const，所以参数的const属性会保存在_Tp中，

```C%2B%2B
int main(){

    int a = 10;

    forward<int>(a);   // return type: int&& -> int&&

    forward<int&>(a);   // return type: int& && -> int&

    forward<int&&>(a);  // return type: int&& && -> int&& 

}
```

(引用折叠，只有一种情况导致最终的折叠类型是T&&, 那就是T&& &&. 其余情况全是int&

- T & 、 T& &、 T& && 、T&& &  --> T&
- T&& &&  --> T&&

)

再看第二个函数：他是个右值引用版本不过里面多个static_assert（可以在编译期来检查错误）。这个static_assert语句，解释起来就是，调用右值引用版本的forward时，forward的模板参数不能实例化为左值引用，也就是返回值不能是左值。转为一句话即为：无法使用forward函数把类型由右值引用转化为左值引用。

```C%2B%2B
int main(){

    int a = 10;

    std::forward<int&>(std::move(a));  // compile-time error 

}  
```

总结起来就是：

- 左值可以转化为左值或右值
- 右值不能转化为左值。

那么这个forward函数有什么作用呢？

我们知道在模板函数推导参数的过程中，会出现丢失参数的部分类型信息（比如const信息以及左值引用信息）

```C%2B%2B
template<typename T>

void func(T a){

    cout << a <<endl;

}

int main(){

    int a = 10;

    func(a);   // T -> int

    const int a = 10;

    func(a)   // T -> int

    int& b = a;

    func(b)  // T -> int 

    func(std::move(b) // T -> int&& ， ok

}
```

为了解决这个问题，引用折叠便排上了用场。

```C%2B%2B
template<typename T>

void func(T&& a){

    cout << a <<endl;

}

int main(){

    int a = 10;

    func(a);   // T -> int&

    const int a = 10;

    func(a)   // T -> const int&

    int& b = a;

    func(b)  // T -> int&

    func(std::move(b)) // T -> int

}
```

forward是在利用引用折叠的基础上，实现其功能的。

假设两个函数f和g， 为了实现上面提到的，在模板推导中保留参数的const和左值引用信息。我们的f定义为右值引用的模板函数.然后f调用了g函数。

```C%2B%2B
template <typename T>

void f(T&& a){

    g(a);    // mark for compile-error

    g(std::forward<T>(a));  // right

}

void g(int&& c){

    cout << c << endl;

}

int main(){

    f(4); // T->int,  compile-time error on g(a). No g(int a) function.

}
```

由此可见forward实现了一种完美转发的机制。他完美在哪，就是在我们能把f函数的实参类型传递给f函数内部调用的其他函数，只要我们用forward处理下被调用的内部函数额实参。

1. ## forward_list container

- C++新增了一个容器类型forward_list用来表示单向链表。

1. ## function interface to callable objects

- C++11标准库提供了一个function的函数，std::function可以存储、赋值、调用可调用对象（函数、lambda表达式、bind表达式、函数对象、以及指向成员函数和成员数据的指针(成员函数的话，调用时第一个参数是类的对象其他参数为成员函数的参数)。

```C%2B%2B
#include <functional>

#include <iostream>

 

struct Foo {

    Foo(int num) : num_(num) {}

    void print_add(int i) const { std::cout << num_+i << '\n'; }

    int num_;

};

 

void print_num(int i)

{

    std::cout << i << '\n';

}

 

struct PrintNum {

    void operator()(int i) const

    {

        std::cout << i << '\n';

    }

};

 

int main()

{

    // store a free function

    std::function<void(int)> f_display = print_num;

    f_display(-9);

 

    // store a lambda

    std::function<void()> f_display_42 = []() { print_num(42); };

    f_display_42();

 

    // store the result of a call to std::bind

    std::function<void()> f_display_31337 = std::bind(print_num, 31337);

    f_display_31337();

 

    // store a call to a member function

    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;

    const Foo foo(314159);

    f_add_display(foo, 1);

    f_add_display(314159, 1);

 

    // store a call to a data member accessor

    std::function<int(Foo const&)> f_num = &Foo::num_;

    std::cout << "num_: " << f_num(foo) << '\n';

 

    // store a call to a member function and object

    using std::placeholders::_1;

    std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );

    f_add_display2(2);

 

    // store a call to a member function and object ptr

    std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );

    f_add_display3(3);

 

    // store a call to a function object

    std::function<void(int)> f_display_obj = PrintNum();

    f_display_obj(18);

 

    auto factorial = [](int n) {

        // store a lambda object to emulate "recursive lambda"; aware of extra overhead

        std::function<int(int)> fac = [&](int n){ return (n < 2) ? 1 : n*fac(n-1); };

        // note that "auto fac = [&](int n){...};" does not work in recursive calls

        return fac(n);

    };

    for (int i{5}; i != 8; ++i) { std::cout << i << "! = " << factorial(i) << ";  "; }

}
```

之所以引出function，是为了可以统一上述的调用对象到一个function类型，这样方便函数统一调用。

1. in-class initializer

- C++11新规定，可以为类内数据成员提供一个类内初始值，用于初始化成员变量，没有初始值的成员变量将被默认初始化。如此便可以对引用类型进行初始化。
- 关于类内初始值和初始化列表初始化的顺序问题，由下例可知，最终执行的是初始化列表初始化。其实类内初始化并没有执行，因为类内初始化和初始化列表初始化都是初始化动作，只能执行一个。

```C%2B%2B
#include <iostream>

#include <string>



using namespace std;



struct Dog {

    Dog() : name("shiyi"), height(0.4) {}



    string name = "wangcai";

    float height = 0.7;

};

int main() {

    Dog dog;

    cout << dog.name << endl;    // shiyi

    cout << dog.height << endl;  // 0.4

    return 0;

}
```

1. ## inherited constructor

- C++11 新标准中，派生类能够重用其基类定义的构造函数。

```C%2B%2B
struct B1 { B1(int, ...) {} };

struct B2 { B2(double)   {} };

 int get();

 struct D1 : B1{

    using B1::B1; // inherits B1(int, ...)

    int x;

    int y = get();

};

 void test()

{

    D1 d(2, 3, 4); // OK: B1 is initialized by calling B1(2, 3, 4),

                   // then d.x is default-initialized (no initialization is performed),

                   // then d.y is initialized by calling get()

 

    D1 e;          // Error: D1 has no default constructor

}

 struct D2 : B2{

    using B2::B2; // inherits B2(double)

    B1 b;

};

 

D2 f(1.0); // error: B1 has no default constructor
```

using语句只是告诉编译器，在此处生成子类的构造函数，对于基类的每个构造函数，子类都会生成一个对应的构造函数。换句话说，对于基类的每个都早函数，编译器都会在子类中生成一个形参列表完全相同的构造函数。但是如果是多继承，且基类的构造函数相同（既具有完全相同的形参列表），则会编译报错。

```C%2B%2B
struct C1{

    C1(int a) {cout << "C1" << endl;}

};

struct C2{ 

    C2(int a)  {cout << "C2" << endl;}

};

struct CC:C1,C2{

    using C1::C1;

    using C2::C2;

};

int main(){

    CC cc(2); // compiler-time error, more than one constructor.

}
```

1. ## initializer_list

- C++11提供了std::initializer_list<T>类型， 

  ```
  std::initializer_list<T>
  ```

   is a lightweight proxy object that provides access to an array of objects of type 

  ```
  const T
  ```

  . 

  - initializer_list的非空构造函数由编译器提供，会使用copy-initailize底层array<T, N>的值（数据类型根据实际情况会可能发生narrow）
  - initializer_list的copy操纵不会copy底层的数据结构。它只是提供了一种访问底层数组的方式。
  - initialzier_list提供了begin()和end()，所以可以使用range-for来遍历元素。

```C%2B%2B
initializer_list<int> l{1,2,3,4};
```

- 当函数行参的类型相同，但是数量不定时，可以用initializer_list作为形参。

```C%2B%2B
void func(initializer_list<int> l){

    for (auto i : l) {

        cout << i << endl;

    }

}

int main() {

    func({1,2,3})

    func({1,2})

}
```

- C++11类内提供了参数为initializer_list的构造函数。

1. ## inline namespace

- C++11引入了一种新的命名空间，叫做内联命名空间（inline namespace）。和普通的嵌套命名空间不同，内联命名空间中的名字可以被外层命名空间直接使用。也就是说我们无需在内联命名空间的名字前面加上表示该命名空间的前缀，通过外层命名空间的名字就可以直接访问到它。

```C%2B%2B
namespace M{

    inline namespace N{

        class Cls{};

    }

}

int main(){

    using namespace M;

    Cls cls;  // replace N::Cls.

}
```

- 内联命名空间inline必须出现在第一次定义的地方，后续再打开命名空间的时候可以写inline，也可以不写。

```C%2B%2B
namespace M{

    inline namespace N{}

    namespace N{           // the keyword "inline" is omitted

        class Cls{};

    }

}
```

1. ## lambda expression

- lambda表达式表示的是一个可调用的代码单元。我们可以将其理解为一个未命名的内联函数。一个lambda表达式具有如下形式：

[capture list] (parameter list) -> return type {function body}

其中parameter list 和 return type是可选的。但必须包含capture list和function body。

```C%2B%2B
auto f = []{return 42;}

auto f1 = [&] (int a) -> int { return a+1;}
```

- lambda表达式不能有默认参数，因此一个lambda表达式实参数目永远与形参数目相等。
- capture list可以让lambda表达式得到其所在函数中局部变量的值。包括值捕获、引用捕获、隐式捕获

```C%2B%2B
int v = 0;

int w = 1;

auto f =[v] () {return v;}  // value capture

auto f1 = [&v] () {return v;}  // reference capture



auto f2 = [=] () {return v + w;}  //implicit value capture

auto f2 = [&] () {return v + w;}  // implicit reference capture
```

- 可变lambda。默认情况下，对于一个值copy变量，lambda不会去改变其值。如果我们希望能改变一个值捕获变量的值，就必须在形参列表后添加mutable。对于一个引用捕获的变量是否可以修改，依赖于此引用指向的是一个const类型还是非const类型。

```C%2B%2B
void fcn() {

    int v = 42;

    auto f = [v]() mutable { return ++v; };

    cout << v << endl;  // 42

    v = 0;

    auto j = f();  

    cout << j << endl;  // 43;

}



void fcn1() {

    int v = 42;

    auto f = [&v]() { return ++v; };

    cout << v << endl;  // 42

    v = 0;

    auto j = f();  

    cout << j << endl;  // 1

}
```

1. ## list initializatoin

- C++11允许使用花括号括起来的初始值列表作为赋值语句的右侧运算对象。

- 可以使用初始化列表初始化容器。

- 初始化动态分配数组。

- 初始化动态对象。

- 初始化pair

```C%2B%2B
// initialization

int k = {3};



// initialze container

vector<int> vi = {1,2,3,4,5};

map<int, string> mp {{1,"one"}, {2, "two"}, {3, "three"}};

set<int> st {1, 2, 3}



//dynamic array

int*p = new int[5]{1,2,3,4,5};



//dynamic object

int *p = new int (5)  // older method

int *n = new int {5}  // new method. C++11

vector<int> * vv = new vector<int>{1,2,3,4,5} // new method. C++11



// pair

pair<int, float> p {1, 1.2};   // initialization

//the other methods of creating pair.

pair<int, float> p1(1,1.2);

pair<int, flaot> p2 = make_pair<int, float>(1,1.2);
```

- C++11允许使用花括号括起来的值的列表作为函数返回值。

```C%2B%2B
vector<int> func(){

    return {1,2,3};

}



pair<int, string> func1(){

    return {1, "one"};

}
```

1. ## long long

- C++11新增long long数据类型。另外，C++规定：一个int至少和一个short一样大。一个long至少和一个int一样大，一个long long至少和一个long一样大。

1. ## mem_fn function

- C++11，新增加了mem_fn函数，可以将成员函数作为调用对象。它基于成员函数生成了一个可调用对象。function也可以实现生成成员函数调用对象的功能，不过我们必须提供成员的调用形式。和function一样，mem_fn也定义在头文件functional中。与function不同的是，mem_fn来让编译器推断成员的类型，并且mem_fn只能用于成员函数生成调用对象。

```C%2B%2B
class Num{

public:

    int add(int a, int b){return a+b;} 

    //static int sub(int a, int b){return a+b;}  //static

};

int main(){

    Num n{};

    auto add = mem_fn(&Num::add);

    //auto sub = mem_fn(&Num::sub);  // error, can't be static

    add(n,1,2);

}
```

1. ## move function

- C++11新标准库添加了一个move函数，用来将一个左值引用类型转化为右值引用。

- move的返回类型可以表述为static_cast<typename [std::remove_reference](http://en.cppreference.com/w/cpp/types/remove_reference)<T>::type&&>(t)

```C%2B%2B
int a = 10;

int &&c = std::move(a);
```

- move调用告诉编译器：“我们有一个左值，但我们希望像一个右值一样处理它”。因此我们必须认识到调用move就意味着承诺：除了对a的赋值和销毁它外，我们将不在使用它。在调用完move后，我们不能对移动后的源对象的值做任何假设。

- 其实move很简单，本质上就是转化左值类型为一个右值类型。别的什么都没做。那么为什么说move后的对象不要再进行除了赋值和销毁外的其他操作呢。其实是可以的，但是不推荐，因为有时会报错。这是因为我们把move返回的右值引用类型的对象，作为形参传给其他函数，其他函数就会按照右值引用约定俗成的条例来执行，那就是这个右值引用的值，其他人已经不在使用了，那么为了效率，我就可以对它内部用指针执行的内存块，通过浅copy获得，并把原来的指针设为nullptr。标准库的很多右值引用版本的构造函数的具体实现就是这么处理的。

```C%2B%2B
#include <iostream>

#include <utility>

#include <string>

#include <list>

#include<vector>





using namespace std;



int main1(){

    int a = 10;

    int && c = move(a);

    cout << a <<endl;  // 10

    a = 2;

    cout << c <<endl;   // 2

    int* p = new int (3);

    move(p);

    cout << *p <<endl;  // 3

    list<int> l {1,2,3,4};

    move(l);

    cout << l.front() << endl;  // 1

    string s{"wangcai"};

    move(s);

    cout << s<<endl;  // wangcai

    return 0;

}



int main()

{

    std::string str = "Salut";

    std::vector<std::string> v;

 

    //std::cout << "After copy, str is " << str << '\n';

    // uses the push_back(const T&) overload, which means 

    // we'll incur the cost of copying str

    v.push_back(str);

    std::cout << "After copy, str is " << str << '\n';  // "Sault"

 

    // uses the rvalue reference push_back(T&&) overload, 

    // which means no strings will be copied; instead, the contents

    // of str will be moved into the vector.  This is less

    // expensive, but also means str might now be empty.

    v.push_back(std::move(str));

    std::cout << "After move, str is " << str << '\n';   // ""

 

    std::cout << "The contents of the vector are { " << v[0]   // "Sault"

              << ", " << v[1] << " }\n";                       // "Sault"

}
```

1. ## move constructor

- C++11新增加了移动构造函数和移动赋值运算符 。

```C%2B%2B
vector( vector&& other )；

vector& operator=( vector&& other )；
```

- 在移动构造函数中，要避免使用深copy。要严格按照移动语义约定俗称的规则来执行，进行浅copy。

1. ## move iterator

- C++11添加了一种新的迭代器，移动迭代器。用于将就内存中的元素快速copy到新内存中。一般来说一个迭代器返回的是一个左值引用，但是移动迭代器返回的是一个右值引用。我们可以通过使用标准库的make_move_iterator将一个普通迭代器转化为一个移动迭代器。

```C%2B%2B
#include <iostream>

#include <iomanip>

#include <list>

#include <vector>

#include <string>

#include <iterator>

 auto print = [](auto const rem, auto const& seq) {

    std::cout << rem;

    for ( auto const& str : seq)

            std::cout << str << ' ';

    std::cout << '\n';

};

 int main()

{

    std::list<std::string> s{"one", "two", "three"};

 

    std::vector<std::string> v1(s.begin(), s.end()); // copy

 

    std::vector<std::string> v2(std::make_move_iterator(s.begin()),

                                std::make_move_iterator(s.end())); // move

 

    print("v1 now holds: ", v1);   // v1 now holds: "one" "two" "three" 

    print("v2 now holds: ", v2);  // v2 now holds: "one" "two" "three"

    print("original list now holds: ", s);  // original list now holds: "" "" ""

}
```

1. ## move-enabled this pointer

- 先看一个例子：

```C%2B%2B
int a = 1;

int b = 2;

a+b = 5; // error, rvalue can't be assigned



// but

string str1 = "111";

string str2 = "222";

str1 + str2 = "555";  // no error
```

上面例子中str1 + str2是一个右值，但是却可以对一个右值进行赋值操作。˙不太符和人们的想法。在旧标准中我们无法阻止人们使用这种赋值方式。那么为了兼容性，新标准库仍然允许对右值进行赋值。但是，我们可能希望自己的类中组织这种用法。在这种情况下，我们强制等号左侧的运算对象（即this之指向的对象）是一个左值。

- 指定this是左值的方法和和指定this指向的对象是const的方法类似，即在参数列表后面放一个引用限定符。

```C%2B%2B
#include <iostream>

using namespace std;



class Foo{

public:

    Foo() = default;

    Foo(int a) : a(a) {}

    Foo& operator=(const Foo& other) &&  {   // mark1

        if (this == &other){

            return *this;

        }

        a = other.a; 

        return *this;

    }



    Foo operator+(const Foo & other) { 

        Foo ret;

        ret.a = other.a + this->a;

        return ret;

    }

private:

    int a;

};



int main(){

    Foo foo1(2);

    Foo foo2(3);

    Foo foo3(3);

    foo1 + foo2 = foo3;  // mark2

}
```

- 经实验，mark1位置，写&&和不写&&都可以被调用，都表示this是个右值。当为&时，表示this是左值，此时mark2处，编译就会报错。
- 当const限定符和&同时存在使，const必须放在&的前面。

1. ## noexcept

### 32.1. noexcept说明符

- C++11新标准中，可以通过noexcept说明指定某个函数不会抛出异常。其形式是关键字noexcept紧跟在函数的参数列表后面，用于标识该函数不会抛出异常。
- 对于告诉编译器此函数不会抛出异常是大有裨益的，首先，知道函数不会抛出异常有利于简化调用该函数的代码；其次，有利于编译器执行某些特殊的优化操作，而这些优化操作可能并不适用于可能出错的代码。
- 对于一个函数来说，noexcept说明要么出现在该函数的所有声明语句和定义中，要么一次也不出现。该说明应该在函数的尾置返回类型之前。也可以在函数指针的声明和定义中指定noexcept。在typedef或类型别名中则不能出现noexcept。在成员函数中，noexcept说明符需要在跟在const及引用限定符之后，而在final、override或虚函数的=0之前。
- 读者需要清楚的一个事实，编译器并不会在编译时检查noexcept说明。实际上，如果在一个函数在说明了noexcept的同时又含有throw语句或者调用了可能抛出异常的其他函数。编译器将顺利编译通过，并不会因为违反这中说明情况而报错（不排除个别编译器会对这种用法提出警告）

```C%2B%2B
void f() noexcept {

    throw exception();

}
```

因此可能出现一种情况：尽管函数声明了它不会抛出异常，但实际上还是抛出了。一旦一个noexcept函数抛出了异常，程序就会调用terminate以确保遵守在运行时不抛出异常的承诺。上述过程是否执行站展开未做约定，因此noexcept可以用在两种情况下：一种是我们确认函数不会抛出异常，二是我们根本不知道如果处理异常。

- 指明某个函数不会抛出异常可以令该函数的调用者不必再考虑如果处理异常。无论是函数确实不抛出异常，还是程序终止，调用者都无须为此负责。

### 32.2 noexcept运算符

- noexcept是一个一元运算符。它的返回值是一个bool类型的右值常量表达式。用于表示给定的表达式是否会抛出异常。

```C%2B%2B
noexcept(func(i)); 
```

- 如果func(i)在运算中，抛出异常则返回false，否则返回true。

1. ## nullptr

- C++11新引入了nullptr字面值，来初始化一个指针为空指针。

1. ## random-number library

- C++11提供了用于生成随机数的random库，放在random头文件中。内容比较独立，在此省略，具体请见https://en.cppreference.com/w/cpp/header/random

1. ## range for statement

- C++11提供了范围for语句。

```C%2B%2B
vector<int> v{1,2,3,4,5};

for (auto& i : v) {

    cout << i << " ";

}
```

- 一个容器能否使用范围for遍历，取决于这个容器内是否实现了返回迭代器的成员函数begin()和end()。范围for等价于下面的传统形式：

```C%2B%2B
for(auto i = v.begin(); i != v.end(); ++i){}
```

1. ## regular expression-library

- C++11提供了正则表达式库，定义在regex头文件中。内容表独立，具体用法请见https://en.cppreference.com/w/cpp/header/regex。

1. ## rvalue reference

- C++11新增了右值引用类型。使用&&来获取右值引用。
- 虽然不能隐式的将一个左值引用转为右值引用，但可以使用static_cast显式地将一个左值引用转为右值引用。使用std::move可以达到同样的目的，我们推荐使用std::move。这样方便统一查找左值转右值的代码位置（通过搜索move即可）

```C%2B%2B
#include <iostream>

using namespace std;



struct A {

    A() = default;

    A(const A& oth) { cout << "left value reference" << endl; }

    A(const A&& oth) { cout << "right value reference" << endl; }

};



int main() {

    A a;

    A a1(static_cast<A&&>(a)); 

    A a2(move(a));

    A a3(a);

}

/**

right value reference

right value reference

left value reference

*/
```

1. ## sizeof data member

- C++11允许我们使用sizeof Foo::m来获取Foo类内成员m的大小，而不需去创建一个Foo的对象。这是因为sizeof运算符无需我们提供一个具体的对象，毕竟想获取一个成员的大小是无需真的获取该成员的。
- sizeof不会实际求运算对象的值。sizeof空指针的解引用是不会报错的，应为空指针的解引用并未真正的运行。不需要真的解引用也能知道其所指对象的类型。

1. ## sizeof... operator

- 当我们想知道一个包中有多少个元素时，可以使用sizeof...运算符。和sizeof类似，sizeof...也返回一个常量表达式

```C%2B%2B
#include <iostream>

using namespace std;



template <typename... Args>

void func(Args... args){

    cout << sizeof...(Args) << endl;  // the number of type of parameter 

    cout << sizeof...(args) << endl;  // the number of func parameter 

}



int main(){

    func(1, 1, 1);   // 3, 3

    func(1, 1.0, "1");  // 3, 3

}
```

1. ## smart pointer

- C++11新增了三个智能指针：share_ptr、unique_ptr、weak_ptr。智能指针的主要作用是为了使用裸指针指向动态申请申请内存，由于程序员的疏忽所带来的内存泄漏问题。

### 40.1 shared_ptr

- share_ptr，内部使用了引用计数的原理。构造时引用计数加1，析构时，应用计数减一，当计数为0时，真正的析构指向的底层数据，并释放指向的底层内存。
- 创建shared_ptr。可以使用make_shared函数来创建，此函数调用了shared_ptr所要指向对象的构造函数。其形参的组织和emplace函数类似。也可以直接调用构造函数来创建。

```C%2B%2B
shared_ptr<int> p = make_shared<int> (42); 

shared_ptr<string> pstr = make_shared<string>(4, 's') // "ssss"

shared_ptr<int> p1(new int{10}) // constructor
```

### 40.2 unique_ptr

- unique_ptr的具体实现和shared_ptr类似。但是它只允许一个指针指向对象。它不允许进行copy和赋值操作，实际上是删除了非右值版本的copy构造和赋值运算符操作。
- unique_ptr的初始化必须采用直接初始化的方式。即在初始化时，必须得到指向要管理内存的指针。

```C%2B%2B
unique_ptr<int> p(new int{5});
```

- unique_ptr可以move。把管理的对象交个另一个unique_ptr,调用的是另一个unique_ptr的右值构造。

```C%2B%2B
unique_ptr<int> p(new int{5});

unique_ptr<int> p1(std::move(p));
```

- 构造unique_ptr时，可以提供一个删除器，用来删除要管理的对象。删除器也可以是std::default_delete类型，这样使用的默认的删除器。

```C%2B%2B
struct Foo { // object to manage

    Foo() { std::cout << "Foo ctor\n"; }

    Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }

    Foo(Foo&&) { std::cout << "Foo move ctor\n"; }

    ~Foo() { std::cout << "~Foo dtor\n"; }

};



struct D { // deleter

    D() {};

    D(const D&) { std::cout << "D copy ctor\n"; }

    D(D&) { std::cout << "D non-const copy ctor\n";}

    D(D&&) { std::cout << "D move ctor \n"; }

    void operator()(Foo* p) const {

        std::cout << "D is deleting a Foo\n";

        delete p;

    };

};



int main(){

    D d;

    {  // deleter type is not a reference

       std::unique_ptr<Foo, D> up3(new Foo, d); // deleter copied

    }

}



/**

Foo ctor

D copy ctor

D is deleting a Foo

~Foo dtor

*/
```

### 40.3 weak_ptr

- weak_ptr是一种不控制所指对象生存期的智能指针，它指向一个shared_ptr管理的对象。将一个weak_ptr绑定到shared_ptr不会改变shared_ptr的引用计数。一旦最后一个指向对象的shared_ptr被销毁，指向的对象就会被释放。即使有weak_ptr指向对象，对象还是会被释放。
- weak_ptr的构造函数：(Y 应该可以隐式转化为T)

```C%2B%2B
// weak_ptr<T>::weak_ptr

constexpr weak_ptr() noexcept;  // (1)(since C++11)



weak_ptr( const weak_ptr& r ) noexcept;  // (2)(since C++11)

template< class Y >

weak_ptr( const weak_ptr<Y>& r ) noexcept;  //  (2)(since C++11)

template< class Y >

weak_ptr( const std::shared_ptr<Y>& r ) noexcept;  // (2)(since C++11)



weak_ptr( weak_ptr&& r ) noexcept;  // (3)(since C++11)

template< class Y >

weak_ptr( weak_ptr<Y>&& r ) noexcept;
```

- 部分成员函数
  - use_count()可以得到共享对象的shared_ptr的对象。
  - expired()， 返回值等价于use_count() == 0。
  - lock(),如果expired()返回true,则返回一个空的shared_ptr， 否则返回一个指向对象的shared_ptr.
- 在使用weak_ptr得到对象时，需要用lock来获取，然后判断是否为空，非空的话则可以直接使用lock返回的shared_ptr.

1. string

- C++11新增了数值转换，添加了多个转换函数。

```Kotlin
to_string(val); // val type cant be any arithmetic operation type



/**

 * s is string. 

 * p: the size_t pointer for save the index of the first nonnumeric char in s.

 * b: the base, default is 10.

*/

stoi(s, p, b);   

stol(s, p, b);

stoul(s, p, b);

stoll(s, p, b);

stoull(s, p, b);

// float, double, long double.

stof(s, p)

stod(s, p)

stold(s, p)
```

- C++11新增了文件流对象使用string作为参数构造，之前只能使用C字符串来构造.

```C%2B%2B
string ifile = "/home/wangcai/test.text";

ifstream in(ifile);
```

1. ## template 

### 42.1 template function default template argument

- C++11新增加了为函数提供默认实参，之前的C++只允许为类提供默认模板实参。

```C%2B%2B
template<typename T, typename F = less<T> >

int compare(const T& v1, const T& v2, F f=F()){

    if (f(v1, v2)) return -1;

    if (f(v2, v1)) return 1;

    return 0;

}
```

### 42.2 type alias

- C++11新标准允许我们为类模板定义一个类别名

```C%2B%2B
template<typename T> using twin = pair<T, T>;

twin<double> area;
```

### 42.3 type parameter as friend

- C++11新标准允许我们将模板类型参数声明为友元

```C%2B%2B
template<typename Type>

class Bar {

friend Type;

}
```

这样就可以在Type类型内来使用Bar内的所有成员函数和成员变量。

```C%2B%2B
#include <iostream>

using namespace std;



template <typename T>

class Foo {

friend T;



public:

    Foo() = default;



private:

    void print() { cout << "test...." << endl; }

};



class A {

public:

    A() = default;

    void func() { foo_a.print(); }

private:

    Foo<A> foo_a;

};



int main() {

    A a;

    a.func();   // print "test...." 

}
```

### 42.4 variadic 

- C++11新增了可变参数模板。可以定义一个可变参数的模板类或模板函数。
- 可变数目的参数被称为参数包。存在两个参数包，一个是模板参数包，表示零个或多个模板参数；一个是函数参数包，表示零个或多个函数参数。用省略号来指出一个模板参数或函数参数表示一个包。使用class...或typename...指出接下来的参数表示零个或多个类型的列表。

```C%2B%2B
#include <iostream>

using namespace std;



template<typename T>

void print(const T& t){ cout << t << endl << "end" <<endl;}  // mark 1



void print() {cout << "end" << endl;}  // mark 2, 



template <typename T, typename... Args>

void print(const T& t, const Args&... rest){

    cout << t << " ";

    print(rest...);

}



int main() {

    print(1, 1.1, "wangcai");

    return 0;

}

/* output:(mark1 or mark2, or both exist)

1 1.1 wangcai

end

*/
```

42.5 variadics and forwarding

- 可变参数与转发。可以使用forward达到将实参不变的传递给其他函数。

```C%2B%2B
 template<typename.. Args>

 void func(Args&&... args){

     func1(std::forward<Args>(args)...);

 }
```

1. ## trailing return type

- C++11新增了尾置返回类型。任何函数的定义都可以使用尾置返回类型。不过最好是比较复杂的返回类型，使用尾置返回。尾置返回类型可以是decltype推导出的类型。

```C%2B%2B
auto func(int i) -> int (*) [10];

auto func1(int i) -> decltype(func());
```

- 一些情况必须使用尾置返回类型
  - 尾置返回类型是decltype推导出来，但是decltype的实参在之前没定义，而是定义在了函数形参中

```C%2B%2B
template<typename It>

auto fcn(It beg, It end) -> decltype(*beg){

    return *beg;

}
```

- lambda表达式的返回类型必须是尾置类型

```C%2B%2B
auto f = [](int i) -> int {}
```

1. ## tuple

- C++11新增了tuple类型，tuple类似有pair，不同之处在于tuple打破了只能有两个元素的限制，tuple可以有任意数量的成员。和pair类似，成员的数据类型不必一致。

```C%2B%2B
using Type = tuple<int, double, string, list<int>>;

Type tt{1, 1.1, "wangcai", {1,2,3,4} };  
```

- 由于tuple的形参为tuple内存的各个元素的构造函数是explict的。且未提供初始化列表版的构造函数。说以上例中如果这么写Type tt = {1, 1.1, "wangcai", {1,2,3,4} }，将会报错。  

```Rust
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

/**output:

foo contains: 100 y

3 12 16

*/
```

- 可以使用构造函数、make_tuple来创建对象
- 使用get<n>来获取tuple中的第n个元素。返回这可以使左值引用、右值引用或const左值引用。具体何种类型和形参保持一致。

```C%2B%2B
// the definition of std::get

template <size_t I, class... Types>

typename tuple_element< I, tuple<Types...> >::type& get(tuple<Types...>& tpl) noexcept;



template <size_t I, class... Types>

typename tuple_element< I, tuple<Types...> >::type&& get(tuple<Types...>&& tpl) noexcept;

template <size_t I, class... Types>

typename tuple_element< I, tuple<Types...> >::type const& get(const tuple<Types...>& tpl) noexcept;
```

- std::tie 创建一个tuple，此tuple内存的是各个源元素的左值引用。所以把foo的值赋给tie的对象，可以达到拆解获取tuple中各个元素的目的。
- std::tuple_size可以在编译时获取tuple的大小。

1. ## type alias declaration

- C++11 新规定了一种新的方法用来定义类型的别名，using方法。

```C%2B%2B
class Dog{};

using T = Dog; // typedef Dog T
```

1. ## union member of class type

- 在早期版本中，union中不能含有定义了构造函数或者copy控制成员（copy构造函数和赋值运算符函数）的类类型。C++11新标准则取消了这一限制。不过如果union的成员类型定义了自己的构造函数和copy构造函数的话，改union的用法要比只含有内置类型成员的union复杂的多。
  - 当union包含的是内置类型的成员时，我们可以使用普通的赋值语句来改变union保存的值。而对于含有特殊类型成员的union，则我们必须调用其构造函数或copy构造函数才能达到改变union保存值的目的。反之当我们将改类型成员的值改为其他值时，必须先调用改类型成员的析构函数。
  - 当union包含的是内置类型成员时，编译器将按照成员的次序依次合成默认构造函数和copy控制成员。但是如果union含有类类型的成员，且改类型自定义了默认构造函数或copy控制成员，则编译器将为union合成对应的版本并将其声明为delete。原因很简单，既然成员中含有类类型，且此类类型自定了默认构造函数和copy控制成员。那么union在编译器生成默认版本时，由于成员类没有使用编译器生成的默认构造函数和copy控制成员，所以编译器要求用户自己定义union的对应版本，因为如果用户自己不定义的话，编译器会自己生成一个版本，但是此版本不能保证程序正确（因为是用户自定义的和编译器默认生成的可能性很低），所以索性声明为delete，以达到在用为未为union自定义默认构造函数和copy控制成员时，禁止使用这些默认构造或copy控制成员。

1. ## unordered containers

- C++ 11新增了4种无序关联容器。之前的C++的关联容器一共有四种，且都是有序的，分别是map、set、multimap、multiset。与之对应，新增了四个无序关联容器unordered_map、unordered_set、unordered_multimap、unordered_multiset。这些容器不是使用比较运算符来组织元素，而是使用哈希函数和关键字类型的==符号来管理元素。其底层个哈希表。
- 对于某些不需要知道元素顺序的场景，使用无序关联容器是有必要的。因为在某些情况下维护有序的代价是非常高昂的，因此使用无序容器是很有必要的。

1. ## virtual function

### 48.1 final

- C++11新标准允许派生类的的虚函数声明为final，其位置和override一样。如此做，则派生类的派生类不能重写此虚函数。

### 48.2 override

- C++11新标准允许派生类显式地注明它使用某个成员函数覆盖了它继承的虚函数。具体做法就是在形参后面、或者const成员函数的const关键字后面、或者在引用成员函数的引用限定符后面加一个override。
- 其目的是，我们想要子类重写基类的某个某个虚函数。但是一不小心把形参写作了，导致重写失败。相当于子类又定义了一个函数，此为子类函数的重载版本，但编译能够通过。那么加上override关键字后，编译器就会去检查是否是重写，如果不是，则编译报错。

## END

- 根据C++ primer一书把C++11的新增特性都总结了出来。可以参看cppreference来研究具体细节。





