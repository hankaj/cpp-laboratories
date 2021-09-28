#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;


class bad_index:public std::exception{};

class String{
    /*
     * txt to wskażnik na tablice znaków o pojemności capacity,
     * czyli zdolną pomieścić capacity-1 znaków. Zawsze pamiętaj o miejscu na znak 0 na końcu
     * długość tekstu nie musi być przechowywana, używamy funkcji strlen()
     */
    char*txt=0;
    int capacity=0;
public:
    /*
     * Konstruktor - jeżeli tekst niezerowy, przydziela pamięć
     */
    String(const char*_txt="");
    /*
     * Destruktor
     */
    ~String(){free();}
    /*
     * Konstruktor kopiujący
     */
    String(const String&other){copy(other);}

    /*
     * Operator przypisania.
     * Dodajmy optymalizację: jeżeli other.txt zmieści się w txt, nie zwalniajmy pamięci
     * jeżeli nie zmieści się: stosujemy free + copy
     */
    String&operator=(const String&other);

    /*
     *  Dostęp do znaku, może generować wyjątek bad_index;
     */
    char&operator[](int idx);
    /*
     * Dodaje tekst na końcu może rozszerzać tekst
     */
    String&operator+=(const char*_txt);

    /*
     * Zwraca obiekt String, którego zawartość jest konkatenacją obu tekstów
     */
    String operator+(const char*_txt)const;
    /*
     * Dodaje znak na końcu, może rozszerzać długość tablicy
     */
    String&operator+=(char c);
    /*
     * Zwraca nowy obiekt zawierający tekst ze znakiem na końcu
     */
    String operator+(char c)const;
    /*
     * Dodaje na końcu tektową reprezentację arg. Użyj funkcji sprintf()
     */
    String&operator+=(int arg);
    /*
     * Zwraca nowy obiekt zawierający tekst z dodaną tekstową reprezentacją argumentu
     */
    String operator+(int arg)const;

    /*
     * Dodaje na końcu tektową reprezentację arg. Użyj funkcji sprintf()
     */
    String&operator+=(double arg);
    /*
     * Zwraca nowy obiekt zawierający tekst z dodaną tekstową reprezentacją argumentu
     */
    String operator+(double arg)const;
    /*
     * Zwraca nowy obiekt zawierający zawartość wzorca (this->txt) powtórzoną n razy i sklejoną
     */
    String operator*(int n)const;

    /*
     * Jeżeli txt jest wskaźnikiem o wartości zerowej (nullptr) zwraca ""
     * w przeciwnym przypadku zwraca ptr
     */
    operator const char*()const;
    /*
     * Porównanie zawartości
     */
    bool operator==(const char*_txt)const;
    bool operator!=(const char*_txt)const{
        return !(*this == _txt);
    }
    /*
     * Porównanie leksykograficzne, użyj strcmp()
     */
    bool operator<(const char*txt)const;
    /*
     * Zwraca rozmiar (nie licząc znaku 0 na końcu). Jeżeli is_empty() zwraca 0;
     */
    int size()const;
    /*
     * Zwraca capacity
     */
    int get_capacity()const{return capacity;}
    /*
     * Test czy tekst jest pusty
     */
    bool is_empty()const{return !txt || !*txt;}
protected:
    /*
     * Zwalnia pamięć, zeruje zmienne
     */
    void free();
    /*
     * Kopiuje zawartość do pustego stringu
     */
    void copy(const String&other);
    /*
     * Rozszerza pamięć tablicy do rozmiaru równego co najmniej newcapacity
     * Implementuje własną logikę...
     */
    void reserve(int newcapaicty);
};

String::String(const char*_txt){
//    txt=0;
//    capacity=0; ....
    capacity = strlen(_txt)+1;
    if (capacity) txt = new char[capacity];
    strcpy(txt, _txt);
}

String&String::operator=(const String&other){
    if (other.size() <= size()) strcpy(txt, other.txt);
    else {
        free();
        copy(other);
    }
    return *this;
}

char&String::operator[](int idx){
    if (idx<0 || idx>capacity) throw bad_index();
    return txt[idx];
}

String&String::operator+=(const char*_txt){
    if(!_txt)return *this;
    if(size()+strlen(_txt)+1>capacity){
        reserve(size()+strlen(_txt)+1);
    }
    strcat(txt, _txt);
    return *this;
}

String String::operator+(const char*_txt)const{
    String r(txt);
    r += _txt;
    return r;
}

String&String::operator+=(char c){
    if(size()+2>capacity){
        reserve(size()+2);
    }
    txt[size()] = c;
    return *this;
}

String String::operator+(char c)const{
    String r(txt);
    r += c;
    return r;
}

String&String::operator+=(int arg){
    char buf[256];
    sprintf(buf, "%d", arg);
    *this += buf;
    return *this;
}

String String::operator+(int arg)const{
    String r(txt);
    r+=arg;
    return r;
}

String&String::operator+=(double arg){
    char buf[256];
    sprintf(buf, "%g", arg);
    *this += buf;
    return *this;
}

String String::operator+(double arg)const{
    String r(txt);
    r+=arg;
    return r;
}

String String::operator*(int n)const{
    if(!size()||n<=0)return String();
    String r(txt);
    for (int i=1; i<n; i++) r+=txt;
    return r;
}

String::operator const char*()const{
    if(!txt) return "";
    else return txt;
}

bool String::operator<(const char*_txt)const{
    if (strcmp(txt, _txt)<0) return true;
    else return false;
}

bool String::operator==(const char*_txt)const{
    return !strcmp(txt, _txt);
}

int String::size()const{
    if (is_empty()) return 0;
    return strlen(txt);
}

void String::free(){
    if (txt) delete []txt;
    txt = 0;
    capacity = 0;
}

void String::copy(const String&other){
    capacity = other.capacity; txt=0;
    if (capacity>0) txt = new char[capacity];
    strcpy(txt, other.txt);
}

void String::reserve(int newcapaicty){
    if(newcapaicty<=capacity)return;
    int c = 64;
    while(c<newcapaicty)c*=2;
    newcapaicty=c;
    char*tmp = new char[newcapaicty];
    if(txt){
        memcpy(tmp,txt,size()+1);
        delete txt;
    }else{
        tmp[0]=0;
    }

    txt = tmp;
    capacity=newcapaicty;
}

void  test_plus(){
    srand(0);
    String r;
    for(int i=0;i<10;i++){
        r+=(char)('a'+rand()%26);
    }
    cout<<r<<endl;
    r+="ABCDEFGH_";
    r+=12;
    r+="_@_";
    r+=3.14;
    r+='_';
    cout<<r<<endl;
    r=r*3;
    cout<<r<<endl;
}

void  test_plus2(){
    srand(1);
    String a;
    for(int i=0;i<10;i++){
        a+=(char)('a'+rand()%26);
    }
    String b;
    for(int i=0;i<10;i++){
        b+=(char)('0'+rand()%10);
    }
    String c= a+b;
    cout<<c<<endl;
    c=a+12.5;
    cout<<c<<endl;
    c=b+123.456;
    cout<<c<<endl;
}


int main() {
    test_plus();
    test_plus2();
    return 0;
}
