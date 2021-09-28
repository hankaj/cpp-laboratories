
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
//#include <iomanip>
using namespace std;

string mode_to_string(mode_t mode) {
    const char* chrmode = "xwr";
    string result;
    for (int i = 8; i >= 0; i--) {
        result += mode & (1<< i)?chrmode[i%3]:'-';
    }
    return result;
}

void print_file_info(const char*path){
    struct stat fstat;
    stat(path, &fstat);
    cout << "size:" << fstat.st_size << " ";
    char buf[20];
    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", localtime(&fstat.st_mtime));
    cout << "Modified at:" << buf<<" ";
    cout<< "mode: "<<mode_to_string(fstat.st_mode)<<" "<<oct<<fstat.st_mode;

}

static void test_dir1(){
    DIR *dir;
    struct dirent *entry;
    map<int,string> ftypes{
            {DT_UNKNOWN,"unknown"},
            {DT_REG,"file"},
            {DT_DIR,"dir"},
            {DT_FIFO,"pipe"},
            {DT_SOCK,"socket"},
            {DT_CHR,"chardev"},
            {DT_BLK,"blkdev"},
            {DT_LNK,"symlink"}
    };
    string dir_name="/";
    dir = opendir(dir_name.c_str());

    if(!dir)return;
    while ((entry = readdir(dir)) != NULL) {
        cout << entry->d_name << " [";
        cout<<ftypes[entry->d_type]<< " ";
        auto fpath = dir_name+"/"+entry->d_name;
        print_file_info(fpath.c_str());
        cout<<"]"<<endl;

    }
    closedir(dir);
}

class Dirent{
protected:
    // nazwa
    string name;
    // wskaźnik na jednostkę  nadrzędną (katalog)
    const Dirent*parent;
    // stuktura z atrybutami pliku
    struct stat fstat;
    // funkcja, która ma odczytać atrybuty pliku i zapisać w fstat
    void fill_info();

public:
    Dirent(const char*_name,const Dirent*_parent):name(_name),parent(_parent){
        fill_info();
    }
    Dirent(const Dirent&other){name.assign(other.name), parent=other.parent; fill_info();}
    virtual ~Dirent(){}
    string get_name()const {return name;}
    /*
     * Zwraca ścieżkę, która jest konkatenacją ścieżki rodzica, path_separtor i name
     */
    string get_path()const;

    /*
     * manualna implementacja RTTI
     */
    virtual bool is_file()const{return false;}
    virtual bool is_dir()const{return false;}
    /*
     * Formatuje opis uprawnień, kod w przykładach
     */
    string get_mode_string()const;
    /*
     * formatuje opis pliku (nazwa, czy katalog, rozmiar, data modyfikacji uprawnienie dostępu)
     */
    virtual string to_string()const;
    /*
     * może się przydać ale Windows też akceptuje /
     */
    static char path_separator;
    /*
     *
     */
    virtual void list(ostream&os,int indent)const;

    //virtual void scan(int)=0;
};

class File:public Dirent{
public:
    File(const char*_name,const Dirent*_parent=nullptr)
            :Dirent(_name,_parent){}
    virtual bool is_file()const{return true;};

    string to_string()const;
};

class Directory:public Dirent{
public:
    vector<Dirent*> entries;
    Directory(const char*_name,const Dirent*_parent=nullptr)
            :Dirent(_name,_parent){}
    Directory(const Directory&other):Dirent(other){copy(other);}
    /*
     * Uwaga entries zawierają wskaźniki, trzeba te pliki usunąć
     */
    ~Directory();
    virtual bool is_dir()const{return true;};

    string to_string()const;
    /*
     *
     */
    void scan(int maxdepth=INT_MAX);
    /*
     *
     */
    void list(ostream&os,int indent=0)const;
    void free();
    void copy(const Directory&other);
    Directory&operator=(const Directory&other);
    Dirent *find(const char*name);
};

char Dirent::path_separator='/';

string Dirent::get_path()const{

    if (parent) return parent->get_path()+path_separator+name;
    return name;
}

string Dirent::get_mode_string()const{
    const char* chrmode = "xwr";
    string result;
    for (int i = 8; i >= 0; i--) {
        result += fstat.st_mode & (1<< i)?chrmode[i%3]:'-';
    }
    return result;
}

string Dirent::to_string()const{
    return name;
}

void Dirent::fill_info(){
    auto full_path = get_path();
    stat(full_path.c_str(), &fstat);
}

void Dirent::list(ostream&os,int indent)const{
    for(int i=0;i<indent;i++){
        os<<"  ";
    } //wołamy przedefiniowaną polimorficzną funkcję klas potomnych...
    os<<to_string()<<endl;
}

string File::to_string()const{
    ostringstream os;
    os<<name<<" [";
    // rozmiar
    os<<"size:" << fstat.st_size << " ";
    // data modyfikajci
    char buf[20];
    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", localtime(&fstat.st_mtime));
    os << "Modified at:" << buf<<" ";
    // prawa dostępu i wartość ósemkowa praw dostępu
    os << "mode: "<<mode_to_string(fstat.st_mode)<<" "<<oct<<fstat.st_mode;
    os<<"]";
    return os.str();
}

Directory::~Directory(){
    free();
}

string Directory::to_string()const{
    ostringstream os;
    os<<name<<" <DIR> [";
    //czas modyfikacji
    char buf[20];
    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", localtime(&fstat.st_mtime));
    os << "Modified at:" << buf<<" ";
    // prawa dostępu (tekst i ósemkowo)
    os << "mode: "<<mode_to_string(fstat.st_mode)<<" "<<oct<<fstat.st_mode;
    os<<"]";
    return os.str();

}

void Directory::scan(int max_depth){
    if(max_depth==0)return;
    DIR *dir;
    dir = opendir(get_path().c_str());
    if(!dir)return;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        //pomiń pliki "." ".."
        //pomiń DT_LNK
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        if (entry->d_type==DT_LNK) continue;

        if(entry->d_type==DT_REG){
            entries.push_back(new File(entry->d_name, this));
        }
        if(entry->d_type==DT_DIR){
            entries.push_back(new Directory(entry->d_name, this));
        }
    }
    closedir(dir);

    if(max_depth>1) {
        for (auto e:entries) {
            //wywołaj scan(max_depth - 1) ale tylko dla obiektów typu directory
            if (e->is_dir()) {
                //Directory d = e;
                ((Directory*)e)->scan(max_depth-1);
            };
        }
    }
}


void Directory::list(ostream&os,int indent)const{
    // wypisz informacje o kataloguDirent::list(os,indent);
    Dirent::list(os, indent);
    // dodaj wcięcie i wypisz informacje o elementach podrzędnych
    for(int i=0;i<indent;i++){
        os<<"  ";
    };
    for (auto e:entries){
        e->list(os, indent+1);
    }
}

void Directory::free() {
    for (auto e:entries){
        delete e;
    }
}


void Directory::copy(const Directory &other) {
    name.assign(other.name);
    parent=other.parent;
    fstat=other.fstat;
    for (auto e:other.entries){
        if(e->is_file()){
            entries.push_back(new File(*(File*)e));
        }
        if(e->is_dir()){
            entries.push_back(new Directory(*(Directory*)e));
        }
    }
}


Directory&Directory::operator=(const Directory&other){
    if(&other==this) return *this;
    free();
    copy(other);
    return *this;
}

Dirent *Directory::find(const char *name) {
    Dirent *result=NULL;
    for (auto e:entries){
        if (!strcmp(e->get_name().c_str(),name)) return e;
        if (e->is_dir()) result=(*(Directory*)e).find(name);
        if (result) return result;
    }
    return result;
}

static void test_fast(){
    Directory d("/");
    d.scan(1);
    d.list(cout);
}

static void test_long(){
    Directory d("/Users/hania/Desktop/test/");
    d.scan(4); // 2,3 na początek
    ofstream of("dir.txt");
    d.list(of, 2);
    string found = (d.find("b"))->get_name();
    cout << found<<endl;
    Directory f=d;
    f.list(cout);
}




int main(){
    //test_dir1();
//    cout<<mode_to_string(0751)<<endl;
    //test_fast();
    test_long();
}

