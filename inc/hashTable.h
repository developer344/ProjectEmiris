#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdlib.h>
#include <vector>
#include <string>
#include <random>

#define BIGM (4294967291)
#define W (4)
using namespace std;

typedef struct PointStruct *PointPtr;

typedef struct PointStruct
{
    string id;
    vector<double> coords;
} Point;

class HashTables
{
private:
    int numOfHashTables;
    int k, n, TableSize, dim;
    vector<vector<vector<Point *>>> hash_tables;
    vector<vector<int>> ri; //r=(0-100)
    vector<vector<double>> t;
    vector<vector<vector<double>>> v;

public:
    HashTables(int L, int k, int n, int dimension); // Constructor
    void InsertPoint(PointPtr point);
    int HashFunc(PointPtr point, int hashtableId);
    void PrintHashTables();
    ~HashTables(); // Destructor
};

#endif
