#include <algorithm>
#include <unistd.h>
#include <iterator>
#include <limits.h>

#include "methods.h"
#include "../../lib/mathUtils.h"
#include "../../lib/projectUtils.h"

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
    std::vector<PointPtr> foundPoints;
    std::vector<std::string> foundPointIDs;
    std::vector<std::vector<std::string>> foundPointIDsPerCluster;

    foundPointIDsPerCluster.resize(CLData->number_of_clusters);
    // Adding centroids to found points so that they cannot be added to the cluster as points
    for (int i = 0; i < centroids->size(); i++)
        foundPoints.push_back((*centroids)[i]);

    HashTables HashTablesObject(CLData->number_of_vector_hash_tables, CLData->number_of_vector_hash_functions, numOfInputPoints, CLData->dimension, numOfInputPoints / 8);

    for (int i = 0; i < numOfInputPoints; i++)
        HashTablesObject.HashTables::InsertPoint((*inputPoints)[i]);
    int inputPointsSize = inputPoints->size();
    double currRadius = minDistBetweenCentroids(centroids, CLData->number_of_clusters, CLData->dimension) / 2;
    std::vector<std::vector<PointPtr>> clusterPoints;
    std::vector<PointPtr> duplicates;
    clusterPoints.resize(CLData->number_of_clusters);

    int initialInputPoints = inputPointsSize;
    int initialRadius = currRadius;
    int currNumOfFound = 101;
    while (inputPointsSize > initialInputPoints / 100 && currRadius < initialRadius * 100 && currNumOfFound > 1)
    {
        currNumOfFound = 0;
        for (int c = 0; c < CLData->number_of_clusters; c++)
        {
            clusterPoints[c] = HashTablesObject.range_search((*centroids)[c], currRadius, &(foundPointIDsPerCluster[c]));
        }
        std::vector<std::string> tempArray;
        for (int i = 0; i < CLData->number_of_clusters; i++)
        {
            std::merge(foundPointIDsPerCluster[i].begin(), foundPointIDsPerCluster[i].end(), tempArray.begin(), tempArray.end(), std::back_inserter(foundPointIDs));
            tempArray.clear();
            for (auto currPoint : foundPointIDs)
            {
                tempArray.push_back(currPoint);
            }
        }
        for (int i = 0; i < CLData->number_of_clusters; i++)
        {
            foundPointIDsPerCluster[i].clear();
            for (auto currPoint : foundPointIDs)
            {
                foundPointIDsPerCluster[i].push_back(currPoint);
            }
        }

        duplicates = find_duplicates(clusterPoints, CLData->number_of_clusters);
        for (auto currPoint : duplicates)
        {
            std::vector<int> CentroidsToBeErased;
            std::vector<int> position;
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
                {
                    clusterPoints[CentroidsToBeErased[i]].erase(clusterPoints[CentroidsToBeErased[i]].begin() + position[i]);
                }
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
                (*clusters)[c].size++;
                currNumOfFound++;
            }
            clusterPoints[c].clear();
        }
        //std::sort(foundPointIDs.begin(), foundPointIDs.end());
        currRadius *= 2;
        std::cout << currNumOfFound << std::endl;
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

void hyperCube_method(std::vector<PointPtr> *centroids, std::vector<Cluster> *clusters, std::vector<PointPtr> *inputPoints, inputData *CLData, int numOfInputPoints)
{
    std::vector<PointPtr> foundPoints;
    std::vector<std::string> foundPointIDs;
    std::vector<std::vector<std::string>> foundPointIDsPerCluster;

    foundPointIDsPerCluster.resize(CLData->number_of_clusters);
    // Adding centroids to found points so that they cannot be added to the cluster as points
    for (int i = 0; i < centroids->size(); i++)
        foundPoints.push_back((*centroids)[i]);

    HChashTable HypercubeObject(CLData->dimension, CLData->number_of_hypercube_dimensions, CLData->number_of_probes, CLData->max_number_M_hypercube);
    // HashTables HashTablesObject(CLData->number_of_vector_hash_tables, CLData->number_of_vector_hash_functions, numOfInputPoints, CLData->dimension, numOfInputPoints / 8);

    for (int i = 0; i < numOfInputPoints; i++)
        HypercubeObject.HChashTable::InsertPoint((*inputPoints)[i]);
    int inputPointsSize = inputPoints->size();
    double currRadius = minDistBetweenCentroids(centroids, CLData->number_of_clusters, CLData->dimension) / 2;
    std::vector<std::vector<PointPtr>> clusterPoints;
    std::vector<PointPtr> duplicates;
    clusterPoints.resize(CLData->number_of_clusters);

    int initialInputPoints = inputPointsSize;
    int initialRadius = currRadius;
    int currNumOfFound = 101;
    while (inputPointsSize > initialInputPoints / 100 && currRadius < initialRadius * 100 && currNumOfFound > 1)
    {
        currNumOfFound = 0;
        for (int c = 0; c < CLData->number_of_clusters; c++)
        {
            clusterPoints[c] = HypercubeObject.range_search((*centroids)[c], currRadius, &(foundPointIDsPerCluster[c]));
        }
        std::vector<std::string> tempArray;
        for (int i = 0; i < CLData->number_of_clusters; i++)
        {
            std::merge(foundPointIDsPerCluster[i].begin(), foundPointIDsPerCluster[i].end(), tempArray.begin(), tempArray.end(), std::back_inserter(foundPointIDs));
            tempArray.clear();
            for (auto currPoint : foundPointIDs)
            {
                tempArray.push_back(currPoint);
            }
        }
        for (int i = 0; i < CLData->number_of_clusters; i++)
        {
            foundPointIDsPerCluster[i].clear();
            for (auto currPoint : foundPointIDs)
            {
                foundPointIDsPerCluster[i].push_back(currPoint);
            }
        }

        duplicates = find_duplicates(clusterPoints, CLData->number_of_clusters);
        for (auto currPoint : duplicates)
        {
            std::vector<int> CentroidsToBeErased;
            std::vector<int> position;
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
                {
                    clusterPoints[CentroidsToBeErased[i]].erase(clusterPoints[CentroidsToBeErased[i]].begin() + position[i]);
                }
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
                (*clusters)[c].size++;
                currNumOfFound++;
            }
            clusterPoints[c].clear();
        }
        //std::sort(foundPointIDs.begin(), foundPointIDs.end());
        currRadius *= 2;
        std::cout << currNumOfFound << std::endl;
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

std::vector<PointPtr> find_duplicates(std::vector<std::vector<PointPtr>> clusterPoints, int numOfClusters)
{
    std::vector<PointPtr> mainArray;
    std::vector<PointPtr> tempArray;
    std::vector<PointPtr> duplPoints;
    for (int i = 0; i < numOfClusters; i++)
    {
        std::merge((clusterPoints)[i].begin(), (clusterPoints)[i].end(), tempArray.begin(), tempArray.end(), std::back_inserter(mainArray), BY_ID());
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