#include <iostream>
#include <utility>
#include <string>
#include <list>
#include<vector>


using namespace std;

int main1(){
    int a = 10;
    int && c = move(a);
    cout << a <<endl;
    a = 2;
    cout << c <<endl;
    int* p = new int (3);
    move(p);
    cout << *p <<endl;
    list<int> l {1,2,3,4};
    move(l);
    cout << l.front() << endl;
    string s{"wangcai"};
    move(s);
    cout << s<<endl;
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
    std::cout << "After copy, str is " << str << '\n';
 
    // uses the rvalue reference push_back(T&&) overload, 
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is " << str << '\n';
 
    std::cout << "The contents of the vector are { " << v[0]
                                             << ", " << v[1] << " }\n";
}