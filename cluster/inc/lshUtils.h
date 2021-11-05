#ifndef _LSH_UTILS_H_
#define _LSH_UTILS_H_

#include <string>
#include "mathUtils.h"
#include "hashTable.h"

typedef struct inputDataStruct
{
    std::string inputFileName;
    std::string queryFileName;
    std::string outputFileName;
    int numberOfHyperplanes; // numberOfHyperplanes
    int intL;
    int numberOfNearest;
    int radius;
    int dimension;
} inputData;

void sort_neighbours(kNeighboursPtr k_nearest_neighbours, int k_neighbours);
void sort_points(vector<PointPtr> *Data);
int notAlreadyExists(kNeighboursPtr k_nearest_neighbours, string pointID);
kNeighboursPtr find_k_true_neighbours(PointPtr queryPoint, int k_neighbours, vector<PointPtr> inputPoints, int dimension);

#endif