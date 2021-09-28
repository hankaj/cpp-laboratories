#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
using namespace std;

class Array{
    double*data;
    int n;
public:
    Array(int size=20):data(0){
        n=(size>=0)?size:0;
        if(n>0)data = new double[n];
    }
    ~Array(){
        if(data)delete data;
    }

    /*
     * wypełnij tablice równomiernie rozłożonymi elementami od start to end
     * Funkcja zwraca false, jeżeli end<start
     * */
    bool linspace(double start=0,double end=1);

    /*
     * wypełnij tablice losowymi elementami z zakresu 0-1
     *
     * Wywołanie srand()
     * jeżeli seed == -1 zainicjuj generator liczb pseudolosowych time(0)
     * w przeciwnym wypadku zainicjuj wartością seed
     * */
    void random(int seed=-1);

    /*
     * zapis do strumienia wyjściowego w formacie:
     * n, ciąg n liczb typu double
     */

    void write_formatted(ostream&os)const;

    /*
     * Zwolnij pamięć
     * Przeczytaj n
     * Przydziel pamięć
     * Przeczytaj liczby
     * */
    void read_formatted(istream&is);

    /*
     * Dodaj do każdego elementu tablicy wartość v
     */
    void add(double v);

    /*
     * Pomnóż każdy element tablicy przez wartość v
     */
    void mul(double v);
    void print();


    /*
     * Zmień kolejność elementów
     * Seed analogicznie, jak dla random
     * powtórz n/2 razy:
     *      wylosuj indeks i
     *      wylosuj indeks j
     *      zamień miejscami data[i] i data[j]
     * */
    void shuffle(int seed=-1);

    /*
     * Posortuj - sortowanie bąbelkowe
     * */

    void bubble_sort();

    /*
     * Posortuj za pomocą bibliotecznej funkcji qsort
     */
    void qsort();

    /*
     * porównuje elementy
     * zwraca false jeżeli
     *    - długości tablic są różne
     *    - dla któregoś z elementów zachodzi |data[i] - other.data[i]|>tol
     */
    bool equals(const Array&other, double tol=1e-10);
    static int cmpfunc (const void * a, const void * b);

};

void Array::print() {
    for (int i=0; i<n; i++){
        cout<<data[i]<<endl;
    }
}

bool Array::linspace(double start,double end){
    if(start>end) {
        return false;
    }
    if (n==1) return false;
    double dist = (end - start) / (n - 1);
    for (int i=0; i < n; i++){
        data[i] = start + dist * i;
    }
    return true;
}

void Array::random(int seed){
    if(seed==-1){
        srand(time(0));
    }else{
        srand(seed);
    }
    for (int i=0; i<n; i++){
        data[i] = ((double) rand() / (RAND_MAX));
    }
}

void Array::add(double v){
    for (int i=0; i<n; i++){
        data[i] += v;
    }
}

void Array::mul(double v){
    for (int i=0; i<n; i++){
        data[i] *= v;
    }
}

void Array::write_formatted(ostream&os)const{
    os << n << endl;
    for (int i=0; i<n; i++){
        os << data[i] << endl;
    }
}

void Array::read_formatted(istream &is) {
    delete [] data;
    is >> n;
    data = new double[n];
    for (int i=0; i<n; i++){
        is >> data[i];
    }
}

void Array::shuffle(int seed){
    int id1, id2;
    double tmp;
    if(seed==-1){
        srand(time(0));
    }else{
        srand(seed);
    }
    for (int i=0; i < n/2; i++){
        id1 = rand() % n;
        id2 = rand() % n;
        tmp = data[id1];
        data[id1] = data[id2];
        data[id2] = tmp;
    }
}

void Array::bubble_sort() {
    double tmp;
    for (int i=0; i<n; i++){
        for (int j=0; j<n-1; j++){
            if (data[j] > data[j+1]){
                tmp = data[j];
                data[j] = data[j+1];
                data[j+1] = tmp;
            }
        }
    }
}

int Array::cmpfunc (const void * a, const void * b)
{
    if (*(double*)a > *(double*)b)
        return 1;
    else if (*(double*)a < *(double*)b)
        return -1;
    else
        return 0;
}

void Array::qsort() {
    std::qsort(data, n, sizeof(double), cmpfunc);
}

bool Array::equals(const Array&other, double tol){
    int n1 = n;
    int n2 = other.n;
    if (n1 != n2){
        return false;
    }
    for (int i=0; i<n; i++){
        if (fabs(data[i]-other.data[i])>tol){
            return false;
        }
    }
    return true;
}

void test_linspace(){
    Array a;
    a.linspace();
    a.print();
    Array b(6);
    b.linspace(0, 5);
    b.print();
}

void test_random(){
    Array a(7);
    a.random();
    a.print();
}

void test_mul_add(){
    Array a(5);
    a.random();
    a.add(4);
    cout << "4 to 5" << endl;
    a.print();
    Array b(5);
    b.random();
    b.add(-0.5);
    b.mul(20);
    cout << "-10 to 10" << endl;
    b.print();
    Array c(5);
    c.linspace(0, 1);
    c.mul(100);
    cout << "0 to 100" << endl;
    c.print();
    Array d(11);
    d.linspace(0, 10);
    d.mul(-1);
    cout << "0 to -10" << endl;
    d.print();
}

void test_file_write(const char*nazwa_pliku){
    Array array(10);
    array.linspace();
    ofstream os(nazwa_pliku);
    if(!os){
        cerr<<"Błąd otwarcia pliku: "<<nazwa_pliku<<endl;
        return;
    }
    array.write_formatted(os);
}

void test_file_read(const char*nazwa_pliku){
    Array array(0);
    ifstream ifs(nazwa_pliku);
    if(!ifs){
        cerr<<"Błąd otwarcia pliku: "<<nazwa_pliku<<endl;
        return;
    }
    array.read_formatted(ifs);
    array.print();
}

void test_shuffle_bubble_sort(){
    Array a(5);
    a.linspace();
    a.shuffle();
    a.print();
    a.bubble_sort();
    a.print();
}

void test_shuffle_qsort(){
    Array a(5);
    a.linspace();
    a.shuffle();
    a.print();
    a.qsort();
    a.print();
}

void test_equals(){
    Array a(5);
    a.linspace();
    Array b(5);
    b.linspace();
    a.print();
    b.print();
    if(a.equals(b)){
        cout << "equals"<< endl;
    };
}

/*
 * Utwórz tablicę 10 obiektów typu array
 * Wypełnij je wartościami losowymi z zakresu [i,i+1], gdzie i = 0,...,10
 * Zapisz do pliku liczbę 10
 * W kolejnych wierszach zapisz tab[i]
 */

void test_save_array_2D(const char*file_name){
    Array *tab = new Array[10];
    for (int i=0; i<10; i++){
        tab[i].random();
    }
    ofstream os(file_name);
    if(!os){
        cerr<<"Błąd otwarcia pliku: "<<file_name<<endl;
        return;
    }
    os << 10 << endl;
    for (int i=0; i<10; i++){
        tab[i].write_formatted(os);
    }

    delete[]tab;
}

/*
 * Przeczytaj z pliku liczbę tablic
 * Utwórz dynamicznie tablicę obiektów typu Array
 * Przeczytaj z pliku zawartość kolejnych tablic
  */

void test_load_array_2D(const char*file_name, const char*file_name2=0){
    ifstream ifs(file_name);
    int number;
    if(!ifs){
        cerr<<"Błąd otwarcia pliku: "<<file_name<<endl;
        return;
    }
    ifs >> number;
    if (number<=0) return;
    Array *tab = new Array[number];
    for (int i=0; i<number; i++){
        tab[i].read_formatted(ifs);
    }
    for (int i=0; i<number; i++){
        tab[i].print();
    }
    if (file_name2){
        test_save_array_2D(file_name2);
    }
}
/*
 * Otwórz plik
 * Sprawdz, czy się udało
 * wczytaj kolejne znaki i dodawaj do obiektu klasy string
 * w petli nieskończonej wykonaj
 *     int c = is.get();
 *     if(c<0)break;
 *     dodaj c do stringu
 * zwróć string
 */
string load_file(const char*name){
    ifstream is(name);
    if(!is){
        cerr<<"Bład otwarcia pliku:"<<name<<endl;
        return "";
    }
    string r;
    for(;;){
        int c = is.get();
        if (c<0) break;
        r += (char) c;
    }
    return r;
}

void test_load_save(){
    test_save_array_2D("plik1.txt");
    test_load_array_2D("plik1.txt", "plik2.txt");
    string s1= load_file("plik1.txt");
    string s2= load_file("plik2.txt");
    cout<<"equals? " << (s1==s2) << endl;
}

int main() {
    test_linspace();
    test_random();
    test_mul_add();
    test_file_write("test");
    test_file_read("test");
    test_shuffle_bubble_sort();
    test_shuffle_qsort();
    test_equals();
    test_save_array_2D("array.txt");
    test_load_array_2D("array.txt");
    test_load_save();

    return 0;
}
