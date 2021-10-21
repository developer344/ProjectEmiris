#include <iostream>
#include <string>
#include <vector>
#include "hashTable.h"
using namespace std;

int main(int argc, char **argv)
{
    // vector<HashTable> hashTables;
    // hashTables.resize(100);
    // for (int i = 0; i < 100; i++)
    // {
    //     gg = new HashTable(100);
    //     hashTables.push_back(*gg);
    // }
    // cout << "Num of hash tables " << hashTables.size() << endl;
    // for (int i = 0; i < 100; i++)
    // {
    //     delete (&hashTables[i]);
    // }

    string str = "this\tis a\tstring with spaces";
    string word = "";
    for (char x : str)
    {
        if (x == '\t')
        {
            cout << word << endl;
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    cout << word << endl;

    string a = "hello";
    string b = a;

    a = "hi";

    cout << a << "," << b << endl;
}