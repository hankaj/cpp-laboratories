#include <iostream>
#include <cstring>

class TextListElement
{
    friend class TextList;
    /*
     * Tworzy kopię tekstu txt alokując pamięć na stercie
     * za pomocą operatora new
     */
    TextListElement(const char*txt);
    /*
     * Zwalnia pamięć elementu i jego danych!
     */
    ~TextListElement();
    TextListElement*next;
    TextListElement*prev;
    char* value;
};

class TextList
{
    /* wskaźnik na pierwszy element */
    TextListElement*start;
    /* wskaźnik na ostatni element */
    TextListElement*end;
public:
    /* konstruktor*/
    TextList();
    /* destruktor */
    ~TextList();

    /*
     * Dodaje tekst na początku listy
     */
    void pushFront(const char* v);

    /*
     * Dodaje tekst na końcu listy
     */
    void pushBack(const char* v);

    /*
     * Zwraca wskaźnik do tekstu przechowywanego w pierwszym elemencie
     */
    const char*getFront()const;

    /*
     * Zwraca wskaźnik do tekstu przechowywanego w ostatnim elemencie
     */
    const char*getBack()const;

    /*
     * Usuwa pierwszy element
     */
    void deleteFront();

    /*
     * Usuwa ostatni element
     */
    void deleteBack();

    /*
     * Drukuje zawartość listy (w przód).
     * Oddziela elementy listy łańcuchem sep, kończy wydruk listy wypisując endWith
     */
    void dump(const char*sep=" ",const char*endWith="\n")const;

    /*
     * Drukuje zawartość listy (w tył)
     * Oddziela elementy listy łańcuchem sep, kończy wydruk listy wypisując endWith
     */
    void rdump(const char*sep=" ",const char*endWith="\n")const;

    /*
     * Konstruktor kopiujący
     */
    TextList(const TextList&other);
    /*
     * Operator przypisania
     */
    TextList&operator=(const TextList&other);

    /*
     * Czy lista jest pusta
     */
    bool isEmpty()const{return !start;}
protected:
    /* funkcja do wykorzystania, zwalania pamięć listy */
    void free();

    /* funkcja do wykorzystania,
     * zakłada, że lista jest pusta
     * ale zeruje wskaźniki start i end - bo będzie użyta w konstruktorze kopującym
     * i kopiuje do niej zawartość listy other
     */
    void copy(const TextList&other);
};

TextListElement::TextListElement(const char*txt){
    next=0;
    prev=0;
    value = new char[sizeof(char)*strlen(txt)+1];
    strcpy(value, txt);
}

TextListElement::~TextListElement() {
    next = nullptr;
    prev = nullptr;
    delete []value;
    value = nullptr;
}

TextList::TextList(){
    start=end=0;
}

TextList::~TextList(){
    free();
}

void TextList::pushFront(const char* v)
{
    //tutaj ogarnąć gdy start=0
    TextListElement *le = new TextListElement(v);
    le->next=start;
    le->prev= 0;
    start->prev=le;
    start=le;
    if (end==0) end=start;
}

void TextList::pushBack(const char* v)
{
    TextListElement *le = new TextListElement(v);
    le->next=0;
    le->prev=end;
    if(end)end->next=le;
    end=le;
    if(start==0)start=end;
}

const char*TextList::getFront()const{
    if (start==0) return 0;
    return start->value;
}

const char*TextList::getBack()const{
    if (end==0) return 0;
    return end->value;
}

void TextList::deleteFront(){
    if(!start) return;
    TextListElement*todel=start;
    if(start->next) start->next->prev=0;
    start=start->next;
    delete todel;
    if (start==0) end=0;
}

void TextList::deleteBack(){
    if (!start) return;
    TextListElement*todel=end;
    if(end->prev) end->prev->next=0;
    end=end->prev;
    delete todel;
    if (end==0) start=0;
}

void TextList::dump(const char*sep, const char*endWith)const{
    for (TextListElement*i=start;i!=0;i=i->next){
        std::cout<<i->value<<sep;
    }
    std::cout<<endWith;
}

void TextList::rdump(const char*sep, const char*endWith)const{
    for (TextListElement*i=end;i!=0;i=i->prev){
        std::cout<<i->value<<sep;
    }
    std::cout<<endWith;
}

void TextList::free(){
    while(start!=0) deleteFront();
}

void TextList::copy(const TextList&other){
    // 4 linijki, pamiętaj w konstruktorze kopiującym kopiujesz dane do niezainicjowanego obiektu
    start=0;
    end=0;
    for (TextListElement*i=other.start;i!=0;i=i->next){
        pushBack(i->value);
    }
}

TextList::TextList(const TextList&other){
    copy(other);
}

TextList&TextList::operator=(const TextList&other){
    if(&other!=this){
        free();
        copy(other);
    }
    return *this;
}

static void test_push_delete(){
    char buf[]="aa bb cc dd ee ff gg hh ii jj kk ll";
    TextList list;

    for(char*ptr=strtok(buf," ,.;\n");ptr;ptr=strtok(NULL," ,.;\n")){
        list.pushBack(ptr);
        list.pushFront(ptr);
    }
    list.dump();
    bool flag=true;
    while(!list.isEmpty()){
        if(flag)list.deleteFront();
        else list.deleteBack();
        list.dump();
        flag=!flag;
    }
}

static void kochanowski(){
    char buf[]="Folgujmy paniom nie sobie, ma rada;\n"
               "Miłujmy wiernie nie jest w nich przysada.\n"
               "Godności trzeba nie za nic tu cnota,\n"
               "Miłości pragną nie pragną tu złota.\n"
               "Miłują z serca nie patrzają zdrady,\n"
               "Pilnują prawdy nie kłamają rady.\n"
               "Wiarę uprzejmą nie dar sobie ważą,\n"
               "W miarę nie nazbyt ciągnąć rzemień każą.\n"
               "Wiecznie wam służę nie służę na chwilę,\n"
               "Bezpiecznie wierzcie nierad ja omylę.";
    TextList list;
    for(char*ptr=strtok(buf," ,.;\n");ptr;ptr=strtok(NULL," ,.;\n")){
        list.pushBack(ptr);
    }
    list.dump();
    std::cout<<"-----------------------------------"<<std::endl;
    list.rdump();
}

static void test_copy_assignment(){
    char buf[]="aa bb cc dd ee ff gg hh ii jj kk ll";
    TextList list;
    for(char*ptr=strtok(buf," ,.;\n");ptr;ptr=strtok(NULL," ,.;\n")){
        list.pushBack(ptr);
        list.pushFront(ptr);
    }
    list.dump();
    TextList list2=list;
    list2.dump();

    list2.pushFront("11");
    list2.pushFront("22");
    list2.pushFront("33");
    list2.pushFront("44");
    list = list2;
    list.dump();
}


int main() {
    //testy
    test_push_delete();
    kochanowski();
    test_copy_assignment();
    return 0;
}
