#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
#include<cmath>
#include <random>
#include <bitset>
#include <windows.h>
#include <list>
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>

using namespace std;

//variable
__int16 _min=0, _max=50;
    //time
    __int64 processor_frequency = 0;
    double time_stamp_0 = .0;

const char *data_file[8] = {"data0.txt","data1.txt","data2.txt","data3.txt",
                            "data4.txt","data5.txt","data6.txt","data7.txt"};
const int   data_size[8] = {1000,2000,5000,10000,15000,20000,50000,1000000};

//-----------------------------------GENERAL USE FUNCTIONS---------------------------------------------------------
unsigned get_seed()
{
    return chrono::system_clock::now().time_since_epoch().count();
}
/*funkcja do generowania losowej liczby ca³kowitej z przedzia³u min max*/
int get_rand_int(__int16 min, __int16 max)
{
    if(min==max) return max;
    if((min<0   && max<0) || (min>0 &&   max>0))//jezeli min i max sa tych samych znakow
        if(min  < 0)
            return rand() % (min-max) + min;//ujemne
        else
            return rand() % (max-min) + min;//dodatnie
    else return rand() % (abs(max)+abs(min)) + min;//jezeli sa roznych znakow
}

void init_text_file(const char* filename, int len)
{
    ofstream f;
    f.open(filename);
    if(f.is_open())
    {
        for (int i = 0; i < len; i++) {
            f << get_rand_int(-100,100);
            if(i==len-1) continue;
            f << endl;
        }
    }else
        cout << "\n\tError opening file while init_text_file\n";

    f.close();
}

void diff_files(const char* filename1, const char* filename2)
{
    ifstream f1,f2;
    f1.open(filename1);
    f2.open(filename2);

    if(f1.is_open() && f2.is_open())
    {
        if(f1.tellg() == f2.tellg()) // same length
        {
            f1.seekg(0,ifstream::beg);
            f2.seekg(0,ifstream::beg);
            bool equal = std::equal(istreambuf_iterator<char>(f1.rdbuf()),
                                    istreambuf_iterator<char>(),
                                    istreambuf_iterator<char>(f2.rdbuf()));
            if(equal)
                cout << "\n\tFiles: "<<filename1<<" && " << filename2 << " are the same.\n";
            else
                cout << "\n\tFiles: "<<filename1<<" && " << filename2 << " __DIFF__\n";
        } else
            cout << "\n\tFiles: "<<filename1<<" && " << filename2 << " __DIFF__LENGTH__\n";
    } else
        cout << "\n\tError opening files in diff_files\n";

}


//---------------------------------------TIMER FUNCTIONS-----------------------------------------------------
void startTimer()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "B³¹d!\n";

    processor_frequency = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    time_stamp_0 = li.QuadPart;
}

void stopTimer()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    cout << "\nOperacja zajela: " << (li.QuadPart - time_stamp_0) / processor_frequency << " milisekund" << endl;
}

//-----------------------------------------TAB---------------------------------------------------
class Tab
{
public:
    int length;
private:
    int *tab;

public:

    Tab()
    {
        length = 0;
        tab = new int[length];
    }

    ~Tab()
    {
        length = 0;
        delete[]tab;
        tab=nullptr;
    }

    void init_rand_values(int len)
    {
        length = len;
        tab = new int[length];
        for(int i = 0 ; i < length ; i++)
        {
            tab[i] = get_rand_int(-100, 100);
        }
    }

    void clear()
    {
        if(length ==0 )cout << "Tablica jest pusta - zakonczono czyszczenie.\n";
        else
        {
            delete this;
            cout << "Zniszczono tablice.\n";
        }
    }

    void read_from_file(const char* datafile)
    {
        ifstream f;
        f.open(datafile);

        if(f.is_open())
        {
            clear();
            while(!f.eof())
            {
                int val;
                f >> val;
                add_as_last(val);
            }
        }else
            cout << "\n\tError opening file while TAB read_from_file\n";

        f.close();
    }

    void print_to_file(const char* outputfile)
    {
        ofstream f;
        f.open(outputfile);
        if(f.is_open())
        {
            int counter=0;
            for (int i = 0; i < length; i++) {
                f << tab[i];
                if(counter==length-1) continue;
                f<<endl;
            }
        }else
            cout << "\n\tError opening file while TAB print_to_file\n";
    }

    void print()
    {
        if(length == 0) cout << "Tablica jest pusta\n";
        else
        {
            cout << "Elementy tablicy: \n";
            for(int i=0;i<length;i++)
            {
                cout << tab[i] << " ";
//                cout << "tab[" << i << "]\t->\t&"<<&tab[i]<<"\t->\t"<<tab[i]<<endl;
            }
            cout << endl;
        }
    }

    void add_as_last(int val)
    {
        cout << "Dodawanie elementu z tylu tablicy:\n\tdodawany element: "<< val <<"\n";
        length++;
        tab = relocate(tab,length);
        tab[length-1] = val;
    }

    void add_as_first(int val)
    {
        cout << "Dodawanie elementu z przodu tablicy:\n\tdodawany element: "<< val <<"\n";
        length++;
        tab = relocate(tab,length);

        for (int i = length-1; i > 0; i--)
        {
            tab[i] = tab[i-1];
        }
        tab[0] = val;
    }

    void insert_at(int index, int val)
    {
        cout << "Dodawanie elementu na wybrana pozycjê w tablicy:\n\twybrany indeks: " << index
             << "\n\tdodawany element: " << val << "\n";
        length++;
        tab = relocate(tab,length);

        for (int i = length-1; i > index; i--)
        {
            tab[i] = tab[i-1];
        }
        tab[index] = val;
    }

    void delete_last()
    {
        if(length == 0) cout << "Tablica jest pusta - brak elementu do usuniecia\n";
        else
        {
            cout << "Usuwanie ostatniego elementu.\n";
            tab = relocate(tab,--length);
        }
    }

    void delete_first()
    {
        if(length == 0) cout << "Tablica jest pusta - brak elementu do usuniecia\n";
        else
        {
            cout << "Usuwanie pierwszego elementu\n";
            for(int i = 0   ;  i    <  length;   i++)
            {
                tab[i]=tab[i+1];
            }
            length--;
            tab = relocate(tab,length);
        }
    }

    void delete_at(int index)
    {
        if(length == 0) cout << "Tablica jest pusta - brak elementu do usuniecia\n";
        else
        {
            cout << "Usuwanie elementu na pozycji:\n\twybrany indeks: " << index << endl;
            for(int i = index   ; i < length; i++)
            {
                tab[i]=tab[i+1];
            }
            length--;
            tab = relocate(tab,length);
        }
    }

    bool search(int val)
    {
        bool found = false;
        if(length==0)cout << "Tablica jest pusta - zakonczono wyszukiwanie.\n";
        else
        {
            cout << "Wyszukiwanie elementu w tablicy.\n\twybrany element: " <<val;
            for (int i = 0; i < length; i++)
            {
                if(tab[i]==val)
                {
                    found = true;
                    break;
                }
            }
            if(found)
                cout << "\nElement jest w tablicy.\n";
            else
                cout << "\nElementu nie ma w tablicy.\n";
        }
        return found;
    }

private:
    inline int* relocate(int *t, int t_len)
    {
        return (int*) realloc(t, t_len * sizeof(int));
    }
private:

};//end class Tab
//---------------------------------------LIST-----------------------------------------------------
struct ElemList
{
public:
    int data;
    ElemList *next,*prev;
};

class List
{
public:
    int length;
private:
    ElemList *head,*tail;
public:
    List()
    {
        head = tail = nullptr;
        length = 0;
    }

    ~List()
    {
        auto *tmp = head;

        while(tmp)
        {
            tmp = tmp->next;
            delete head;
            head = tmp;
        }
        length = 0;
    }

    void init_rand_values(int length)
    {
        for (int i = 0; i < length; i++)
            push_back(get_rand_int(-100, 100));
    }

    void clear()
    {
        if(length==0) cout << "Lista jest pusta - zakonczono czyszczenie.\n";
        else
        {
            delete this;
            cout << "Zniszczono liste.\n";
        }
    }

    void read_from_file(const char* datafile)
    {
        ifstream f;
        f.open(datafile);

        if(f.is_open())
        {
            clear();
            while(!f.eof())
            {
                int val;
                f >> val;
                push_back(val);
            }
        }else
            cout << "\n\tError opening file while LIST read_from_file\n";

        f.close();
    }

    void print()
    {
        if(length == 0) cout << "Lista jest pusta.\n";
        else
        {
            ElemList *tmp = head;
            cout << "Elementy listy:\n";
            int counter = 0;
            while(tmp)
            {
//                cout << "Element["<<counter<<"]\tval: " << tmp->data<<endl;
                cout << tmp->data << " ";
                tmp = tmp->next;
                counter++;
             }
            tmp = tail;
            cout<<endl;
            while(tmp)
            {
                cout << tmp->data << " ";
                tmp = tmp->prev;
            }
            cout<<endl;
        }
    }

    void print_to_file(const char* outputfile)
    {
        ofstream f;
        f.open(outputfile);
        if(f.is_open())
        {
            auto *tmp = head;
            int counter=0;
            while(tmp)
            {
                f << tmp->data;
                tmp = tmp->next;
                if(counter==length-1) continue;
                f<<endl;
                counter++;
            }
        }else
            cout << "\n\tError opening file while LIST print_to_file\n";
    }
    //addLast
    void push_back(int val)
    {
        cout << "Dodawanie elementu z tylu listy:"
             << "\n\tdodawany element:"<< val <<"\n";
        auto *tmp = new ElemList();

        tmp->data = val;
        if(tail)
        {
            tmp->prev = tail;
            tmp->next = nullptr;
            tail->next = tmp;
        }else
        {
            head = tmp;
        }
        tail = tmp;
        length++;
    }
    //addFirst
    void push_front(int val) {
        cout << "Dodawanie elementu z przodu listy:"
             << "\n\tdodawany element: " << val << "\n";
        auto *tmp = new ElemList();

        tmp->next = head;
        tmp->prev = nullptr;

        if (head)
            head->prev = tmp;
        head = tmp;
        if (!tail) tail = head;

        tmp->data = val;
        length++;
    }
    //addRand
    void insert_at(int index, int val)
    {
        cout << "Dodawanie elementu na wybrana pozycje listy:\n\twybrany indeks: " << index
             << "\n\tdodawany element: " << val << "\n";
        auto *tmp_to_insert = new ElemList();
        tmp_to_insert->data = val;

        if(!head)
        {
            tmp_to_insert->next= nullptr;
            tmp_to_insert->prev=nullptr;
            head = tail = tmp_to_insert;
            length++;
            return;
        }

        auto *tmp_bot = head;

        int counter = 0;
        while(tmp_bot)
        {
            if(index == counter)
            {
                auto *tmp_top = tmp_bot->prev;
                tmp_top->next = tmp_to_insert;
                tmp_to_insert->prev = tmp_top;
                tmp_bot->prev = tmp_to_insert;
                tmp_to_insert->next = tmp_bot;
                break;
            }
            tmp_bot = tmp_bot->next;
            counter++;
        }
        length++;
    }

    void pop_back()
    {
        if(length==0)
            cout << "Lista jest pusta - brak elementu do usuniecia\n";
        else
        {
            cout << "Usuwanie elementu z tylu listy.\n";
            if(length == 1)
            {
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                auto*tmp_top = tail->prev;
                tmp_top->next = nullptr;
                tail = tmp_top;
            }
            length--;
        }
    }

    void pop_front(){
        if(length==0)
            cout << "Lista jest pusta - brak elementu do usuniecia\n";
        else
        {
            cout << "Usuwanie elementu z przodu listy.\n";
            if(length == 1)
            {
                tail = nullptr;
                head = nullptr;
            }
            else
            {
                auto *tmp = head->next;
                tmp->prev= nullptr;
                head = tmp;
            }
            length--;
        }
    }

    void pop_at(int index)
    {
        if(length==0)
            cout << "Lista jest pusta - brak elementu do usuniecia\n";
        else
        {
            cout << "Usuwanie elementu na pozycji w liscie."
                 << "\n\tWybrany index: " << index << endl;
            if(length==1)
            {
                tail = nullptr;
                head = nullptr;
            }
            else
            {
                auto *tmp_to_del = head;
                int counter = 0;
                while(tmp_to_del)
                {
                    if(counter == index)
                    {
                        ElemList *tmp_top;
                        ElemList *tmp_bot;
                        if(index>0 && index < length-1)
                        {
                            tmp_top = tmp_to_del->prev;
                            tmp_bot = tmp_to_del->next;
                            tmp_bot->prev = tmp_top;
                            tmp_top->next = tmp_bot;
                        }
                        else if(index == 0)
                        {
                            tmp_bot = tmp_to_del->next;
                            head = tmp_bot;
                            head->prev = nullptr;
                        }
                        else if(index == length-1)
                        {
                            tmp_top = tmp_to_del->prev;
                            tail = tmp_top;
                            tail->next=nullptr;
                        }
                        break;
                    }
                    tmp_to_del = tmp_to_del->next;
                    counter++;
                }
            }
            length--;
        }
    }

    bool search(int val)
    {
        bool found = false;
        if(length ==0 ) cout << "Lista jest pusta - zakonczono wyszukiwanie.\n";
        else
        {
            auto *tmp = head;
            cout << "Wyszukiwanie elementu w liscie.\n\tWybrany element: " << val;
            while(tmp)
            {
                if(tmp->data == val)
                {
                    found = true;
                    break;
                }
                tmp = tmp->next;
            }
            if(found)
                cout << "\nElement jest w liscie.\n";
            else
                cout << "\nElementu nie ma w liscie.\n";
        }
        return found;
    }

private:
};//end class list


//---------------------------------------BINARY HEAP-----------------------------------------------------
/*          KLASA KOPCA BINARNEGO
 *
 *          klasa struktury kopca pozwala na:
 *              -stworzenie kopca w gore i w dol
 *              -naprawa kopca w gore i w dol
 *              -wyczyszczenie kopca
 *              -dodawanie kluczy
 *              -usuwanie kluczy
 *              -sprawdzenie, czy klucz istnieje
 *              -czytanie z pliku i zapis do pliku
 *
 */
class Heap
{
public:
    int length;
    const string spaces1 = "   ", spaces2 = " ";
private:
    int *tab;

    string symbol_upper, symbol_bottom, symbol_vertical;
public:
    Heap()
    {
        length = 0;
        tab = new int[length];

        /*variables used for diagram printing*/
        symbol_vertical[0]=' ';
        symbol_upper = symbol_bottom = symbol_vertical = "        ";
        symbol_upper[0] = '/';
        symbol_bottom[0]= '\\';
        for(int i=1;i<=5;i++)
        {
            symbol_upper[i] ='~';
            symbol_bottom[i]='~';
        }
    }
    ~Heap()
    {
        delete []tab;
        length = 0;
        tab = nullptr;
    }
    void clear()
    {
        if(length ==0 )cout << "Kopiec jest pusty - zakonczono niszczenie.\n";
        else
        {
            delete this;//call destructor
            cout << "Zniszczono kopiec.\n";
        };
    }
    //inicjuje tablice przypadkowych wartosci dla kopca
    void init_rand_values(int len, int min,int max)
    {
        length = len;
        tab = new int[length];
        for(int i = 0 ; i < length ; i++)
            tab[i] = get_rand_int(min, max);
    }
    void read_from_file(const char *datafile)
    {
        ifstream f;
        f.open(datafile);
        if(f.is_open())
        {
            clear();
            length=0;
            while(!f.eof())
            {
                length++;
                tab = change_tab_size(tab,length);
                f>> tab[length-1];
            }
        }else
            cout << "\n\tError opening file while HEAP read_from_file\n";

        f.close();
    }
    //drukuje kopiec w celach testowych
    void print()
    {
        //indeksy nowych poziomów
        cout <<endl;
        int n=22;
        int *breaks = new int[n];
        for(int i=0;i<n;i++)
            breaks[i] = pow(2,i+1)-1;//1,3,7,15,31,63...

        for (int i = 0,break_it = 0;i < length; i++) {
            if(i==breaks[break_it])
            {
                cout << "\n";
                break_it++;
            }
            cout << tab[i] <<" ";
        }
        cout <<endl;
        delete[]breaks;
        breaks=nullptr;
    }
    //drukuje kopiec w stylu diagramu
    void print_diagram(string spaces1, string spaces2, int index)
    {
        if (length == 0)
            cout << "Kopiec jest pusty." << endl;
        string out;

        if (index < length)
        {
            int child_right = 2*index+2;
            int child_left  = 2*index+1;
            out = spaces1;

            if(spaces2 == symbol_bottom)
                out[out.length()-1] = '|';

            print_diagram(out + symbol_vertical, symbol_upper, child_right);

            out = out.substr(0, spaces1.length()-1);
            cout << out << spaces2 << tab[index] << endl;
            out = spaces1;

            if(spaces2 == symbol_upper)
                out[out.length()-1] = '|';

            print_diagram(out + symbol_vertical, symbol_bottom, child_left);
            out = out.substr(0, spaces1.length());
//            out = spaces1;
        }
    }
    void print_to_file(const char* outputfile)
    {
        ofstream f;
        f.open(outputfile);
        if(f.is_open())
        {
            int counter=0;
            for (int i = 0; i < length; i++) {
                f << tab[i];
                if(counter==length-1) continue;
                f<<endl;
            }
        }else
            cout << "\n\tError opening file while HEAP print_to_file\n";
    }
    //stworz kopiec w gore.
    void create_heap_up()
    {
        if(length == 0 ) cout << "Kopiec jest pusty - zakonczono tworzenie w gore.\n";
        else
            for(int i=0;i<length;i++)
                fix_up(i);
    }
    //stworz kopiec w dol.
    void create_heap_dn()
    {
        if(length == 0 ) cout << "Kopiec jest pusty - zakonczono tworzenie w dol.\n";
        else
            for(int i=length/2-1;i>=0;--i)
                fix_dn(i);
    }
    //naprawa kopca w gore
    void fix_up(int child_index)
    {
        //powtarzaj gdy ojciec jest mniejszy od syna i indeks nie jest korzeniem:
            //zamien ojca z synem miejscami
            //ustaw indeks 'poziom wyzej'
        while(child_index != 0 && tab[child_index] > tab[get_parent(child_index)])
        {
            swap(&tab[child_index], &tab[get_parent(child_index)]);
            child_index = get_parent(child_index);
        }
    }
    //naprawa kopca w dol
    void fix_dn(int parent_index)
    {
        int new_parent = parent_index;
        int left_child = get_left_child(parent_index);
        int right_child = get_right_child(parent_index);

        //jesli lewy syn jest wiekszy od ojca
        if(left_child < length && tab[new_parent] < tab[left_child])
            new_parent = left_child;

        //jesli prawy syn jest wiekszy od ojca
        if(right_child < length && tab[new_parent] < tab[right_child])
            new_parent = right_child;

        //jesli ojciec nie jest korzeniem
            //zamien i napraw
        if(new_parent != parent_index)
        {
            swap(&tab[parent_index], &tab[new_parent]);
            fix_dn(new_parent);
        }
    }
    //dodanie klucza
    void insert_key(int key)
    {
        //wstaw nowy klucz na koniec
            //napraw (jesli jest koniecznosc)
        length++;
        tab = change_tab_size(tab,length);
        int index = length - 1;
        tab[index] = key;
        fix_up(index);
    }
    //usuniecie klucza
    void delete_key(int key)
    {
        //znajdz klucz do usuniecia
        //zamien miejscami z ostatnim elementem
        //usun ostatni element
        //napraw kopiec w miejscu gdzie poprzednio byl klucz
        if(length==0) cout << "Kopiec jest pusty - brak mozliwosci usuwania.\n";
        else
        {
            //sprawdz czy istnieje
            int key_index = search(key);    //search key zawiera indeks usuwanego klucza
            if(key_index!=-1)
            {
                swap(&tab[key_index],&tab[--length]);
//                length--;
                tab = change_tab_size(tab,length);
                fix_dn(key_index);
                cout <<"\nusunieto element o indeksie : "<<key_index<<"\n";
            }
        }
    }
    //wyszukiwanie klucza - zwraca -1 w przypadku niepowodzenia
                            //oraz indeks klucza w przypadku powodzenia
    int search(int key)
    {
        if(length == 0) cout << "Kopiec jest pusty - nie mozna kontynuowac wyszukiwania\n";
        else
        {
            //jesli klucz jest wiekszy niz korzen to nie ma co szukac...
            if(tab[0]<key)
                return -1;//indeks nie moze byc ujemny wiec mozna to potraktowac jak false

            for (int parent_index = 0; parent_index <= length / 2 - 1; parent_index++) {
                //jesli klucz jest wiekszy niz ojciec, nie trzeba sprawdzac jego synow
                if(tab[parent_index]<key) continue;
                else if(tab[parent_index]==key)
                    return parent_index;
                else if(tab[get_left_child(parent_index)]==key)
                    return get_left_child(parent_index);
                else if(tab[get_right_child(parent_index)]==key)
                    return get_right_child(parent_index);
            }
        }
        return -1;
    }
private:
    //zwraca ojca syna
    inline int get_parent(int child_index)
    {
        return (child_index-1)/2;
    }
    inline int get_left_child(int parent_index)
    {
        return parent_index*2+1;
    }
    inline int get_right_child(int parent_index)
    {
        return parent_index*2+2;
    }
    //zwraca tablice ze zmienionym rozmiarem nie uzywajac relokacji
    inline int* change_tab_size(int* tab , int t_len)
    {
        int* new_tab = new int[t_len];
        copy(tab,tab+t_len,new_tab);
        delete [] tab;
        tab = nullptr;
        return new_tab;
    }
    //zamiana wartosci ojca z synem
    inline void swap(int *x, int*y)
    {
        cout <<"[zamiana " << *y << " <-> " << *x<<"]"<<endl;//do testow
        int tmp = *x;
        *x = *y;
        *y = tmp;
    }
};

//---------------------------------------MAIN-----------------------------------------------------
int main() {
    srand(time_t(get_seed()));
    int data_mode = 0;
//    init_text_file(data_file[data_mode], data_size[data_mode]);

    Heap heap;
    heap.init_rand_values(44,0,7);
    heap.create_heap_dn();
    heap.print_diagram("  "," ",0);
    heap.delete_key(6);
    heap.print_diagram("  "," ",0);

    return 0;

    bool running = true;
    while(running)
    {
        int choice;
        const string infoMsg = "Choose structure:\n1 tablica\n2 lista\n3 kopiec\n4 drzewo";
        cin >> choice;
        switch(choice)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;
        }
    }
    return 0;
}