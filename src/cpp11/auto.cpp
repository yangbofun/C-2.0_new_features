
int main(){
    int i = 2;
    const int ci = 3;
    const int ci = i, &cr = ci;
    auto b = ci;  // b -> int
    auto c = cr;  // c-> int
    auto d = &i;  // d -> int * 
    auto e = &ci; // e -> const int * 
    *d = 5;
    *e = 5; //error

}