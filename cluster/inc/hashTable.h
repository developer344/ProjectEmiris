#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdlib.h>
#include <vector>
#include <string>
#include <random>

#include "../../lib/projectUtils.h"

// #define BIGM (4294967291)
// #define W (3)
// using namespace std;

// typedef struct PointStruct *PointPtr;
// typedef struct BucketStruct *BucketPtr;
// typedef struct NeighbourStruct *NeighbourPtr;
// typedef struct kNeighboursStruct *kNeighboursPtr;

// typedef struct PointStruct
// {
//     string id;
//     std::vector<double> coords;
// } Point;

// typedef struct BucketStruct
// {
//     std::vector<PointPtr> points;
//     std::vector<long int> ID;

// } Bucket;

// typedef struct NeighbourStruct
// {
//     PointPtr point;
//     double dist;
// } Neighbour;

// typedef struct kNeighboursStruct
// {
//     std::vector<NeighbourPtr> neighbours;
//     int size; // number of requested (k) nearest neighbours
// } kNeighbours;

// struct BY_ID
// {
//     // Source: https://stackoverflow.com/questions/2999135/how-can-i-sort-the-vector-elements-using-members-as-the-key-in-c
//     bool operator()(PointPtr const &a, PointPtr const &b) const
//     {
//         return a->id < b->id;
//     }
// };

// struct BY_ID_INT
// {
//     // Source: https://stackoverflow.com/questions/2999135/how-can-i-sort-the-vector-elements-using-members-as-the-key-in-c
//     bool operator()(PointPtr const &a, PointPtr const &b) const
//     {
//         return stoi(a->id) < stoi(b->id);
//     }
// };

typedef struct LSHinputDataStruct
{
    std::string inputFileName;
    std::string queryFileName;
    std::string outputFileName;
    int numberOfHyperplanes; // numberOfHyperplanes
    int intL;
    int numberOfNearest;
    int radius;
    int dimension;
} LSHinputData;

class HashTables
{
private:
    int numOfHashTables;
    int numberOfHyperplanes, numberOfPoints, TableSize, dim;
    std::vector<std::vector<Bucket>> hash_tables;
    std::vector<std::vector<int>> ri; // r=(-100,100)
    std::vector<std::vector<double>> t;
    std::vector<std::vector<std::vector<double>>> v;

public:
    HashTables(int L, int numberOfHyperplanes, int numberOfPoints, int dimension, int tableSize); // Constructor
    void InsertPoint(PointPtr point);
    int HashFunc(PointPtr point, int hashtableId);
    void PrintHashTables();
    kNeighboursPtr find_k_nearest_neighbours(PointPtr queryPoint, int k_neighbours);
    std::vector<PointPtr> range_search(PointPtr queryPoint, double range, std::vector<std::string> *foundPoints = NULL);
};

#endif