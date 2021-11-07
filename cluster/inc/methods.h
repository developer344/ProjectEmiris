#ifndef _METHODS_H_
#define _METHODS_H_

#include <vector>
#include "hashTable.h"
#include "HChashTable.h"
#include "clusterUtils.h"
#include "../../lib/projectUtils.h"
typedef struct duplStruct
{
    PointPtr point;
    std::vector<int> clusters;
} duplicatePoint;

int lloyd_method(std::vector<PointPtr> *centroidPoints, PointPtr point, int dimension);
void lsh_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints);
void hyperCube_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints);
double minDistBetweenCentroids(std::vector<PointPtr> *centroidPoints, int numOfCentroids, int dimension);
std::vector<PointPtr> find_duplicates(std::vector<std::vector<PointPtr>> clusterPoints, int numOfClusters);
#endif