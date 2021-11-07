#ifndef _HC_HASHTABLE_H_
#define _HC_HASHTABLE_H_

#include <stdlib.h>
#include <string>
#include <vector>

#include "projectUtils.h"

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