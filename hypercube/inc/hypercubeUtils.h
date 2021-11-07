#ifndef _HYPERCUBE_UTILS_H_
#define _HYPERCUBE_UTILS_H_

#include <string>
#include "mathUtils.h"
#include "HChashTable.h"

#define DEF_K 14
#define DEF_M 10
#define DEF_PROBES 2
#define DEF_N 1
#define DEF_R 500

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
    bool distance_true_visible;
} inputData;

std::vector<std::string> get_lines(std::string fileName);
int get_points(vector<std::string> linesVector, vector<PointPtr> *pointVector);
void sort_neighbours(kNeighboursPtr k_nearest_neighbours, int k_neighbours);
void sort_points(std::vector<PointPtr> *Data);
int notAlreadyExists(kNeighboursPtr k_nearest_neighbours, std::string pointID);
kNeighboursPtr find_k_true_neighbours(PointPtr queryPoint,
                                      int k_neighbours,
                                      std::vector<PointPtr> inputPoints,
                                      int dim);
bool mapFunction(int h, int i);
inputData *getInputData(int *argc, char **argv);
int writeToOutput(inputData *LSHData,
                  std::vector<PointPtr> queryPoints,
                  vector<kNeighboursPtr> queryOutputData,
                  vector<kNeighboursPtr> queryTrueNeighbors,
                  vector<vector<PointPtr>> queryRangeSearch,
                  vector<double> tLSH,
                  vector<double> tTrue);
void deleteData(std::vector<PointPtr> *inputPoints,
                std::vector<PointPtr> *queryPoints,
                vector<vector<Neighbour> *> *k_nearest_neighbours,
                vector<kNeighboursPtr> *queryOutputData,
                vector<kNeighboursPtr> *queryTrueNeighbors);

std::string checkRerun();

#endif