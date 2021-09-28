#include <iostream>
#include <string>
using namespace std;

#define N 10

void fibo1(){
    int tab[N];
    for (int i=0; i<N; i++){
        if (i==0 || i==1){
            tab[i]=1;
        }
        else {
            tab[i]=tab[i-2]+tab[i-1];
        }
    }
    for (int i=0; i<N; i++){
        cout<<tab[i]<<endl;
    }
}


void fibo2(int n){
    int *t = new int[n];
    for (int i=0; i<n; i++){
        if (i==0 || i==1){
            t[i]=1;
        }
        else {
            t[i]=t[i-2]+t[i-1];
        }
    }
    for (int i=0; i<n; i++){
        cout<<t[i]<<endl;
    }
    delete []t;
}

class Fibo1{
    int*tab;
    int n;
    public:
        void init(int _n);
        void destroy();
        void fill();
        void print();
};

void Fibo1::init(int _n){
    n = _n;
    if (n > 0){
        tab = new int[n];
    }
    else {
        tab = 0;
    }
}

void Fibo1::destroy(){
    delete []tab;
}

void Fibo1::fill(){
    for (int i=0; i<n; i++){
        if (i==0 || i==1){
            tab[i]=1;
        }
        else {
            tab[i]=tab[i-2]+tab[i-1];
        }
    }
}

void Fibo1::print() {
    for (int i=0; i<n; i++){
        cout<<tab[i]<<endl;
    }
}

void test_fibo1(){
    Fibo1 f;
    f.init(5);
    f.fill();
    f.print();
    f.destroy();
}

class Fibo{
    int*tab;
    int n;
public:
    Fibo (int _n){
        n = _n;
        if (n > 0){
            tab = new int[n];
        }
        else {
            tab = 0;
        }
    };
    ~Fibo(){
        delete []tab;
    };
    void fill();
    void print();
};



void Fibo::fill(){
    for (int i=0; i<n; i++){
        if (i==0 || i==1){
            tab[i]=1;
        }
        else {
            tab[i]=tab[i-2]+tab[i-1];
        }
    }
}

void Fibo::print() {
    for (int i=0; i<n; i++){
        cout<<tab[i]<<endl;
    }
}

void test_fibo(){
    Fibo f(6);
    f.fill();
    f.print();
}

class ArithmeticSequence{
    double *tab;
    double n;
    double first;
    double r;
public:
    ArithmeticSequence(double _n, double _first, double _r){
        n = _n;
        first = _first;
        r = _r;
        if (n > 0){
            tab = new double [n];
        }
        else {
            tab = 0;
        }
    }
    void fill();
    void print();
    ~ArithmeticSequence(){
        delete []tab;
    };

};

void ArithmeticSequence::fill(){
    for (int i=0; i<n; i++){
        tab[i] = first + i * r;
    }
}

void ArithmeticSequence::print() {
    for (int i=0; i<n; i++){
        cout<<tab[i]<<endl;
    }
}

void test_aritm_sen(){
    ArithmeticSequence a(3, 1, 2);
    a.fill();
    a.print();
}

int main() {
    //Zadanie 1
    int x=7,y=8;
    std::cout<<x<<std::endl;
    cout<<x<<"+"<<y<<"="<<x+y <<endl;
    cin>>x>>y;
    string imie;
    string nazwisko;
    cin>>imie>>nazwisko;
    cout<<imie<<" "<<nazwisko<<endl;
    cout<<"fibo1"<<endl;
    fibo1();
    cout<<"fibo2"<<endl;
    fibo2(5);
    cout<<"test_fibo1"<<endl;
    test_fibo1();
    cout<<"test_fibo"<<endl;
    test_fibo();
    cout<<"test_arithm_sen"<<endl;
    test_aritm_sen();
    return 0;
}
