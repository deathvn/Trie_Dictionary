#include <bits/stdc++.h>
using namespace std;

const int ALPHABET_SIZE = 26;
typedef char text[500];
long cur_offset=0; // Luu offset cuoi cung

// Data Struct for Trie Tree
struct Node
{
    long offset = -1;
    Node* child [ALPHABET_SIZE] = {NULL};
};

void addNode(Node *root, char key[50], long off = -1)
{
    Node* p = root;
    for (int i=0; key[i] != '\0' ; ++i)
    {
        int id = key[i] - 'a';
        if (! p->child[id])
            p->child[id] = new Node;
        p = p->child[id];
    }
    p->offset = off;
}

int findNode(Node *root, char key[50])
{
    Node* p = root;
    for (int i=0; key[i] != '\0' ; ++i)
    {
        int id = key[i] - 'a';
        if (! p->child[id])
            return -1;
        p = p->child[id];
    }
    if (!p) return -1;
    return p->offset;
}


// Data Struct for Index
struct row
{
    char key [50];
    long offset;
};

//Index load from file
//vector <row> Index;

void writeIndex(char *Filename)
{
    FILE *f = fopen(Filename, "ab");
    row xx;
    int n;
    cout<<"Number of (key, offset) you want to add: ";
    cin>>n;
    fflush(stdin);
    for (int i=0; i<n; ++i)
    {
        gets(xx.key);
        xx.offset = cur_offset;
        fwrite(&xx, sizeof(xx), 1, f);
        cur_offset = cur_offset + 500;
    }
    fclose(f);
}

long readIndex(char *Filename, Node* root)
{
    FILE *f = fopen(Filename, "rb");
    row xx;
    xx.offset = 0;
    fread(&xx, sizeof(xx), 1, f);
    int i=1;
    while (!feof(f))
    {
        cout<<i++<<" : "<<xx.key<<" |---| "<<xx.offset<<endl;
        addNode(root, xx.key, xx.offset);
        fread(&xx, sizeof(xx), 1, f);
    }
    fclose(f);
    return xx.offset;
}

//Driver for Word on Dictionary
void addWord(char *idFile, char *dicFile, Node *root, char word[50])
{
    if (findNode(root, word) >= 0 )     // if word in Tree, return
        {
            cout<<"The word : "<<word<<" : has been in dictionary"<<endl;
            return;
        }

    addNode(root, word, cur_offset);    // addNode word, cur_offset to Tree

    //Add word and offset to Index file
    FILE *f = fopen(idFile, "ab");
    row xx;
    xx.offset = cur_offset;
    strcpy(xx.key, word);
    fwrite(&xx, sizeof(xx), 1, f);
    fclose(f);

     //Add word and explain text to Dictionary file
    FILE *fdic = fopen(dicFile, "ab");
    text S;
    cout<<"Explain text for "<<word<<" : \n";
    fflush(stdin);
    gets(S);
    //fseek(fdic, cur_offset, 0);
    fwrite(&S, sizeof(S), 1, fdic);
    //cout<<endl<<cur_offset<<endl<<S<<endl;
    fclose(fdic);

    cur_offset += 500;
}

void editExplain (char *dicFile, long offset)
{
    cout<<"Type new Explain text: "<<endl;
    fflush(stdin);
    text T;
    gets(T);
    FILE *f = fopen(dicFile, "r+");
    fseek(f, offset, 0);
    fwrite(&T, sizeof(T), 1, f);
    fclose(f);
    cout<<"Succed editing"<<endl;
}

void findWord(char *dicFile, Node *root, char word[50])
{
    long offset = findNode(root, word);
    if (offset < 0)
    {
        cout<<"The Word : "<<word<<" : not Found"<<endl;
        return;
    }
    text T;
    FILE* fdic = fopen(dicFile, "rb");
    //cout<<offset<<endl;
    fseek(fdic, offset, 0);
    fread(&T, sizeof(T), 1, fdic);
    fclose(fdic);
    cout<<T<<endl;

    char kk;
    cout<<"Do you want to editing explain text ? (Y/N): "; cin>>kk;
    if (kk == 'Y' || kk == 'y')
        editExplain(dicFile, offset);
}

void delWord(char *Filename, char key[50],Node* root)
{
    Node* p = root;
    for (int i=0; key[i] != '\0' ; ++i)
    {
        int id = key[i] - 'a';
        if (! p->child[id])
            {cout<<"Not Found"<<endl; return;}
        p = p->child[id];
    }
    if (!p) {cout<<"Not Found"<<endl; return;}

    if (p->offset < 0) {cout<<"Not Found"<<endl; return;}

    if (p->offset == 0)
        p->offset = -1;
    else
        p->offset = (p->offset)*(-1);
}

int main()
{
    char *Filename ="Index.bin";
    char *Dictionary = "Dict.bin";

    // Constructer for building files
    /*writeIndex(Filename);
    text T = "A namespace is designed to overcome this difficulty and is used.";
    FILE *fdic = fopen(Dictionary, "ab");
    fwrite(&T, sizeof(T), 1, fdic);
    fclose(fdic);*/

    //Greate root for Trie Tree
    Node* root = new Node;
    cur_offset = readIndex(Filename, root) + 500;


    // Input data for Dictionary
    /*int n=25;
    char word[50];
    for (int i=0; i<n; ++i)
    {
        cout<<"New word: ";
        cin>>word;
        addWord(Filename, Dictionary, root, word);
        cout<<"----------------------"<<endl;
    }

    cur_offset = readIndex(Filename, root);

    findWord(Dictionary, root, "app");*/

    /*text T;
    FILE* fdic = fopen(Dictionary, "rb");
    fseek(fdic, cur_offset -500, 0);
    fread(&T, sizeof(T), 1, fdic);
    fclose(fdic);
    cout<<T<<endl;*/

    int k;
    do
    {
        cout<<"__________________________________________"<<endl;
        cout<<"1. Add new Word"<<endl;
        cout<<"2. Search"<<endl;
        cout<<"3. Delete word"<<endl;
        cout<<"4. Quit"<<endl;
        cin>>k;
        if (k == 1)
        {
            char word[50];
            cout<<"Enter new word here: "; cin>>word;
            addWord(Filename, Dictionary, root, word);
        }
        else if (k == 2)
        {
            char word[50];
            cout<<"Search for: "; cin>>word;
            findWord(Dictionary, root, word);
        }
        else if (k==3)
        {
            char word[50];
            cout<<"Enter which word you want to delete: "; cin>>word;
            delWord(Filename, word, root);
        }
        else break;
        cout<<"__________________________________________"<<endl;
    } while (1);


    return 0;
}
