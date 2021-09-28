#include <iostream>
#include <vector>
#include <sstream>      // std::istringstream
#include <string>
#include <cmath>
using namespace std;

class Vector: public vector<double>{
public:
    /*
     * Konstruktor. Tworzy wektor n-elementowy (jeżeli n>0)
     */
    Vector(int n=0);

    /*
     * Mnoży każdy element wektora przez arg (inplace)
     */
    Vector&operator*=(double arg);

    /*
     * Dodaje do każdego elementu wektora arg (inplace)
     */
    Vector&operator+=(double arg);

    /*
     * Zwraca wektor, którego elementy są równe sumie
     * wartości elementów danego wektora i argumentu arg
     */
    Vector operator+(double arg)const;

    /*
     * Zwraca wektor, którego elementy są równe iloczynowi
     * wartości elementów danego wektora i argumentu arg
     */
    Vector operator*(double arg)const;

    /*
     * Jeżeli rozmoary są różne wyrzuca wyjątek
     * Dodaje do każdego i-tego elementu danego wektora i-ty element
     * argumentu arg (inplace)
     */
    Vector&operator+=(const Vector&arg);

    /*
     * Jeżeli rozmoary są różne wyrzuca wyjątek
     * Odejmuje od każdego i-tego elementu danego wektora i-ty element
     * argumentu arg (inplace)
     */
    Vector&operator-=(const Vector&arg);

    /*
     * Jeżeli rozmiary są różne wyrzuca wyjątek
     * Mnoży każdy i-ty element danego wektora przez i-ty element
     * argumentu arg (inplace)
     */
    Vector&operator*=(const Vector&arg);


    /*
     * Zwraca wektor, którego elementy są iloczynami odpowiednich elementów
     * danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne (niekoniecznie bezpośrednio)
     */
    Vector operator*(const Vector&arg)const;

    /*
     * Zwraca wektor, którego elementy są równe sumie odpowiednich elementów
     * danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne (niekoniecznie bezpośrednio)
     */
    Vector operator+(const Vector&arg)const;

    /*
     * Zwraca wektor, którego elementy są równe różnicy odpowiednich elementów
     * danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne (niekoniecznie bezpośrednio)
     */
    Vector operator-(const Vector&arg)const;

    /*
     * Zwraca iloczyn skalarny danego wektora i argumentu
     * Może wygenerować wyjątek, jeżeli rozmiary są różne
     */
    double operator|(const Vector&arg)const;

    /*
    * Oblicza sume elementów wektora
    */
    double sum()const;

    /*
     * Oblicza wartość średnią elementów wektora
     */
    double mean()const;

    /*
     * Oblicza odchylenie standardowe
     */
    double std()const;


    /*
     * Zwraca wektor, którego elementy są wynikiem wywołania funkcji apply
     * na elementach oryginalnego wektora
     */

    Vector apply(double (*f)(double))const;

    /*
     * Zwraca wektor, którego elementy sa równe elementowi danego wektora
     * podniesionymi do potęgi arg
     */
    Vector operator^(double arg)const;



    /*
     * Zwraca wektor o rozmiarz n wypełnionym zerami
     */
    static Vector zeros(int n);
    /*
     * Zwraca wektor o rozmiarz n wypełnionym losowymi wartościami z zakresu 0-1
     */
    static Vector random(int n);

    /*
     * Zwraca wektor wypełniony wartościam z zakresu start (włącznie) do step (weyłącznie)
     * Różnica kolejnych wartości wynosi step
     */

    static Vector range(double stop, double start=0, double step=1);
};

Vector::Vector(int n): vector<double>(n > 0 ? n : 0){
}

Vector&Vector::operator*=(double arg){
    for(auto&e:*this)e*=arg;
    return *this;
}

Vector Vector::operator*(double arg)const{
    Vector ret(*this);
    ret*=arg;
    return ret;
}

Vector&Vector::operator+=(double arg){
    for(auto&e:*this)e+=arg;
    return *this;
}

Vector Vector::operator+(double arg)const{
    Vector ret(*this);
    ret+=arg;
    return ret;
}

Vector&Vector::operator*=(const Vector&arg){
    if(size()!=arg.size())throw "bad size";
    for(int i=0;i<size();i++)
        (*this)[i]*=arg[i];
    return *this;
}

Vector&Vector::operator+=(const Vector&arg){
    if(size()!=arg.size())throw "bad size";
    for(int i=0;i<size();i++)
        (*this)[i]+=arg[i];
    return *this;
}

Vector&Vector::operator-=(const Vector&arg){
    if(size()!=arg.size())throw "bad size";
    for(int i=0;i<size();i++)
        (*this)[i]-=arg[i];
    return *this;
}

Vector Vector::operator*(const Vector&arg)const{
    Vector ret(*this);
    ret*=arg;
    return ret;
}

Vector Vector::operator+(const Vector&arg)const{
    Vector ret(*this);
    ret+=arg;
    return ret;
}

Vector Vector::operator-(const Vector&arg)const{
    Vector ret(*this);
    ret-=arg;
    return ret;
}

double Vector::operator|(const Vector&arg)const{
    if(size()!=arg.size())throw "bad size";
    double result = 0;
    for(int i=0;i<size();i++)
        result += (*this)[i]*arg[i];
    return result;
}

double Vector::sum()const{
    double result = 0;
    for(int i=0;i<size();i++)
        result += (*this)[i];
    return result;
}

double Vector::mean()const{
    return sum()/size();
}

double Vector::std()const{
    double m = mean();
    double s = 0;
    for(int i=0;i<size();i++)
        s += pow((*this)[i]-m, 2);
    return sqrt(s/size());
}

Vector Vector::zeros(int n){
    Vector ret(n);
    for(int i=0;i<ret.size();i++)
        ret[i] = 0;
    return ret;
}

Vector Vector::random(int n){
    Vector ret(n);
    for(int i=0;i<ret.size();i++)
        ret[i] = rand();
    return ret;
}

Vector Vector::range(double stop, double start, double step){
    int n = (stop - start)/step + 1;
    if (stop == (start + (n-1)*step)) n--;
    Vector ret(n);
    for(int i=0;i<ret.size();i++)
        ret[i] = start + i * step;
    return ret;
}

Vector Vector::apply(double (*f)(double))const{
    Vector ret(*this);
    for(int i=0;i<ret.size();i++)
        ret[i] = f(ret[i]);
    return ret;
}

ostream&operator<<(ostream&os,const Vector&v){
    os<<"[";
    for(int i=0;i<v.size();i++){
        if(i!=0)os<<", ";
        os<<v[i];
    }
    os<<"]";
    return os;
}

Vector Vector::operator^(double arg)const{
    Vector ret(*this);
    for(int i=0;i<ret.size();i++)
        ret[i] = pow(ret[i], arg);
    return ret;
}

istream&operator>>(istream&is, Vector&v){
    v.clear();
    string s, pom;
    double number;
    getline(is,s,']'); // wczytaj do znaku ]

    // zamień (przecinki i nawiasy) , ] na spacje
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == ',' || s[i] == ']' || s[i]=='[')
            s[i] = ' ';
    }

    istringstream iss(s);


    for(;;){
        // wczytaj liczby double i dodaj do wektora
        if (!iss) return is;
        iss >> number;
        if (!iss) return is;
        v.push_back(number);
    }
    return is;
}




// testy
static void test_read_write(){
    Vector r = Vector::range(10,0, 3);
    ostringstream oss;
    oss<<r;
    string repr = oss.str();
    cout<<repr<<endl;
    istringstream iss(repr);
    Vector v;
    iss>>v;
    v*=2;
    cout<<v<<endl;
}

static void test_add(){
    int n=10;
    Vector a = Vector::range(10);
    Vector b = Vector::range(10) * 5 + 2;
    Vector c = a + b;
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<c<<endl;
    cout<<c[1]<<endl;
}

void test_dot(){
    Vector a = Vector::range(10);
    Vector b = Vector::range(10) * 5 + 2;
    double c = a|b;
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<c<<endl;
}

void test_pow(){
    Vector a = Vector::range(10);
    Vector c = a^3;
    cout<<c<<endl;
}

void test_apply(){
    Vector a = Vector::range(10);
    Vector c = a.apply(log);
    cout<<c<<endl;
    Vector b = c.apply(exp);
    cout<<b<<endl;
}

void test_stats(){
    Vector a = Vector::random(10);
    a += (-0.5);
    a *= 20;
    Vector c = a^2;
    c *= (-1/8);
    Vector b = c.apply(exp);
    double m = b.mean();
    double d = b.std();
    cout << m << ' ' << d << endl;
}

int main() {
//    test_read_write();
//    test_add();
    //test_pow();
    //test_dot();
    //test_stats();
    test_apply();
    return 0;
}
