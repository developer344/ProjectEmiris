#include <algorithm>
#include <iterator>
#include <limits.h>

#include "methods.h"
#include "mathUtils.h"
// #include "../../lsh/inc/hashTable.h"
//#include "../../hypercube/inc/HChashTable.h"

// Returns index of closest centroid point, after checking all points
int lloyd_method(std::vector<PointPtr> centroidPoints, PointPtr point, int dimension)
{
    double min_dist = INT32_MAX;
    double cur_dist = 0;
    int index = 0;

    for (int i = 0; i < centroidPoints.size(); i++)
    {
        cur_dist = euclideanDistance(point, centroidPoints[i], dimension);
        if (cur_dist < min_dist)
        {
            min_dist = cur_dist;
            index = i;
        }
    }
    return index;
}

void old_lsh_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints)
{

    HashTables HashTablesObject(CLData->number_of_vector_hash_tables, CLData->number_of_vector_hash_functions, numOfInputPoints, CLData->dimension, numOfInputPoints / 8);

    for (int i = 0; i < numOfInputPoints; i++)
        HashTablesObject.HashTables::InsertPoint((*inputPoints)[i]);

    double currRadius = minDistBetweenCentroids(centroids, CLData->number_of_clusters, CLData->dimension) / 2;
    vector<vector<PointPtr>> clusterPoints;
    vector<PointPtr> duplicates;
    clusterPoints.resize(CLData->number_of_clusters);
    while (1)
    {
        for (int c = 0; c < CLData->number_of_clusters; c++)
            clusterPoints[c] = HashTablesObject.range_search((*centroids)[c], currRadius);

        duplicates = find_duplicates(clusterPoints, CLData->number_of_clusters);

        for (PointPtr currPoint : duplicates)
        {
            vector<int> CentroidsToBeErased;
            vector<int> position;
            for (int c = 0; c < CLData->number_of_clusters; c++)
            {
                for (int p = 0; p < clusterPoints[c].size(); p++)
                {
                    if (currPoint->id == clusterPoints[c][p]->id)
                    {
                        CentroidsToBeErased.push_back(c);
                        position.push_back(p);
                    }
                }
            }
            double minDist = INT_MAX;
            int minCentroid;
            double currDist = 0.0;
            int currCentroid;
            for (int i = 0; i < CentroidsToBeErased.size(); i++)
            {
                currCentroid = i;
                currDist = euclideanDistance((*centroids)[currCentroid], currPoint, CLData->dimension);
                if (currDist < minDist)
                {
                    minDist = currDist;
                    minCentroid = currCentroid;
                }
            }
            CentroidsToBeErased.erase(CentroidsToBeErased.begin() + minCentroid);
            position.erase(position.begin() + minCentroid);
            for (int i = 0; i < CentroidsToBeErased.size(); i++)
            {
                clusterPoints[CentroidsToBeErased[i]].erase(clusterPoints[CentroidsToBeErased[i]].begin() + position[i]);
            }
        }
        for (int c = 0; c < CLData->number_of_clusters; c++)
            for (int pointIndex = 0; pointIndex < clusterPoints.size(); pointIndex++)
                (*clusters)[c].points.push_back(clusterPoints[c][pointIndex]);
        clusterPoints.clear();
    }
    currRadius *= 2;
}

void lsh_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints)
{

    HashTables HashTablesObject(CLData->number_of_vector_hash_tables, CLData->number_of_vector_hash_functions, numOfInputPoints, CLData->dimension, numOfInputPoints / 8);

    for (int i = 0; i < numOfInputPoints; i++)
        HashTablesObject.HashTables::InsertPoint((*inputPoints)[i]);

    double currRadius = minDistBetweenCentroids(centroids, CLData->number_of_clusters, CLData->dimension) / 2;
    vector<vector<PointPtr>> clusterPoints;
    vector<PointPtr> duplicates;
    clusterPoints.resize(CLData->number_of_clusters);
    while (currRadius < 1000)
    {
        for (int c = 0; c < CLData->number_of_clusters; c++)
            clusterPoints[c] = HashTablesObject.range_search((*centroids)[c], currRadius);

        //duplicates = find_duplicates(clusterPoints, CLData->number_of_clusters);

        // for (PointPtr currPoint : duplicates)
        // {
        //     vector<int> CentroidsToBeErased;
        //     vector<int> position;
        //     for (int c = 0; c < CLData->number_of_clusters; c++)
        //     {
        //         for (int p = 0; p < clusterPoints[c].size(); p++)
        //         {
        //             if (currPoint->id == clusterPoints[c][p]->id)
        //             {
        //                 CentroidsToBeErased.push_back(c);
        //                 position.push_back(p);
        //             }
        //         }
        //     }
        //     double minDist = INT_MAX;
        //     int minCentroid;
        //     double currDist = 0.0;
        //     int currCentroid;
        //     for (int i = 0; i < CentroidsToBeErased.size(); i++)
        //     {
        //         currCentroid = i;
        //         currDist = euclideanDistance((*centroids)[currCentroid], currPoint, CLData->dimension);
        //         if (currDist < minDist)
        //         {
        //             minDist = currDist;
        //             minCentroid = currCentroid;
        //         }
        //     }
        //     CentroidsToBeErased.erase(CentroidsToBeErased.begin() + minCentroid);
        //     position.erase(position.begin() + minCentroid);
        //     for (int i = 0; i < CentroidsToBeErased.size(); i++)
        //     {
        //         clusterPoints[CentroidsToBeErased[i]].erase(clusterPoints[CentroidsToBeErased[i]].begin() + position[i]);
        //     }
        // }
        for (int c = 0; c < CLData->number_of_clusters; c++){
            for (int pointIndex = 0; pointIndex < clusterPoints.size(); pointIndex++){
                (*clusters)[c].points.push_back(clusterPoints[c][pointIndex]);
                (*clusters)[c].size++;}
            clusterPoints[c].clear();
        }
        currRadius *= 2;
    }
}

double minDistBetweenCentroids(std::vector<PointPtr> *centroidPoints, int numOfCentroids, int dimension)
{
    double minDist = INT_MAX;
    double currDist = 0.0;

    for (int i = 0; i < numOfCentroids; i++)
    {
        for (int j = 0; j < i; j++)
        {
            currDist = euclideanDistance((*centroidPoints)[i], (*centroidPoints)[j], dimension);
            if (currDist < minDist)
                minDist = currDist;
        }
    }
    return minDist;
}

vector<PointPtr> find_duplicates(vector<vector<PointPtr>> clusterPoints, int numOfClusters)
{
    vector<PointPtr> mainArray;
    vector<PointPtr> duplPoints;

    for (int i = 0; i < numOfClusters - 1; i++)
    {
        std::merge((clusterPoints)[i].begin(), (clusterPoints)[i].end(), (clusterPoints)[i + 1].begin(), (clusterPoints)[i + 1].end(), std::back_inserter(mainArray), BY_ID());
    }

    PointPtr currPoint = mainArray[0];
    if(mainArray[0]!=NULL){
        int i = 1;
        while (i < mainArray.size())
        {
            if (currPoint->id == mainArray[i]->id)
            {
                duplPoints.push_back(currPoint);
                while (i < mainArray.size())
                {
                    if (currPoint->id != mainArray[++i]->id)
                        break;
                }
                continue;
            }
            currPoint = mainArray[i];
            i++;
        }
    }
    return duplPoints;
}