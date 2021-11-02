#ifndef _METHODS_H_
#define _METHODS_H_

typedef struct duplStruct
{
    PointPtr point;
    vector<int> clusters;
} duplicatePoint;

int lloyd_method(std::vector<PointPtr> centroidPoints, PointPtr point, int dimension);
void lsh_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData);
#endif