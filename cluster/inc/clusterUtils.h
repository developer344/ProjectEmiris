#ifndef _CLUSTER_UTILS_H_
#define _CLUSTER_UTILS_H_
#include <string>
#include <vector>
#include "../../lib/mathUtils.h"
#include "../../lib/projectUtils.h"

typedef struct ClusterStruct *ClusterPtr;
typedef struct ClusterStruct
{
    PointPtr centroidPoint;
    std::vector<PointPtr> points;
    int size;
    double silhouette;
} Cluster;

typedef struct ClusterDataStruct
{

    std::string inputFileName,
        configFileName,
        outputFileName,
        methodName;

    int method;

    bool complete;

    int number_of_clusters,
        number_of_vector_hash_tables,
        number_of_vector_hash_functions,
        max_number_M_hypercube,
        number_of_hypercube_dimensions,
        number_of_probes;

    int dimension;

} inputData;

double silhouette_calculator(PointPtr point, std::vector<Cluster> clusters, int dimensions);
std::vector<int> get_2_closest_clusters(PointPtr point, std::vector<Cluster> clusters, int dimensions);

#endif