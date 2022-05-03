class A{
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
    aa = a; //编译报错，不存在赋值构造
}
