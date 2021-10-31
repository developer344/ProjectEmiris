#ifndef _HC_HASHTABLE_H_
#define _HC_HASHTABLE_H_

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#define W (3)

typedef struct PointStruct *PointPtr;
typedef struct BucketStruct *BucketPtr;
typedef struct NeighbourStruct *NeighbourPtr;
typedef struct kNeighboursStruct *kNeighboursPtr;

typedef struct PointStruct
{
    std::string id;
    std::vector<double> coords;
} Point;

typedef struct BucketStruct
{
    std::vector<PointPtr> points;
} Bucket;

typedef struct NeighbourStruct
{
    PointPtr point;
    double dist;
} Neighbour;

typedef struct kNeighboursStruct
{
    std::vector<NeighbourPtr> neighbours;
    int size; // number of requested (k) nearest neighbours
} kNeighbours;

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
};

#endif