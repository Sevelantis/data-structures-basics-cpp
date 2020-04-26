#include <iostream>
#include <chrono>
#include<cmath>
#include <bitset>
#include <windows.h>
#include <list>
#include<fstream>
#include<conio.h>
#include<cstdio>
using namespace std;

//variable
//time
    __int64 clock_frequency = 0;
    chrono::high_resolution_clock::time_point start_time;
    chrono::high_resolution_clock::time_point stop_time;


const char *data_file[6] = {"data0.txt","data1.txt","data2.txt","data3.txt",
                            "data4.txt","data5.txt"};
const int   data_size[6] = {100,1000,2500,5000,10000,25000};
const int minv = 0, maxv = 100000;
//functions declarations
void display_menu_table(class Tab&);
void display_menu_list(class List&);
void display_menu_heap(class Heap&);
void display_menu_tree(class Tree&);

//-----------------------------------GENERAL USE FUNCTIONS---------------------------------------------------------
//returns seed for generating random numbers
unsigned get_seed()
{
    return chrono::system_clock::now().time_since_epoch().count();
}
//returns random integer from range min max
int get_rand_int(int min, int max)
{
    if(min==max) return max;
    if((min<0   && max<0) || (min>0 &&   max>0))//if min and max are same signs
        if(min  < 0)
            return rand() % (min-max) + min;//negative
        else
            return rand() % (max-min) + min;//positive
    else return rand() % (abs(max)+abs(min)) + min;//if different signs
}
//creates file with random data
void create_rand_data_file(const char* filename, int len)
{
    ofstream f;
    f.open(filename);
    if(f.is_open())
    {
        f << len;
        f << endl;
        for (int i = 0; i < len; i++) {
            f << get_rand_int(minv,maxv);
            if(i==len-1) continue;
            f << endl;
        }
    }else
        cout << "\n\tNie ma takiego pliku.\n";

    f.close();
}
//checks if files are identical
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
        cout << "\n\tNie ma takiego pliku.\n";

}

//---------------------------------------TIMER FUNCTIONS-----------------------------------------------------
void startTimer()
{
    start_time = chrono::high_resolution_clock::now();
}
long stopTimer()
{
    stop_time = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(stop_time-start_time).count();
}

//-----------------------------------------TAB---------------------------------------------------
/*          TABLE CLASS ALLOWS TO:
 *              -read from file and print to file
 *              -show table in console, separating elements with space " ".
 *              -zapelnienie tablicy przypadkowymi wartosciami
 *              -dodawanie/usuwanie pierwszego elementu
 *              -dodawanie/usuwanie ostatniego elementu
 *              -dodawanie/usuwanie elementu na pozycji
 *              -sprawdzenie, czy tablica zawiera wartosc
 *              -dynamiczna relokacje pamieci, zmiana rozmiaru tablicy
 *              -zniszczenie tablicy
 */
class Tab
{
public:
    int length;
    int *tab;
private:
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
    }
    void destroy()
    {
    if(length>0)
        {
            delete[]tab;
            length = 0;
            tab = new int[length];
        }
    }
    void create_rand_tab(int len,int min,int max)
    {
        if(len<=0) cout << "\nNieprawidlowa dlugosc tablicy.";
        else
        {
            length = len;
            tab = new int[length];//dynamically allocated memory
            for(int i = 0 ; i < length ; i++)
                tab[i] = get_rand_int(min, max);
        }
    }
    void read_from_file(const char* datafile)
    {
        ifstream f;
        f.open(datafile);

        if(f.is_open())
        {
            int counter = 0;

            while(!f.eof())
            {
                int val;
                f >> val;
                if(counter != 0)
                    tab[counter-1] = val;
                else                    //first value is total length
                {
                    length = val;
                    tab = new int[length];
                }
                counter++;
            }
        }else
            cout << "\n\tNie ma takiego pliku.\n";

        f.close();
    }
    void print_to_file(const char* output_file)
    {
        if(length == 0) cout << "Tablica jest pusta. Brak danych do zapisania.\n";
        else
        {
            ofstream f;
            f.open(output_file);
            if(f.is_open())
            {
                f << length;
                f << endl;
                for (int i = 0; i < length; i++) {
                    f << tab[i];
                    if(i == length-1) continue; //last line can not be new line
                    f << endl;
                }
            }else
                cout << "\n\tNie ma takiego pliku.\n";
            f.close();
        }
    }

    void print_to_console()
    {
        cout << "Tablica - "<<length <<" elementow\n";
        if(length >0)
        {
            cout << "Wyswietlanie tablicy: \n";
            for(int i=0;i<length;i++)
                cout << tab[i] << " ";

            cout << endl;
        }
    }

    void add_as_last(int val)
    {
        if(!tab)
            tab = new int[length];length++;
        tab = relocate(tab,length); //dynamically change size of table
        tab[length-1] = val;
    }

    void add_as_first(int val)
    {
        tab = relocate(tab,++length);

        for (int i = length-1; i > 0; i--)
            tab[i] = tab[i-1];

        tab[0] = val;
    }

    void insert_at(int index, int val)
    {
        if(index<0 || index > length) cout <<" Nieprawidlowy indeks.\n";
        else
        {
            length++;
            tab = relocate(tab,length);

            for (int i = length-1; i >= index; i--)
                tab[i] = tab[i-1];

            tab[index] = val;
        }
    }

    void delete_last()
    {
        if(length == 0) cout << "Tablica jest pusta - brak elementu do usuniecia\n";
        else
        {
            tab = relocate(tab,--length);
        }
    }

    void delete_first()
    {
        if(length == 0) cout << "Tablica jest pusta - brak elementu do usuniecia\n";
        else
        {
            for(int i = 0   ;  i    <  length;   i++)
                tab[i]=tab[i+1];

            length--;
            tab = relocate(tab,length);
        }
    }

    void delete_at(int index)
    {
        if(index<0 || index > length) cout << "Nieprawidlowy indeks.\n";
        else
        {
            if(length == 0) cout << "Tablica jest pusta - brak elementu do usuniecia\n";
            else
            { for(int i = index   ; i < length; i++)
                    tab[i]=tab[i+1];

                length--;
                tab = relocate(tab,length);
            }
        }
    }

    bool search(int val)
    {
        bool found = false;
        if(length > 0)
        {
            for (int i = 0; i < length; i++)
            {
                if(tab[i]==val)
                {
                    found = true;
                    break;
                }
            }
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
/*
 *      STRUCT ELEMLIST
 *          -holds 32 bits of information (integer)
 *          -element stores pointers to previous and next element
 *          -imagine list as
 *          (head = element1)->element2->...->element(length-2) -> (tail = element (length-1))
 *          head->next...->tail
 *          tail->prev...->head
 */

struct ElemList
{
public:
    int data;
    ElemList *next,*prev;
};
/*          LIST CLASS ALLOWS TO:
 *              -read from file and print to file
 *              -print to console forwards and backwards
 *              -create list made of random elements
 *              -add/delete first element
 *              -add/delete last element
 *              -add/delete element at given index
 *              -searching for value
 *              -destroy list
 */
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
    void create_rand_list(int len, int min, int max)
    {
        for (int i = 0; i < len; i++)
            push_back(get_rand_int(min, max));
    }
    void destroy()
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    void read_from_file(const char* datafile)
    {
        ifstream f;
        f.open(datafile);

        if(f.is_open())
        {
            int counter = 0;
            while(!f.eof())
            {
                int val;
                f >> val;
                if(counter != 0)
                    push_back(val);
                else
                    length = val;
                counter++;
            }
        }else
            cout << "\n\tNie ma takiego pliku.\n";

        f.close();
    }

    void print_to_console()
    {
        cout << "Lista - "<<length<<" elementow.\n";
        if(length>0)
        {
            ElemList *tmp = head;
            //print forwards
            while(tmp)
            {
                cout << tmp->data << " ";
                tmp = tmp->next;
             }
            tmp = tail;
            cout<<endl;
            //print backwards
            while(tmp)
            {
                cout << tmp->data << " ";
                tmp = tmp->prev;
            }
        }
    }

    void print_to_file(const char* outputfile)
    {
        ofstream f;
        f.open(outputfile);
        if(f.is_open())
        {
            auto *tmp = head;
            //first value is list length
            f << length;
            f << endl;
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
            cout << "\n\tNie ma takiego pliku.\n";
        f.close();
    }
    //addLast
    void push_back(int val, bool inc_len = false)
    {
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
        if(inc_len)
            length++;
    }
    //addFirst
    void push_front(int val) {
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
    void push_at(int index, int val)
    {
        if(index < 0 || index > length)
            cout <<"Niepoprawny indeks.\n";
        else {
            auto *tmp_to_insert = new ElemList();
            tmp_to_insert->data = val;

            if (!head) {
                tmp_to_insert->next = nullptr;
                tmp_to_insert->prev = nullptr;
                head = tail = tmp_to_insert;
                length++;
                return;
            }
            auto *tmp_bot = head;

            int counter = 0;
            while (tmp_bot) {
                if (index == counter)
                {
                    auto *tmp_top = tmp_bot->prev;
                    if(tmp_top)
                    {
                        tmp_top->next = tmp_to_insert;
                        tmp_to_insert->prev = tmp_top;
                    }
                    else //else its head
                    {
                        tmp_to_insert->prev = nullptr;
                        head = tmp_to_insert;
                    }
                    tmp_bot->prev = tmp_to_insert;
                    tmp_to_insert->next = tmp_bot;
                    break;
                }
                if(!tmp_bot->next)//its tail
                {
                    tmp_bot->next = tmp_to_insert;
                    tmp_to_insert->prev = tmp_bot;
                    tail = tmp_to_insert;
                    break;
                }
                tmp_bot = tmp_bot->next;
                counter++;
            }
            length++;
        }
    }
    //delet last
    void pop_back()
    {
        if(length==0)
            cout << "Lista jest pusta - brak elementu do usuniecia\n";
        else
        {
            if(length == 1)
            {
                head = nullptr;
                tail = nullptr;
            }
            else if(tail->prev)
            {
                auto*tmp_top = tail->prev;
                tmp_top->next = nullptr;
                tail = tmp_top;
            }
            length--;
        }
    }
    //delet first
    void pop_front()
    {
        if(length==0)
            cout << "Lista jest pusta - brak elementu do usuniecia\n";
        else
        {
            if(length == 1)
            {
                tail = nullptr;
                head = nullptr;
            }
            else if(head->next)
            {
                auto *tmp = head->next;
                tmp->prev= nullptr;
                head = tmp;
            }
            length--;
        }
    }
    //delete at given index
    void pop_at(int index)
    {
        if(index < 0 || index >=length)
            cout << "Nieprawidlowy indeks.\n";
        else {
            if(length!=0){
                if (length == 1 && index == 0) {
                    tail = nullptr;
                    head = nullptr;
                }
                else {
                    auto *tmp_to_del = head;
                    int counter = 0;
                    while (tmp_to_del) {
                        if (counter == index) {
                            ElemList *tmp_top;
                            ElemList *tmp_bot;
                            if (index > 0 && index < length-1) {
                                tmp_top = tmp_to_del->prev;
                                tmp_bot = tmp_to_del->next;
                                if(tmp_bot)
                                {
                                    tmp_bot->prev = tmp_top;
                                    tmp_top->next = tmp_bot;
                                }
                            } else if (index == 0) {
                                tmp_bot = tmp_to_del->next;
                                head = tmp_bot;
                                head->prev = nullptr;
                            } else if (index == length - 1) {
                                tmp_top = tmp_to_del->prev;
                                tail = tmp_top;
                                tail->next = nullptr;
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
    }
    //search for value
    bool search(int val)
    {
        bool found = false;
        if(length!=0)
        {
            auto *tmp = head;
            while(tmp)
            {
                if(tmp->data == val)
                {
                    found = true;
                    break;
                }
                tmp = tmp->next;
            }
        }
        return found;
    }

private:
};//end class list

//---------------------------------------BINARY HEAP - MAXIMUM HEAP-----------------------------------------------------
/*          BINARY HEAP CLASS ALLOWS TO:
 *              -read from file and print to file
 *              -print heap - developer mode
 *              -print heap to console as tree diagram (right side is up, left side is down)
 *              -create heap with random elements
 *              -create heap down, create heap up
 *              -fix heap down, fix heap up
 *              -add key
 *              -delete key
 *              -check if key exists
 *              -not dynamic table relocation after every operation
 *              -swap two elements
 *              -destroy heap
 */
class Heap
{
public:
    int length;
    const string spaces1 = "   ", spaces2 = " ";
private:
    int *tab;

    /*variables used for diagram printing*/
    string symbol_upper, symbol_bottom, symbol_vertical;
public:
    Heap()
    {
        //core
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
        tab = nullptr;
        length = 0;
    }
    void destroy()
    {
        delete []tab;
        tab = nullptr;
        length = 0;
        tab = new int[length];
    }
    //create heap with random elements
    void create_rand_heap(int len, int min,int max)
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
            int counter = 0;
            while(!f.eof())
            {
                int val;
                f >> val;
                if(counter == 0)
                {
                    length = val;
                    tab = new int[length];
                }
                else
                {
                    tab[counter-1] = val;
                }
                counter++;
            }
        }else
            cout << "\n\tNie ma takiego pliku.\n";
        f.close();
    }
    void print_to_file(const char* output_file)
    {
        ofstream f;
        f.open(output_file);
        if(f.is_open())
        {
            //first value is heap length
            f << length;
            f << endl;
            for (int i = 0; i < length; i++) {
                f << tab[i];
                if(i==length-1) continue;
                f<<endl;
            }
        }else
            cout << "\n\tNie ma takiego pliku.\n";
        f.close();
    }
    //print heap - developer mode
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
    void print_tab()
    {
        cout << "Kopiec - " << length << " elementow.\n";
        if(length>0)
        {
            for (int i = 0; i < length; i++)
                cout << tab[i] << " ";
            cout << endl;
        }

    }
    void print_diagram(string spaces1, string spaces2, int index)
    {
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
    //create heap up
    void create_heap_up()
    {
        if(length!=0)
            for(int i=0;i<length;i++)
                fix_up(i);
    }
    //create heap down
    void create_heap_down()
    {
        if(length!=0)
            for(int i=length/2-1;i>=0;--i)
                fix_dn(i);
    }
    //fix heap up
    void fix_up(int child_index)
    {
        /*  while parent is less than son and index is not root
         *      -replace father with son
         *      -set index
         */
        while(child_index != 0 && tab[child_index] > tab[get_parent(child_index)])
        {
            swap(&tab[child_index], &tab[get_parent(child_index)]);
            child_index = get_parent(child_index);
        }
    }
    //fix heap down
    void fix_dn(int parent_index)
    {
        int new_parent = parent_index;
        int left_child = get_left_child(parent_index);
        int right_child = get_right_child(parent_index);

        //if left son is greater than parent
        if(left_child < length && tab[new_parent] < tab[left_child])
            new_parent = left_child;

        //if right son is greater than parent
        if(right_child < length && tab[new_parent] < tab[right_child])
            new_parent = right_child;

        //jif parent is not root
            //swap and fix
        if(new_parent != parent_index)
        {
            swap(&tab[parent_index], &tab[new_parent]);
            fix_dn(new_parent);
        }
    }
    //add key
    void insert_key(int key)
    {
        //insert new key at the end
            //fix heap (if needed)
        length++;
        if(!tab)
            tab = new int[length];
        tab = change_tab_size(tab,length);
        int index = length - 1;
        tab[index] = key;
        fix_up(index);
    }
    //delete key
    void delete_key(int key)
    {
        //find key to delete
        //replace with last element
        //delete last element
        //fix heap in the place where the key was found
        if(length!=0)
        {
            int key_index = search(key);    //search key has index of deleting key
            if(key_index != -1)             //if key not found: it is set to -1
            {
                swap(&tab[key_index],&tab[--length]);
                tab = change_tab_size(tab,length);
                fix_dn(key_index);
            }
        }
    }
    //search for key - returns -1 if not found, else returns index of found element
    int search(int key)
    {
        if(length > 0)
        {
            //if key is greater than root, no need to search
            if(tab[0]<key)
                return -1;//index in array cannot be negative, so treat like bool-false when needed
            else if(tab[0] == key)
                return 0;

            for (int parent_index = 0; parent_index <= length / 2 - 1; parent_index++) {
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
    //getters
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
    //returns array with changed size - NOT USING DYNAMIC RELOCATION
        //- deletes whole array and creates new with fixed size
    inline int* change_tab_size(int* tab , int t_len)
    {
        int* new_tab = new int[t_len];
        copy(tab,tab+t_len , new_tab);
        delete [] tab;
        tab = nullptr;
        return new_tab;
    }
    //swap two elements
    inline void swap(int *x, int*y)
    {
        int tmp = *x;
        *x = *y;
        *y = tmp;
    }
};
//---------------------------------------BINARY SEARCH TREE-----------------------------------------------------
    /*
     *       STRUCTURE NODE OF BINARY TREE:
     *       -holds 4bytes of data - integer
     *       -stores points to: left, right and parent Nodes
     *
     *
     */
struct Node
{
    explicit Node(int k)
    {
        key = k;
        //need to initiate pointers in case of dynamic argument loading
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
    int key;
    Node *left, *right,*parent;
};
/*          BST TREE ALLOWS TO:
 *              -read from file and print to file
 *              -print tree to console as diagram (left side is down, right side is up)
 *              -create tree made of random values
 *              -insert node
 *              -delete node
 *              -search for first existing element using given key
 *              -search for last existing element using given key
 *              -destroy tree
 */
class Tree
{
public:
    //num of elements
    int length;

    //save to file modes
    static const int PRE_ORDER = 0,
                IN_ORDER = 1,
                POST_ORDER=2;

private:
    //tree rot
    Node *root;

    //for diagram printing
    string symbol_upper, symbol_bottom, symbol_vertical;
public:
    Tree()
    {
        length = 0;
        root = nullptr;

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
    ~Tree()
    {
        delete root;
        root = nullptr;
        length = 0;
    }
    void destroy()
    {
        if(length > 0)
        {
            root = nullptr;
            length = 0;
        }
    }
    void create_rand_tree(int len, int min, int max)
    {
        length = len;
        for(int i=0; i<len;i++)
            insert_node(new Node(get_rand_int(min,max)),false);
    }

    void read_from_file(const char *datafile)
    {
        ifstream f;
        f.open(datafile);

        if(f.is_open())
        {
            int key;
            f >> key;
            length = key;
            int cc = 0;
            while(!f.eof())
            {
                f >> key;
                insert_node(new Node(key),false);
                cc++;
                if(cc == length) break; //dont read last line
            }
        }else
            cout << "\n\tNie ma takiego pliku.\n";
        f.close();
    }
    string print_node(Node *node)
    {
        string tmp;
        tmp.append(to_string(node->key));
        return tmp;
    }
    void print_diagram(string spaces1, string spaces2, Node *root_)
    {
        string out;

        if (root_)
        {
            out = spaces1;

            if(spaces2 == symbol_bottom)
                out[out.length()-1] = '|';

            if(root_->right)
                print_diagram(out + symbol_vertical, symbol_upper, root_->right);

            out = out.substr(0, spaces1.length()-1);
            cout << out << spaces2 << root_->key << endl;
            out = spaces1;

            if(spaces2 == symbol_upper)
                out[out.length()-1] = '|';

            if(root_->left)
                print_diagram(out + symbol_vertical, symbol_bottom, root_->left);
            out = out.substr(0, spaces1.length());
//            out = spaces1;
        }
    }
    void print_to_file(const char *output_file, int MODE)
    {
        ofstream f;
        f.open(output_file);
        if(f.is_open())
        {
            //first value is heap length
            f << length;
            f << endl;
            if(MODE==IN_ORDER)
                in_order_print(root, &f);
            else if(MODE==PRE_ORDER)
                pre_order_print(root, &f);
            else if(MODE==POST_ORDER)
                post_order_print(root, &f);
        }
        else
            cout << "\n\tNie ma takiego pliku.\n";
        f.close();
    }
    void print_num_of_elements()
    {
        cout << "Drzewo - " << length << " elementow.\n";
    }
    void pre_order_print(Node *node, ofstream*_f)
    {
        if(!node) return;
        *_f << print_node(node) << "\n";
        pre_order_print(node->left,_f);
        pre_order_print(node->right,_f);
    }
    void in_order_print(Node *node, ofstream*_f)
    {
        if(!node) return;
        in_order_print(node->left,_f);
        *_f << print_node(node) << "\n";
        in_order_print(node->right,_f);
    }
    void post_order_print(Node *node, ofstream*_f)
    {
        if(!node) return;
        post_order_print(node->left,_f);
        post_order_print(node->right,_f);
        *_f << print_node(node) << "\n";
    }
    void pre_order(Node*node)
    {
        if(!node) return;
        cout << print_node(node) << " ";
        pre_order(node->left);
        pre_order(node->right);
    }
    void in_order(Node *node)
    {
        if(!node) return;
        in_order(node->left);
        cout << print_node(node) << " ";
        in_order(node->right);
    }
    void post_order(Node *node)
    {
        if(!node) return;
        post_order(node->left);
        post_order(node->right);
        cout << print_node(node) << " ";
    }
    //add node, in case of manual adding set inc_length to true to increase total length of tree.
    void insert_node(Node* node_insert, bool inc_length = false)
    {
        if(inc_length)
            length++;
        Node* parent;
        Node* tmp = root;
        while(tmp)
        {
            parent = tmp;
            if(node_insert->key < parent->key)
                tmp = tmp->left;
            else
                tmp = tmp->right;
        }

        if(!root)
            root = node_insert;
        else if(node_insert->key < parent->key)
        {
            node_insert->parent = parent;
            parent->left = node_insert;
        }
        else
        {
            node_insert->parent = parent;
            parent->right = node_insert;
        }
    }
    //delete node
    void delete_node(Node *root_, int key, bool dec_len = false)
    {
        if(!root_)
        {
            cout <<"Drzewo jest puste. Nie ma czego usuwac.\n";
            return;
        }
        //find LAST node to delete
        Node* node_delete = search_iterative(root_, key);

        if(node_delete) //if found
        {
            if(dec_len) length--;
            //if node is leaf
            if(!node_delete->right && !node_delete->left)
            {
                auto *parent = node_delete->parent;
                //check if node is right or left son
                    //unlink from parent
                if(parent)
                {
                    if(node_delete == parent->left)
                        parent->left = nullptr;

                    else if(node_delete == parent->right)
                        parent->right = nullptr;
                }
                else
                {
                    delete root_;
                    root_ = nullptr;
                }
            }
            //when node has both sons
            else if(node_delete->right && node_delete->left)
            {
                auto *parent = node_delete->parent;
                //find minimum in right subtree
                //or
                //find maximum in left subtree

                Node* node_to_replace = nullptr;
                //if right son exists, search right
                if(node_delete->right)
                    node_to_replace = get_min(node_delete->right);
                else//there may be left only
                    node_to_replace = get_max(node_delete->left);

                delete_node(node_delete, node_to_replace->key);
                node_delete->key = node_to_replace->key;
            }
            //node has only ONE son
            else
            {
                auto *parent = node_delete->parent;
                //check if its root
                if(!parent)
                {
                    Node* node_to_replace = nullptr;
                    if(node_delete->right)
                        node_to_replace = get_min(node_delete->right);
                    else
                        node_to_replace = get_max(node_delete->left);

                    delete_node(node_delete, node_to_replace->key);
                    node_delete->key = node_to_replace->key;
                }
                else
                {
                    //check if its right or left
                    if (node_delete == parent->left) {
                        //check if it has right or left son
                        //link to new parent
                        if (node_delete->left)//left left
                        {
                            parent->left = node_delete->left;
                            node_delete->left->parent = parent;
                        } else//left right...
                        {
                            parent->left = node_delete->right;
                            node_delete->right->parent = parent;
                        }
                    } else if (node_delete == parent->right) {
                        if (node_delete->left) {
                            parent->right = node_delete->left;
                            node_delete->left->parent = parent;
                        } else {
                            parent->right = node_delete->right;
                            node_delete->right->parent = parent;
                        }
                    }
                }
            }
        }
    }
    //find first matching Node
    Node* search(Node*root_, int key)
    {
        if(!root_)
            return nullptr;
        if(root_->key == key)
            return root_;
        if(key < root_->key)              //szukaj w lewym poddrzewie
            return search(root_->left, key);
        return search(root_->right, key);  //szukaj w prawym poddrzewie
    }
    //find last matching Node
    static Node* search_iterative(Node*root_, int key)
    {
        if(!root_)
            return nullptr;
        //if key found
        if(root_->key == key)
        {
            auto* node_before = root_;
            //check if theres more of keys...

            if(key < root_->key)
                root_ = search_iterative(root_->left,key);
            else
                root_ = search_iterative(root_->right,key);
            if(!root_)
                root_ = search_iterative(node_before->left,key);

            if(!root_)
            {
                return node_before;
            }else//if theres more, root_ return it
                return root_;
        }
        if(key < root_->key)//search left subtree
            return search_iterative(root_->left, key);
        return search_iterative(root_->right, key);
    }
    //getters
    Node* get_root()
    {
        return root;
    }
    Node* get_min(Node*root_)
    {
        auto *tmp = root_;
        while(tmp->left)
            tmp = tmp->left;
        return tmp;
    }
    Node* get_max(Node*root_)
    {
        auto *tmp = root_;
        while(tmp->right)
            tmp = tmp->right;
        return tmp;
    }
    //same as length of tree (Tree class public field)
    int get_height(Node* root_){
        int height = 0 ;
        if(!root_)
            return 0;

        auto*tmp = root_;
        while(tmp)
        {
            tmp = tmp->right;
            if(tmp)
                height++;
        }
        return height;
    }
    //traverse tree to linear
    void flatten(const char* file)
    {
        print_to_file(file,IN_ORDER);
        destroy();
        read_from_file(file);
//        remove(file);
    }
    //traverse left rotation
    Node* rotate_left(Node *tmp) {
        auto*tmp_r = tmp->right;
        if (tmp->parent)
            tmp->parent->right = tmp_r;
        tmp_r->parent = tmp->parent;
        if (!tmp_r->parent)
            root = tmp_r;
        tmp->parent = tmp_r;
        if (tmp_r->left)
            tmp_r->left->parent = tmp;
        tmp->right = tmp_r->left;
        tmp_r->left = tmp;
        return tmp_r;
    }
    //balancing BST tree using DSW algorithm - NEED LINEAR
    // REPRESENTATION OF TREE TO WORK PROPERLY (function void flatten)
    void create_balanced_tree()
    {
        int m = pow(2,log2(length + 1)) - 1;
        auto*tmp = root;
        while(m!=1)
        {
            for (int i = 0; i < length - m; i++) {
                if(tmp)
                {
                    if(tmp->right)
                    {
                        tmp = rotate_left(tmp);
                        tmp = tmp->right;
                    }
                    else break;
                }
            }
            m/=2;
            //go back to root
            tmp = root;
        }
        //one more rotation
        if(tmp->right)
            root = rotate_left(tmp);
    }
private:
};


//---------------------------------------MAIN-----------------------------------------------------
int main(int argc, char**argv)
{
    srand(time_t(get_seed()));

    bool running = true;
    char choice;
    Tab  *tab  = new Tab();
    List *list = new List();
    Heap *heap = new Heap();
    Tree *tree = new Tree();

    while(running)
    {
        cout <<"--- MENU GLOWNE ---\n";
        cout << "Wybierz strukture:\n1. Tablica\n2. Lista\n3. Kopiec\n4. Drzewo BST\n0. Wyjscie z programu\n";
        cout <<"--- MENU GLOWNE ---\n";
        cout << "User choice:";
        cin >> choice;
        switch(choice)
        {
            case '1':
                startTimer();
                display_menu_table(*tab);
                cout << stopTimer()/1000000.0<<"ms\n\n";
                break;
            case '2':
                display_menu_list(*list);
                break;
            case '3':
                display_menu_heap(*heap);
                break;
            case '4':
                display_menu_tree(*tree);
                break;
            case '0':
                running = false;
                break;
            default:
                break;
        }
    }
    system("pause");
    return 0;
}

//-------------------------------------MENU------------------------------------------
void display_menu_tree(Tree &tree)
{
    char opt;
    string filename;
    int val=0, len=0;
    bool running = true;
    while(running)
    {
        cout << endl;
        cout << "--- DRZEWO BST---" << endl;
        cout << "1.Zbuduj z pliku" << endl;
        cout << "2.Stworz losowo" << endl;
        cout << "3.Dodaj wierzcholek" << endl;
        cout << "4.Usun wierzcholek" << endl;
        cout << "5.Znajdz wierzcholek" << endl;
        cout << "6.Wyswietl" << endl;
        cout << "7.Usun drzewo" << endl;
        cout << "0.Powrot do menu" << endl;
        cout << "s.Zapisz do pliku" << endl;
        cout << "--- DRZEWO BST---" << endl;
        cout << "User choice:";
        cin >> opt;
        switch (opt)
        {
            case '1': //wczytaj z pliku
                cout << "Podaj nazwe zbioru do wczytania: ";
                cin >> filename;
                tree.destroy();
                tree.read_from_file(filename.c_str());
                if(tree.length>0)
                {
                    tree.flatten("flaten.txt");
                    tree.create_balanced_tree();
                }
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                break;
            case '2':   //stworz losowo
                cout << "Podaj dlugosc: ";
                cin >> len;
                tree.destroy();
                tree.create_rand_tree(len,minv,maxv);
                if(tree.length>0)
                {
                    tree.flatten("flaten.txt");
                    tree.create_balanced_tree();
                }
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                break;
            case '3'://dodaj wierzcholek
                cout <<"Podaj wartosc: ";
                cin >> val;
                tree.insert_node(new Node(val),true);
                if(tree.length>0)
                {
                    tree.flatten("flaten.txt");
                    tree.create_balanced_tree();
                }
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                break;
            case '4'://usun wierzcholek
                cout <<"Podaj wartosc: ";
                cin >> val;
                tree.delete_node(tree.get_root(), val ,true);
                if(tree.length>0)
                {
                    tree.flatten("flaten.txt");
                    tree.create_balanced_tree();
                }
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                break;
            case '5'://znajdz element
                cout << "Podaj wartosc: ";
                cin >> val;
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                if(tree.search(tree.get_root(), val))
                    cout << "Element jest w liscie.\n";
                else
                    cout << "Elementu nie ma w liscie.\n";
                break;
            case '6'://wyswietl
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                break;
            case '7'://usun drzewo
                tree.destroy();
                tree.print_num_of_elements();
                tree.print_diagram("   "," ",tree.get_root());
                break;
            case '0':
                running = false;
                break;
            case 's'://zapisz do pliku
                cout <<"Podaj nazwe pliku do utworzenia: ";
                cin >> filename;
                tree.print_to_file(filename.c_str(),Tree::PRE_ORDER);
                break;
            default:
                break;
        }
    }
}

void display_menu_heap(Heap &heap)
{
    char opt;
    string filename;
    int val=0, len=0;
    bool running = true;
    while(running)
    {
        cout << endl;
        cout << "--- KOPIEC ---" << endl;
        cout << "1.Zbuduj z pliku" << endl;
        cout << "2.Stworz losowo" << endl;
        cout << "3.Dodaj klucz" << endl;
        cout << "4.Usun klucz" << endl;
        cout << "5.Znajdz klucz" << endl;
        cout << "6.Wyswietl" << endl;
        cout << "7.Usun kopiec" << endl;
        cout << "0.Powrot do menu" << endl;
        cout << "s.Zapisz do pliku" << endl;
        cout << "--- KOPIEC ---" << endl;
        cout << "User choice:";
        cin >> opt;
        switch (opt)
        {
            case '1': //wczytaj z pliku
                cout << "Podaj nazwe zbioru do wczytania: ";
                cin >> filename;
                heap.destroy();
                heap.read_from_file(filename.c_str());
                heap.create_heap_up();
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                break;
            case '2':   //stworz losowo
                cout << "Podaj dlugosc: ";
                cin >> len;
                heap.destroy();
                heap.create_rand_heap(len,minv,maxv);
                heap.create_heap_up();
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                break;
            case '3'://dodaj klucz
                cout <<"Podaj wartosc: ";
                cin >> val;
                heap.insert_key(val);
                heap.print_tab();
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                break;
            case '4'://usun klucz
                cout <<"Podaj wartosc: ";
                cin >> val;
                heap.delete_key(val);
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                break;
            case '5'://znajdz element
                cout << "Podaj wartosc: ";
                cin >> val;
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                if(heap.search(val) != -1)
                    cout << "Element jest w liscie.\n";
                else
                    cout << "Elementu nie ma w liscie.\n";
                break;
            case '6'://wyswietl
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                break;
            case '7'://usun kopiec
                heap.destroy();
                heap.print_tab();
                heap.print_diagram("   "," ",0);
                break;
            case '0':
                running = false;
                break;
            case 's'://zapisz do pliku
                cout <<"Podaj nazwe pliku do utworzenia: ";
                cin >> filename;
                heap.print_to_file(filename.c_str());
                break;
            default:
                break;
        }
    }
}
void display_menu_list(List &list)
{
    char opt;
    string filename;
    int index=0, val=0, len=0;
    bool running = true;
    while(running)
    {
        cout << endl;
        cout << "--- LISTA ---" << endl;
        cout << "1.Zbuduj z pliku" << endl;
        cout << "2.Stworz losowa liste" << endl;
        cout << "3.Dodaj element na pozycje" << endl;
        cout << "4.Usun element z pozycji" << endl;
        cout << "5.Znajdz element" << endl;
        cout << "6.Wyswietl" << endl;
        cout << "7.Usun liste" << endl;
        cout << "0.Powrot do menu" << endl;
        cout << "s.Zapisz do pliku" << endl;
        cout << "--- LISTA ---" << endl;
        cout << "User choice:";
        cin >> opt;
        switch (opt)
        {
            case '1': //wczytaj z pliku
                cout << "Podaj nazwe zbioru do wczytania: ";
                cin >> filename;
                list.destroy();
                list.read_from_file(filename.c_str());
                list.print_to_console();
                break;
            case '2':   //stworz losowa tablice
                cout << "Podaj dlugosc: ";
                cin >> len;
                list.destroy();
                list.create_rand_list(len,minv,maxv);
                list.print_to_console();
                break;
            case '3'://dodaj element na pozycje
                cout <<"Podaj indeks: ";
                cin >> index;
                cout <<"Podaj wartosc: ";
                cin >> val;
                list.push_at(index, val);
                list.print_to_console();
                break;
            case '4'://usun element na pozycji
                cout <<"Podaj indeks: ";
                cin>> index;
                list.pop_at(index);
                list.print_to_console();
                break;
            case '5'://znajdz element
                cout << "Podaj wartosc: ";
                cin >> val;
                list.print_to_console();
                if(list.search(val))
                    cout << "Element jest w liscie.\n";
                else
                    cout << "Elementu nie ma w liscie.\n";
                break;
            case '6'://wyswietl
                list.print_to_console();
                break;
            case '7'://usun tablice
                list.destroy();
                list.print_to_console();
                break;
            case 's'://zapisz do pliku
                cout <<"Podaj nazwe pliku do utworzenia: ";
                cin >> filename;
                list.print_to_file(filename.c_str());
                break;
            case '0':
                running = false;
                break;
            default:
                break;
        }
    }
}

void display_menu_table(Tab &tab)
{
    char opt;
    string filename;
    int index=0, val=0, len=0;
    bool running = true;
    while (running)
    {
        cout << endl;
        cout << "--- TABLICA ---" << endl;
        cout << "1.Zbuduj z pliku" << endl;
        cout << "2.Stworz losowa tablice" << endl;
        cout << "3.Dodaj element na pozycje" << endl;
        cout << "4.Usun element z pozycji" << endl;
        cout << "5.Znajdz element" << endl;
        cout << "6.Wyswietl" << endl;
        cout << "7.Usun tablice" << endl;
        cout << "0.Powrot do menu" << endl;
        cout << "s.Zapisz do pliku" << endl;
        cout << "--- TABLICA ---" << endl;
        cout << "User choice:";
        cin >> opt;
        switch (opt){
            case '1': //wczytaj z pliku
                cout << "Podaj nazwe zbioru do wczytania: ";
                cin >> filename;
                tab.destroy();
                tab.read_from_file(filename.c_str());
                tab.print_to_console();
                break;
            case '2':   //stworz losowa tablice
                cout << "Podaj dlugosc: ";
                cin >> len;
                tab.destroy();
                tab.create_rand_tab(len,minv,maxv);
                tab.print_to_console();
                break;
            case '3'://dodaj element na pozycje
                cout <<"Podaj indeks: ";
                cin >> index;
                cout <<"Podaj wartosc: ";
                cin >> val;
                tab.insert_at(index,val);
                tab.print_to_console();
                break;
            case '4'://usun element na pozycji
                cout <<"Podaj indeks: ";
                cin>> index;
                tab.delete_at(index);
                tab.print_to_console();
                break;
            case '5'://znajdz element
                cout << "Podaj wartosc: ";
                cin >> val;
                tab.print_to_console();
                if(tab.search(val))
                    cout <<"Element jest w tablicy.\n";
                else
                    cout <<"Elementu nie ma w tablicy.\n";
                break;
            case '6'://wyswietl
                tab.print_to_console();
                break;
            case '7'://usun tablice
                tab.destroy();
                tab.print_to_console();
                break;
            case '0':
                running = false;
                break;
            case 's'://zapisz do pliku
                cout <<"Podaj nazwe pliku do utworzenia: ";
                cin >> filename;
                tab.print_to_file(filename.c_str());
                break;
            default:
                break;
        }
    }
}