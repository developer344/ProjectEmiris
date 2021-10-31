#ifndef _HYPERCUBE_UTILS_H_
#define _HYPERCUBE_UTILS_H_

#include <string>
#include "mathUtils.h"
#include "HChashTable.h"

using namespace std;

typedef struct inputDataStruct
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
} inputData;

void sort_neighbours(kNeighboursPtr k_nearest_neighbours, int k_neighbours);
void sort_points(std::vector<PointPtr> *Data);
int notAlreadyExists(kNeighboursPtr k_nearest_neighbours, std::string pointID);
kNeighboursPtr find_k_true_neighbours(PointPtr queryPoint,
                                      int k_neighbours,
                                      std::vector<PointPtr> inputPoints,
                                      int dim);
bool mapFunction(int h, int i);

#endif