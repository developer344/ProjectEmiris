#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdlib.h>
#include <vector>
#include <string>
#include <random>

#define BIGM (4294967291)
#define W (3)
using namespace std;

typedef struct PointStruct *PointPtr;
typedef struct BucketStruct *BucketPtr;
typedef struct NeighbourStruct *NeighbourPtr;
typedef struct kNeighboursStruct *kNeighboursPtr;

typedef struct PointStruct
{
    string id;
    vector<double> coords;
} Point;

typedef struct BucketStruct
{
    vector<PointPtr> points;
    vector<long int> ID;

} Bucket;

typedef struct NeighbourStruct
{
    PointPtr point;
    double dist;
} Neighbour;

typedef struct kNeighboursStruct
{
    vector<NeighbourPtr> neighbours;
    int size; // number of requested (k) nearest neighbours
} kNeighbours;

struct BY_ID
{
    // Source: https://stackoverflow.com/questions/2999135/how-can-i-sort-the-vector-elements-using-members-as-the-key-in-c
    bool operator()(PointPtr const &a, PointPtr const &b) const
    {
        return a->id < b->id;
    }
};

class HashTables
{
private:
    int numOfHashTables;
    int numberOfHyperplanes, numberOfPoints, TableSize, dim;
    vector<vector<Bucket>> hash_tables;
    vector<vector<int>> ri; // r=(-100,100)
    vector<vector<double>> t;
    vector<vector<vector<double>>> v;

public:
    HashTables(int L, int numberOfHyperplanes, int numberOfPoints, int dimension, int tableSize); // Constructor
    void InsertPoint(PointPtr point);
    int HashFunc(PointPtr point, int hashtableId);
    void PrintHashTables();
    kNeighboursPtr find_k_nearest_neighbours(PointPtr queryPoint, int k_neighbours);
    vector<PointPtr> range_search(PointPtr queryPoint, double range);
};

#endif
