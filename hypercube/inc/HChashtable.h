#ifndef _HC_HASHTABLE_H_
#define _HC_HASHTABLE_H_

#include <stdlib.h>
#include <string>
#include <vector>

#include "mathUtils.h"
#include "hypercubeUtils.h"

#define W (3)

typedef struct PointStruct *PointPtr;
typedef struct BucketStruct *BucketPtr;

typedef struct PointStruct
{
    std::string id;
    std::vector<double> coords;
} Point;

typedef struct BucketStruct
{
    std::vector<PointPtr> points;
} Bucket;

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
    int maxcantidatesPoints;
    int bucketCount;

public:
    HChashTable(int dimension,
                int projectionDimension,
                int probes,
                int maxcantidatesPoints);
    ~HChashTable();
    void Insert(PointPtr p);
};

#endif