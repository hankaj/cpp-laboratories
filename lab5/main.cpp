#include <iostream>
#include <set>
#include <vector>
#include <iostream>
using namespace std;

class relation{
    class pair{
    public:
        int x;
        int y;
//        bool operator<(const pair&other)const;
        pair(int _x, int _y):x(_x),y(_y){}
    };
public:
    vector<pair> pairs;
    /*
     * czy zawiera parę (x,y)?
     */
    bool has_pair(int x,int y)const;
    /*
     * dodaje parę
     * para musi być unikalna (nie może pojawiac sie kilka razy)
     */
    void add(int x,int y);
    /*
     * zwraca dziedzine relacji (iksy)
     */
    set<int> get_domain()const;
    /*
     * zwraca przeciwdziedzine relacji (igreki)
     */
    set<int> get_range()const;

    /*
     * czy relacja jest zwrotna?
     */

    bool is_reflexive()const;
    /*
     * czy relacja jest symetryczna?
     */
    bool is_symmetric()const;
    /*
     * czy jest przechodnia
     */
    bool is_transitive()const;
    /*
     * czy jest relacją równowazności
     */
    bool is_equivalence()const;
    /*
     *  czy jest antysymetryczna
     *  https://en.wikipedia.org/wiki/Antisymmetric_relation
     */
    bool is_antisymmetric()const;
    /*
     * Czy jest relacją częściowego porządkująca
     * https://en.wikipedia.org/wiki/Partially_ordered_set#Formal_definition
     */
    bool is_partial_order()const;
    /*
     * czy jest spójna
     * https://en.wikipedia.org/wiki/Connex_relation
     */
    bool is_connex()const;
    /*
     * czy jest relacja całkowitego porządku
     * https://en.wikipedia.org/wiki/Total_order
     */
    bool is_total_order()const;
    /*
     * Zapisuje relację w formacie dot
     * https://graphviz.org/
     */
    void to_dot(std::ostream&os)const;
};

bool relation::has_pair(int x,int y)const{
    for (pair p : pairs){
        if ((p.x == x) && (p.y == y)){
            return true;
        }
    }
    return false;
}

void relation::add(int x,int y){
    if (!has_pair(x, y)){
        pair new_pair(x, y);
        pairs.push_back(new_pair);
    }
}

set<int> relation::get_domain()const{
    set<int> r;
    for (pair p : pairs){
        r.insert(p.x);
    }
    return r;
}

set<int> relation::get_range()const{
    set<int> r;
    for (pair p : pairs){
        r.insert(p.y);
    }
    return r;
}

bool relation::is_reflexive()const{
    int x, y;
    bool has_pair;
    for (pair p : pairs){
        x = p.x;
        has_pair = false;
        for (pair r : pairs){
            if (x==r.x && x==r.y) has_pair = true;
        }
        if (!has_pair) return false;
    }
    for (pair p : pairs){
        y = p.y;
        has_pair = false;
        for (pair r : pairs){
            if (y==r.x && y==r.y) has_pair = true;
        }
        if (!has_pair) return false;
    }
    return true;
}

bool relation::is_symmetric()const{
    bool has_symmetric_pair;
    int x, y;
    for (pair p : pairs){
        x = p.x;
        y = p.y;
        has_symmetric_pair = false;
        for (pair r : pairs){
            if ((x==r.y)&&(y==r.x)) has_symmetric_pair = true;
        }
        if (!has_symmetric_pair) return false;
    }
    return true;
}

bool relation::is_transitive()const{
    bool is_pair_transitive;
    int x, y, z;
    for (pair p : pairs){
        x = p.x;
        y = p.y;
        is_pair_transitive = false;
        for (pair r : pairs){
            if (r.x==y){
                z = r.y;
                for (pair q : pairs){
                    if ((q.x==x) && (q.y==z)) is_pair_transitive = true;
                }
            }
        }
        if (!is_pair_transitive) return false;
    }
    return true;
}

bool relation::is_equivalence()const{
    return (is_reflexive() && is_symmetric() && is_transitive());
}

bool relation::is_antisymmetric()const{
    bool has_symmetric_pair;
    int x, y;
    for (pair p : pairs){
        x = p.x;
        y = p.y;
        if (x==y) continue;
        has_symmetric_pair = false;
        for (pair r : pairs){
            if ((x==r.y)&&(y==r.x)) has_symmetric_pair = true;
        }
        if (has_symmetric_pair) return false;
    }
    return true;
}

bool relation::is_partial_order()const{
    return (is_reflexive() && is_antisymmetric() && is_transitive());
}

bool relation::is_connex()const{
    bool are_connected;
    set<int> domain = get_domain();
    set<int> range = get_range();
    for (int x : domain){
        for (int y : range){
            are_connected = false;
//            if (x==y){
//                are_connected = true;
//            }
           for (pair p : pairs){
               if (((x==p.x)&&(y==p.y))||((x==p.y)&&(y==p.x))){
                   are_connected = true; break;
               }}
            if (!are_connected) return false;
        }
    }
    return true;
}

bool relation::is_total_order()const{
    return (is_antisymmetric() && is_transitive() && is_connex());
}

void relation::to_dot(std::ostream&os)const{
    os<<"digraph g{\n";
    for(auto&e:pairs){
        os<<e.x<<" -> "<<e.y<<";"<<endl;
    }
    os<<"}\n";
}

void info(const relation&r){
    cout<<"is_reflexive: "      <<r.is_reflexive()<<endl;
    cout<<"is_symmetric: "      <<r.is_symmetric()<<endl;
    cout<<"is_transitive: "     <<r.is_transitive()<<endl;
    cout<<"is_equivalence: "    <<r.is_equivalence()<<endl;
    cout<<"is_antisymmetric: "  <<r.is_antisymmetric()<<endl;
    cout<<"is_partial_order: "  <<r.is_partial_order()<<endl;
    cout<<"is_connex: "         <<r.is_connex()<<endl;
    cout<<"is_total_order: "    <<r.is_total_order()<<endl;

}

void test_relation_1(){
    relation r;
    int n=50;
    for(int x=0;x<n;x++){
        for(int y=0;y<n;y++){
            if(x%7==y%7)r.add(x,y);
        }
    }
    info(r);
}

void test_relation_2(){
    relation r;
    int n=50;
    for(int x=0;x<n;x++){
        for(int y=1;y<n;y++){
            if(x%y==0)r.add(x,y);
        }
    }
    info(r);
}

void test_relation_3(){
    relation r;
    int n=50;
    for(int x=0;x<n;x++){
        for(int y=0;y<n;y++){
            if(x<=y)r.add(x,y);
        }
    }
    info(r);
}

void test_relation_4(){
    relation r;
    int n=20;
    for(int x=-n;x<n;x++){
        for(int y=-n;y<n;y++){
            if(x*x<=y*y)r.add(x,y);
        }
    }
    info(r);
}

void test_relation_5(){
    relation r;
    int n=2;
    for(int x=-n;x<n;x++){
        for(int y=-n;y<n;y++){
            if(x*x<=y*y)r.add(x,y);
        }
    }
    info(r);
    r.to_dot(cout);
}

void test_relation_6(){
    relation r;
    int n=5;
    for(int x=0;x<n;x++)r.add(x,x);
    for(int x=0;x<n-3;x++){
        for(int y=1;y<n;y++){
            if(x<y+2)r.add(x,y);
        }
    }
    info(r);
    r.to_dot(cout);
}


int main() {
    //test_relation_1();
    //test_relation_2();
//    test_relation_3();
//    test_relation_4();
    test_relation_5();
    //test_relation_6();
}
