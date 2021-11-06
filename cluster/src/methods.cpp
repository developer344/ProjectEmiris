#include <algorithm>
#include <unistd.h>
#include <iterator>
#include <limits.h>

#include "methods.h"
#include "mathUtils.h"
// #include "../../lsh/inc/hashTable.h"
//#include "../../hypercube/inc/HChashTable.h"

// Returns index of closest centroid point, after checking all points
int lloyd_method(std::vector<PointPtr> *centroidPoints, PointPtr point, int dimension)
{
    double min_dist = INT32_MAX;
    double cur_dist = 0;
    int index = 0;

    for (int i = 0; i < centroidPoints->size(); i++)
    {
        cur_dist = euclideanDistance(point, (*centroidPoints)[i], dimension);
        if (cur_dist < min_dist)
        {
            min_dist = cur_dist;
            index = i;
        }
    }
    return index;
}

void lsh_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints)
{
    vector<PointPtr> foundPoints;
    // Adding centroids to found points so that they cannot be added to the cluster as points
    for (int i = 0; i < centroids->size(); i++)
        foundPoints.push_back((*centroids)[i]);

    HashTables HashTablesObject(CLData->number_of_vector_hash_tables, CLData->number_of_vector_hash_functions, numOfInputPoints, CLData->dimension, numOfInputPoints / 8);

    for (int i = 0; i < numOfInputPoints; i++)
        HashTablesObject.HashTables::InsertPoint((*inputPoints)[i]);
    int inputPointsSize = inputPoints->size();
    double currRadius = minDistBetweenCentroids(centroids, CLData->number_of_clusters, CLData->dimension) / 2;
    vector<vector<PointPtr>> clusterPoints;
    vector<PointPtr> duplicates;
    clusterPoints.resize(CLData->number_of_clusters);
    while (inputPointsSize > 100 && currRadius < 10000)
    {
        for (int c = 0; c < CLData->number_of_clusters; c++)
        {

            clusterPoints[c] = HashTablesObject.range_search((*centroids)[c], currRadius);
            for (int pointIndex = 0; pointIndex < clusterPoints[c].size(); pointIndex++)
            {
                for (auto pntptr : foundPoints)
                {

                    if (pntptr->id == clusterPoints[c][pointIndex]->id)
                    {
                        clusterPoints[c].erase(clusterPoints[c].begin() + pointIndex);
                        break;
                    }
                }
            }
        }

        duplicates = find_duplicates(clusterPoints, CLData->number_of_clusters);

        for (auto currPoint : duplicates)
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

            for (int i = 0; i < CentroidsToBeErased.size(); i++)
            {
                if (i != minCentroid)
                    clusterPoints[CentroidsToBeErased[i]].erase(clusterPoints[CentroidsToBeErased[i]].begin() + position[i]);
            }
        }
        for (int c = 0; c < CLData->number_of_clusters; c++)
        {
            for (int pointIndex = 0; pointIndex < clusterPoints[c].size(); pointIndex++)
            {

                auto Found = find(inputPoints->begin(), inputPoints->end(), clusterPoints[c][pointIndex]);
                if (Found != inputPoints->end())
                {
                    inputPoints->erase(Found);
                    inputPointsSize--;
                }
                (*clusters)[c].points.push_back(clusterPoints[c][pointIndex]);
                foundPoints.push_back(clusterPoints[c][pointIndex]);
                (*clusters)[c].size++;
            }
            clusterPoints[c].clear();
        }
        currRadius *= 2;
    }
    int index = 0;
    for (auto currPoint : (*inputPoints))
    {
        index = lloyd_method(centroids, currPoint, CLData->dimension);
        (*clusters)[index].points.push_back(currPoint);
        (*clusters)[index].size++;
    }
    inputPoints->clear();
}

// void hyperCube_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints)
// {
//     vector<PointPtr> foundPoints;
//     // Adding centroids to found points so that they cannot be added to the cluster as points
//     for (int i = 0; i < centroids->size(); i++)
//         foundPoints.push_back((*centroids)[i]);

//     HashTables HashTablesObject(CLData->number_of_vector_hash_tables, CLData->number_of_vector_hash_functions, numOfInputPoints, CLData->dimension, numOfInputPoints / 8);

//     for (int i = 0; i < numOfInputPoints; i++)
//         HashTablesObject.HashTables::InsertPoint((*inputPoints)[i]);

//     double currRadius = minDistBetweenCentroids(centroids, CLData->number_of_clusters, CLData->dimension) / 2;
//     vector<vector<PointPtr>> clusterPoints;
//     vector<PointPtr> duplicates;
//     clusterPoints.resize(CLData->number_of_clusters);
//     while (!inputPoints->empty() && currRadius < 10000)
//     {
//         cout << "lsh1\n";

//         for (int c = 0; c < CLData->number_of_clusters; c++)
//         {
//             clusterPoints[c] = HashTablesObject.range_search((*centroids)[c], currRadius);
//             for (int pointIndex = 0; pointIndex < clusterPoints[c].size(); pointIndex++)
//                 for (auto pntptr : foundPoints)
//                     if (pntptr->id == clusterPoints[c][pointIndex]->id)
//                         clusterPoints[c].erase(clusterPoints[c].begin() + pointIndex);
//         }

//         duplicates = find_duplicates(clusterPoints, CLData->number_of_clusters);
//         cout << "lsh2\n";

//         for (auto currPoint : duplicates)
//         {
//             vector<int> CentroidsToBeErased;
//             vector<int> position;
//             for (int c = 0; c < CLData->number_of_clusters; c++)
//             {
//                 cout << "Hello1\n";
//                 for (int p = 0; p < clusterPoints[c].size(); p++)
//                 {

//                     if (currPoint->id == clusterPoints[c][p]->id)
//                     {
//                         cout << "Hello2\n";
//                         cout << "Hello3\n";
//                         CentroidsToBeErased.push_back(c);
//                         cout << "Hello4\n";
//                         position.push_back(p);
//                         cout << "Hello5\n";
//                     }
//                 }
//             }
//             cout << "lsh3\n";
//             double minDist = INT_MAX;
//             int minCentroid;
//             double currDist = 0.0;
//             int currCentroid;
//             cout << "lsh4\n";

//             for (int i = 0; i < CentroidsToBeErased.size(); i++)
//             {
//                 currCentroid = i;
//                 currDist = euclideanDistance((*centroids)[currCentroid], currPoint, CLData->dimension);
//                 if (currDist < minDist)
//                 {
//                     minDist = currDist;
//                     minCentroid = currCentroid;
//                 }
//             }
//             cout << "lsh5\n";

//             for (int i = 0; i < CentroidsToBeErased.size(); i++)
//             {
//                 if (i != minCentroid)
//                     clusterPoints[CentroidsToBeErased[i]].erase(clusterPoints[CentroidsToBeErased[i]].begin() + position[i]);
//             }
//             cout << "lsh6\n";
//         }
//         cout << "lsh7\n";

//         for (int c = 0; c < CLData->number_of_clusters; c++)
//         {
//             cout << "lsh8\n";

//             for (int pointIndex = 0; pointIndex < clusterPoints[c].size(); pointIndex++)
//             {
//                 auto Found = find(inputPoints->begin(), inputPoints->end(), clusterPoints[c][pointIndex]);
//                 if (Found != inputPoints->end())
//                 {
//                     inputPoints->erase(Found);
//                 }
//                 (*clusters)[c].points.push_back(clusterPoints[c][pointIndex]);
//                 foundPoints.push_back(clusterPoints[c][pointIndex]);
//                 (*clusters)[c].size++;
//             }
//             clusterPoints[c].clear();
//         }
//         cout << "lsh9\n";

//         currRadius *= 2;
//     }
//     int index = 0;
//     for (auto currPoint : (*inputPoints))
//     {
//         index = lloyd_method(centroids, currPoint, CLData->dimension);
//         (*clusters)[index].points.push_back(currPoint);
//         (*clusters)[index].size++;
//     }
//     inputPoints->clear();
// }

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
    vector<PointPtr> tempArray;
    vector<PointPtr> duplPoints;
    for (int i = 0; i < numOfClusters; i++)
    {
        std::merge((clusterPoints)[i].begin(), (clusterPoints)[i].end(), tempArray.begin(), tempArray.end(), std::back_inserter(mainArray), BY_ID_INT());
        usleep(2);
        tempArray.clear();
        for (auto currPoint : mainArray)
        {
            tempArray.push_back(currPoint);
        }
    }

    if (!mainArray.empty())
    {
        PointPtr currPoint = mainArray[0];
        int i = 1;
        while (i < mainArray.size())
        {
            if (currPoint->id == mainArray[i]->id)
            {
                duplPoints.push_back(currPoint);
                i++;
                while (i < mainArray.size())
                {
                    if (currPoint->id != mainArray[i]->id)
                        break;
                    i++;
                }
                continue;
            }
            currPoint = mainArray[i];
            i++;
        }
    }
    return duplPoints;
}