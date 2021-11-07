#ifndef _HC_HASHTABLE_H__
#define _HC_HASHTABLE_H__

#include <stdlib.h>
#include <string>
#include <vector>

#include "../../lib/projectUtils.h"

// using namespace std;

// #define W (3)

// typedef struct PointStruct *PointPtr;
// typedef struct BucketStruct *BucketPtr;
// typedef struct NeighbourStruct *NeighbourPtr;
// typedef struct kNeighboursStruct *kNeighboursPtr;

// typedef struct PointStruct
// {
//     std::string id;
//     std::vector<double> coords;
// } Point;

// typedef struct BucketStruct
// {
//     std::vector<PointPtr> points;
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

typedef struct HCinputDataStruct
{
    std::string inputFileName;
    std::string queryFileName;
    std::string outputFileName;
    int projectionDimension;
    int maxCandidatePoints;
    int probes;
    int numberOfNearest;
    int radius;
    int dimension;
} HCinputData;

class HChashTable
{
private:
    std::vector<Bucket *> Table;
    std::vector<int> ri; // r=(-100,100)
    std::vector<double> t;
    std::vector<std::vector<double>> v;
    int dimension;
    int projectionDimension;
    int probes;
    int maxcandidatesPoints;
    unsigned long bucketCount;

public:
    HChashTable(int dimension,
                int projectionDimension,
                int probes,
                int maxcandidatesPoints);
    ~HChashTable();
    void InsertPoint(PointPtr p);
    unsigned long HashFunc(PointPtr point);
    kNeighboursPtr find_k_nearest_neighbours(PointPtr queryPoint, int k_neighbours);
    std::vector<unsigned long> *find_n_hamming_distance(unsigned long currBucketValue, int hammingDistance);
    std::vector<PointPtr> range_search(PointPtr queryPoint, double range, std::vector<std::string> *foundPoints = NULL);
};

#endif