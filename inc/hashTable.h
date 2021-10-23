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
typedef struct BucketStruct *BucketPtr;

typedef struct PointStruct
{
    string id;
    vector<double> coords;
} Point;

typedef struct BucketStruct
{
    vector<PointPtr> points;
    vector<int> ID;

} Bucket;

class HashTables
{
private:
    int numOfHashTables;
    int numberOfHyperplanes, numberOfPoints, TableSize, dim;
    vector<vector<Bucket>> hash_tables;
    vector<vector<int>> ri; // r=(0-100)
    vector<vector<double>> t;
    vector<vector<vector<double>>> v;

public:
    HashTables(int L, int numberOfHyperplanes, int numberOfPoints, int dimension); // Constructor
    void InsertPoint(PointPtr point);
    int HashFunc(PointPtr point, int hashtableId);
    void PrintHashTables();
    string k_nearest_neighbours(PointPtr queryPoint, int k_neighbours);
};

#endif
