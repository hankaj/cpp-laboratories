#include <iostream>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cfloat>

using namespace std;

class Komora{
public:
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    Komora(int _x1=0,int _y1=0,int _z1=0,int _x2=0, int _y2=0,int _z2=0)
            :x1(_x1),y1(_y1),z1(_z1),x2(_x2),y2(_y2),z2(_z2)
    {
        normalizuj();
    }

    /*
     * Znormalizuj tak aby x1<=x2, y1>=y2, z1>=z2
     */
    void normalizuj();


    /*
     * Czy nasza jama przecina się z inną komorą?
     * Jeżeli dwie komory graniczą z zewnątrz ścianami
     * (współrzędne są równe) to nie przecinają się
     */
    bool czy_przecina(const Komora&other)const;

    /*
     * zwróć string sformatowany jako ( x1 y1 z1 , x2 y2 z2 )
     */
    string to_string()const;

    /*
     * wypisz w formacie ( x1 y1 z1 , x2 y2 z2 )
     */
    ostream& wypisz(ostream&os)const;

    /*
     * wczytaj w takim samym formacie ( x1 y1 z1 , x2 y2 z2 )
     *
     */
    istream& wczytaj_ze_spacjami(istream&is);

    /*
     * Docelowa wersja, nawiasy i przecinki mogą łączyć się z liczbami
     * wczytaj w formacie (x1 y1 z1,x2 y2 z2)
     *
     */
    istream& wczytaj(istream&is);

    /*
     * wylosuj wartości z podanych zakresów
     * Zadbaj, aby współrzedne były rózne, np. stosujac do-while
     *
     */
    void losuj(int xmin,int ymin,int zmin,int xmax,int ymax,int zmax);


    /*
     * przesuwa współrzedne komory o wektor (dx,dy,dz)
     */
    void przesun(int dx,int dy,int dz);
};

void Komora::normalizuj(){
    int tmp;
    if (x1 > x2){
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y1 < y2){
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    if (z1 < z2){
        tmp = z1;
        z1 = z2;
        z2 = tmp;
    }
}

string Komora::to_string()const{
    ostringstream os;

    //użyj innej metody do wypisania na strumień ostringstream - zapisu do stringu
    os << "( ";
    os << x1;
    os << " ";
    os << y1;
    os << " ";
    os << z1;
    os << " , ";
    os << x2;
    os << " ";
    os << y2;
    os << " ";
    os << z2;
    os << " )";

    return os.str();
}

ostream& Komora::wypisz(ostream&os)const{
    os << to_string();
    return os;
}

istream& Komora::wczytaj_ze_spacjami(istream &is) {
    string s, s2, s3;
    is>>s;
    if(s!="(")is.setstate(ios::failbit);
    // wczytaj współrzedne
    is >> x1;
    is >> y1;
    is >> z1;
    // wczytaj przecinek
    is >> s2;
    if(s2!=",")is.setstate(ios::failbit);
    // znowu współrzędne
    is >> x2;
    is >> y2;
    is >> z2;
    // nawias
    is >> s3;
    if(s3!=")")is.setstate(ios::failbit);
    normalizuj();
    return is;
}

istream& Komora::wczytaj(istream&is){
    string s;

    getline(is,s,','); // wczytaj do przecinka
    int idx = s.find('('); // znajdź nawias
    if(idx==string::npos){
        is.setstate(ios::failbit);
        return is;
    }

    s=s.substr(idx+1, s.size());// wydziel fragment po nawiasie
    istringstream iss(s); // utwórz wejściowy strumień z obiektu string
    iss>>x1; // wczytaj współrzędne
    iss>>y1;
    iss>>z1;

    getline(is,s,')'); // wczytaj do nawiasu zamykajacego
    iss.clear();
    iss.str(s);  // przypnij strumień do nowego stringu
    iss >> x2; // następne współzedne
    iss >> y2;
    iss >> z2;
    if (!iss) {
        is.setstate(ios::failbit);
        return is;
    }
    normalizuj();
    return is;
}

void Komora::losuj(int xmin,int ymin,int zmin,int xmax,int ymax,int zmax){
    int xrange = xmax - xmin + 1;
    int yrange = ymax - ymin + 1;
    int zrange = zmax - zmin + 1;

    x1 = rand() % xrange + xmin;
    x2 = rand() % xrange + xmin;
    while (x1 == x2){
        x2 = rand() % xrange + xmin;
    }

    y1 = rand() % yrange + ymin;
    y2 = rand() % yrange + ymin;
    while (y1 == y2){
        y2 = rand() % yrange + ymin;
    }

    z1 = rand() % zrange + zmin;
    z2 = rand() % zrange + zmin;
    while (z1 == z2){
        z2 = rand() % zrange + zmin;
    }
    normalizuj();
}

void Komora::przesun(int dx,int dy,int dz){
    x1 += dx;
    x2 += dx;
    y1 += dy;
    y2 += dy;
    z1 += dz;
    z2 += dz;
}

bool Komora::czy_przecina(const Komora&other)const{
    return (((x1 < other.x2) && (x2 > other.x1)) && (y2 < other.y1 && y1 > other.y2) && (z2 < other.z1 && z1 > other.z2));
}

void test_komora_wypisz(){
    Komora k(1,2,3,4,5,6);
    k.wypisz(cout);
}

void test_komora_odczyt_spacje(){
    istringstream is("( 1 2 3 , 4 5 -6 )");
    Komora k;
    k.wczytaj_ze_spacjami(is);
    k.wypisz(cout);
}

void test_czy_przecina(){
    istringstream is("(3 1 6,3 4 3)(2 2 8,6 6 5)");
    Komora k;
    k.wczytaj(is);
    k.wypisz(cout);
    Komora k2;
    k2.wczytaj(is);
    k2.wypisz(cout);
    cout<<"Przecina:"<<k.czy_przecina(k2)<<endl;
}

void test_czy_losowe_przecina(){
    Komora k(1, 1, 6,4, 4, 3);
    k.wypisz(cout);
    cout<<endl;
    srand(0);
    for(int i=0;i<10;i++){
        Komora k2;
        k2.losuj(0, 0, 0, 10, 10, 10);
        k2.wypisz(cout);
        cout<<" "<<k.czy_przecina(k2)<<endl;
    }
}


void test_przesun(){
    Komora k(1, 1, 6,4, 4, 3);
    k.wypisz(cout);
    cout<<endl;
    k.przesun(5,10,15);
    k.wypisz(cout);
}

void test_komora_odczyt(){
    istringstream is("(1 2 3,4 5 6)(23 24 25,26 27 28)");
    Komora k;
    k.wczytaj(is);
    k.wypisz(cout);
    k.wczytaj(is);
    k.wypisz(cout);
}

double jaka_wysokosc(Komora *komory, int liczba_komor, double ilosc_wody){
    vector <int> glebokosci;
    int pole_podstawy, zmin, zmax, x, y, z;
    double h, poziom_napelnienia=0;
    for (int i=0; i < liczba_komor; i++){
        glebokosci.push_back(komory[i].z1);
        glebokosci.push_back(komory[i].z2);
    }
    sort( glebokosci.begin(), glebokosci.end() );
    glebokosci.erase( unique( glebokosci.begin(), glebokosci.end() ), glebokosci.end() );
    for (int i=0; i < glebokosci.size() - 1; i++){
        pole_podstawy = 0;
        zmin = glebokosci[i];
        zmax = glebokosci[i+1];
        z = zmax - zmin;
        for (int j=0; j < liczba_komor; j++){
            if ((komory[j].z2 <= zmin) && (komory[j].z1 >= zmax)){
                x = komory[j].x2 - komory[j].x1;
                y = komory[j].y1 - komory[j].y2;
                pole_podstawy += x * y;
            }
        }
        h = ilosc_wody / pole_podstawy;
        if (h>z) {
            ilosc_wody -= pole_podstawy * z;
            poziom_napelnienia += z;
        }
        else {
            poziom_napelnienia += h;
            break;
        }
    }
    return poziom_napelnienia;
}

void test_jaka_wysokosc(){
    int liczba_komor = 4;
    Komora komory[liczba_komor];
    Komora k0(0, 0, 2, 1, 1, 6), k1(1, 0, 3, 2, 1, 5),
    k2(2,0,0,3,2,6), k3(3,0,1,4,1,4);
    komory[0] = k0;
    komory[1] = k1;
    komory[2] = k2;
    komory[3] = k3;
    cout << jaka_wysokosc(komory, liczba_komor, 2);
}

class Kretowisko {
public:
    vector<Komora> komory;

/*
* dodaje komorę, ale tylko w przypadku,
* kiedy nie przecina się z posotałymi
*/
    bool dodaj(const Komora &k);

/*
* Buduje kretowisko losując n komór o współrzednych x, y z zakresu [min,max]
* Wygenerowane komory należy przesunąć o dx=rand()%scatter, dy=rand()%scatter i dz=0
* Ponieważ komory są pod ziemią - dla współrzednej z losuj z zakresu -abs(depth) do -1
*/
    void buduj(int n, int min, int max, int depth, int scatter);

/*
* Zapisz w formacie ( komora1,komora2,komora3,....)
*/
    ostream &wypisz(ostream &os) const;

/*
* Wczytaj w takim formacie, jak zapisałeś
*/

    istream &wczytaj(istream &is);

/*
* Zwróć całkowitą powierzchnię komór
*/
    int powierzchnia() const;

/*
* Zwróć powierzchnię komór na głębokości depth
*/
    int powierzchnia(int depth) const;

/*
* Zwróć całkowitą objętość komór
*/
    int objetosc() const;

/*
* Zwróć objętość komór od -inf do głębokości depth
* Jest to patrząc formalnie całka powierzchnia(d) delta_d
* Ale powierzchnia(d) to funkcja odcnikami stała...
*/
    int objetosc(int depth) const;

/*
* Wszystkie komory są połączone ze sobą. Złośliwy ogrodnik wlał do kretowiska wodę,
* która rozpłynęła się pomiędzy komoram.
* Załóż że objętość wlanej wody nie przewyższa sumarycznej objętości komór
* Oblicz do jakiego poziomu zostaną zalane komory?
*/
    double poziom_wody(double v) const;
};


bool Kretowisko::dodaj(const Komora&k){
    int i;
    for (i=0; i<komory.size(); i++){
        if (k.czy_przecina(komory[i])){
            return false;
        }
    }
    komory.push_back(k);
    return true;}

void Kretowisko::buduj(int n, int min, int max, int depth, int scatter){
    int i=0;
    if(depth>0)depth*=-1;
    while(i<n){
        Komora k;
        k.losuj(min, min, depth, max, max, -1);
        k.przesun(rand()%scatter, rand()%scatter,0);
        if (dodaj(k)) i++;
    }
}

ostream&Kretowisko::wypisz(ostream&os)const{
    os<<'(';
    int i;
    for (i=0; i<komory.size(); i++){
        os << komory[i].to_string() << " ";
    }
    os<<')';
    return os;
}

istream&Kretowisko::wczytaj(istream&is){
    komory.clear(); //nie chcemy poprzednio zdefiniowanych komór
    int c;
    do{
        c=is.get();
    }while(c!='(' && is); // znajdź '(' ale przerwij, gdy !is

    for(;;){
        c=is.get();
        // jeżeli koniec pliku lub napotkano ')' to przerwij
        if (!is || c==')') break;
        if(c=='('){
            // jeżeli napotkano '(', to
            // cofnij znak do strumienia za pomocą unget()
            is.unget();
            // wczytaj komorę i dodaj do kretowiska
            Komora k;
            k.wczytaj(is);
            dodaj(k);
        }
    }
    return is;
}

int Kretowisko::powierzchnia(int depth)const{
    int x, y;
    int pow = 0;
    for (int j = 0; j < komory.size(); j++){
        if (komory[j].z2 <= depth && komory[j].z1 >= depth){
            x = komory[j].x2 - komory[j].x1;
            y = komory[j].y1 - komory[j].y2;
            pow += x * y;
        }
    }
    return pow;
}

int Kretowisko::powierzchnia() const {
    int x, y;
    int i;
    int pow = 0;
    for (i = 0; i < komory.size(); i++){
        x = komory[i].x2 - komory[i].x1;
        y = komory[i].y1 - komory[i].y2;
        pow += x * y;
    }
    return pow;
}

int Kretowisko::objetosc()const{
    int obj = 0;
    int i;
    int x, y, z;
    for (i = 0; i<komory.size(); i++){
        x = komory[i].x2 - komory[i].x1;
        y = komory[i].y1 - komory[i].y2;
        z = komory[i].z1 - komory[i].z2;
        obj += x * y * z;
    }
    return obj;
}

int Kretowisko::objetosc(int depth) const {
    vector <int> glebokosci;
    int pole_podstawy, zmin, zmax, x, y, z;
    int obj=0;
    for (int i=0; i < komory.size(); i++){
        glebokosci.push_back(komory[i].z1);
        glebokosci.push_back(komory[i].z2);
    }
    sort( glebokosci.begin(), glebokosci.end() );
    glebokosci.erase( unique( glebokosci.begin(), glebokosci.end() ), glebokosci.end() );
    for (int i=0; i < glebokosci.size() - 1; i++){
        pole_podstawy = 0;
        zmin = glebokosci[i];
        zmax = glebokosci[i+1];
        z = zmax - zmin;
        for (int j=0; j < komory.size(); j++){
            if ((komory[j].z2 <= zmin) && (komory[j].z1 >= zmax)){
                x = komory[j].x2 - komory[j].x1;
                y = komory[j].y1 - komory[j].y2;
                pole_podstawy += x * y;
            }
        }

        if (depth>=zmax) {
            obj += pole_podstawy * z;
        }
        else {
            obj += pole_podstawy * (depth - zmin);
            break;
        }
    }
    return obj;
}

double Kretowisko::poziom_wody(double v) const{
    vector <int> glebokosci;
    int pole_podstawy, zmin, zmax, x, y, z;
    double h, poziom_napelnienia=0;
    if (objetosc() < v) return DBL_MAX;
    for (int i=0; i < komory.size(); i++){
        glebokosci.push_back(komory[i].z1);
        glebokosci.push_back(komory[i].z2);
    }
    sort( glebokosci.begin(), glebokosci.end() );
    glebokosci.erase( unique( glebokosci.begin(), glebokosci.end() ), glebokosci.end() );
    for (int i=0; i < glebokosci.size() - 1; i++){
        pole_podstawy = 0;
        zmin = glebokosci[i];
        zmax = glebokosci[i+1];
        z = zmax - zmin;
        for (int j=0; j < komory.size(); j++){
            if ((komory[j].z2 <= zmin) && (komory[j].z1 >= zmax)){
                x = komory[j].x2 - komory[j].x1;
                y = komory[j].y1 - komory[j].y2;
                pole_podstawy += x * y;
            }
        }
        h = v / pole_podstawy;
        if (h>z) {
            v -= pole_podstawy * z;
            poziom_napelnienia += z;
        }
        else {
            poziom_napelnienia += h;
            break;
        }
    }
    poziom_napelnienia += glebokosci[0];
    return poziom_napelnienia;
}

void test_kretowisko_buduj(){
    srand(0);
    Kretowisko k;
    k.buduj(10,0,5,10,20);
    k.wypisz(cout);
}

void test_kretowisko_wczytaj()
{
    string s="(( 8 15 -5 , 11 14 -7 ) ( 14 13 -4 , 17 11 -8 ) ( 13 7 -3 , 16 4 -4 ) "
             "( 1 6 -7 , 4 4 -10 ) ( 16 20 -4 , 18 19 -10 ) ( 5 16 -7 , 8 14 -8 ) "
             "( 13 11 -3 , 17 10 -5 ) ( 11 20 -2 , 13 18 -7 ) ( 17 14 -5 , 18 12 -8 ) "
             "( 17 18 -4 , 20 16 -9 ) )";
    istringstream is(s);
    Kretowisko k;
    k.wczytaj(is);
    cout<<s<<endl;
    k.wypisz(cout);
}

void test_kretowisko_buduj_zapisz_wczytaj_wypisz(){
    srand(0);
    string s, s2;
    Kretowisko k;
    // zbuduj
    k.buduj(5,0,5,10,20);
    // zapisz do stringu s
    ostringstream os(s);
    k.wypisz(os);
    s = os.str();
    // wczytaj ze stringu s
    istringstream is(s);
    k.wczytaj(is);
    // wypisz na cout
    k.wypisz(cout);
    // zapisz do stringu s2
    ostringstream os2(s2);
    k.wypisz(os2);
    s2 = os2.str();
    cout << endl << "Are strings the same? " << (s == s2) << endl;
}

void test_jedna_komora(){
    srand(0);
    Kretowisko k;
    k.buduj(1,0,5,50,20);
    k.wypisz(cout);
    cout << endl;
    int z1 = k.komory[0].z1;
    int z2 = k.komory[0].z2;
    for (int i = z2; i <= z1; i++){
        cout << k.powierzchnia(i) << " " << k.objetosc(i) << endl;
    }
}

void test_dwie_komory(){
    srand(0);
    Kretowisko k;
    k.buduj(2,0,5,50,20);
    k.wypisz(cout);
    cout << endl;
    int z1 = max(k.komory[0].z1, k.komory[1].z1);
    int z2 = min(k.komory[0].z2, k.komory[1].z2);
    for (int i = z2; i <= z1; i++){
        cout << k.powierzchnia(i) << " " << k.objetosc(i) << endl;
    }
}

void test_objetosc_odd(){
    srand(0);
    Kretowisko k;
    k.buduj(10,0,5,300,20);
    int zmin = k.komory[0].z2;
    int zmax = k.komory[0].z1;
    for (int i = 0; i < k.komory.size(); i++){
        if (k.komory[i].z2 < zmin) zmin = k.komory[i].z2;
        if (k.komory[i].z1 > zmax) zmax = k.komory[i].z1;
    }
    double step = ((double)zmax - (double)zmin)/100;
    cout << zmin << " " << zmax << " "<< step << endl;
    for (int z = zmin; z < zmax; z+=step){
        cout << k.powierzchnia(z) << " " << k.objetosc(z) << endl;
    }
}

void test_poziom_wody_odv(){
    srand(0);
    Kretowisko k;
    k.buduj(10,0,5,300,20);
    int v;
    double d;
    int V = k.objetosc();
    for (v = 0; v <= V; v++){
        d = k.poziom_wody(v);
        cout << v << " " << d << " " << k.powierzchnia(d) << endl;
    }
}



int main() {
    //test_komora_wypisz();
    //test_komora_odczyt_spacje();
    //test_komora_odczyt();
    //test_czy_przecina();
    //test_czy_losowe_przecina();
    //test_przesun();
    //test_jaka_wysokosc();

    //test_kretowisko_buduj();
    //test_kretowisko_wczytaj();
    //test_kretowisko_buduj_zapisz_wczytaj_wypisz();
    //test_jedna_komora();
    //test_dwie_komory();

    //test_objetosc_odd();
    test_poziom_wody_odv();

    return 0;
}
