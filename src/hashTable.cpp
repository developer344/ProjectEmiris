#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <algorithm> //std::find

#include "hashTable.h"
#include "randomGenerators.h"

using namespace std;

HashTables::HashTables(int L, int numberOfHyperplanes, int numberOfPoints, int dimension) // Constructor

{
    this->numOfHashTables = L;
    this->numberOfHyperplanes = numberOfHyperplanes;
    this->numberOfPoints = numberOfPoints;
    this->TableSize = numberOfPoints / 4;

    this->dim = dimension;
    this->hash_tables.resize(L);
    this->t.resize(L);
    this->ri.resize(L);
    this->v.resize(L);

    cout << "Number of hash tables" << this->hash_tables.size() << endl;
    for (int i = 0; i < L; i++)
    {
        this->hash_tables[i].resize(this->TableSize);
        cout << "Number of buckets in hash table "
             << i << " is "
             << this->hash_tables[i].size() << endl;
        this->t[i].resize(numberOfHyperplanes);
        this->ri[i].resize(numberOfHyperplanes);
        this->v[i].resize(numberOfHyperplanes);

        for (int j = 0; j < numberOfHyperplanes; j++)
        {
            this->t[i][j] = uniformDistributionGenerator(0.0, W * 1.0);
            this->ri[i][j] = rand() % 100 + 1;
            this->v[i][j].resize(this->dim);
            for (int l = 0; l < this->dim; l++)
                this->v[i][j][l] = normalDistributionGenerator(0.0, 1.0);
        }
    }
}

void HashTables::InsertPoint(PointPtr point)
{

    for (int i = 0; i < this->numOfHashTables; i++)
    {
        int id = HashFunc(point, i);
        this->hash_tables[i][euclideanModulo(id, this->TableSize)].ID.push_back(id);
        this->hash_tables[i][euclideanModulo(id, this->TableSize)].points.push_back(point);
    }
}

int HashTables::HashFunc(PointPtr point, int hashtableId)
{
    int h;
    for (int i = 0; i < numberOfHyperplanes; i++)
    {
        h += this->ri[hashtableId][i] * floor((inner_product(point->coords.begin(), point->coords.end(), this->v[hashtableId][i].begin(), 0) + this->t[hashtableId][i]) / W);
    }
    return euclideanModulo(h, BIGM);
}

kNeighboursPtr HashTables::find_k_nearest_neighbours(PointPtr queryPoint, int k_neighbours)
{
    // PointPtr curPoint;
    // int curDist;

    NeighbourPtr currNeighbour = new Neighbour;

    kNeighboursPtr returnData = new kNeighbours;
    returnData->neighbours.resize(k_neighbours);
    returnData->size = 0;

    for (int i = 0; i < k_neighbours; i++)
    {
        returnData->neighbours[i] = new Neighbour;
        returnData->neighbours[i]->point = NULL;
        returnData->neighbours[i]->dist = INT32_MAX; // initialize distance with a very big value
    }

    for (int i = 0; i < this->numOfHashTables; i++) // for i from 1 to L do
    {
        int queryID = this->HashFunc(queryPoint, i);
        int g = euclideanModulo(queryID, this->TableSize);

        for (int j = 0; j < this->hash_tables[i][g].points.size(); j++) // for each item p in bucket gi(q) do
        {

            if (this->hash_tables[i][g].ID[j] == queryID && notAlreadyExists(returnData, this->hash_tables[i][g].points[j]->id)) // if p,q actually belong in same bucket
            {

                currNeighbour->point = this->hash_tables[i][g].points[j];
                currNeighbour->dist = euclideanDistance(queryPoint, currNeighbour->point, this->dim);
                // if dist(q,p) < db then b <- p; db <- dist(q,p)
                if (currNeighbour->dist < returnData->neighbours[k_neighbours - 1]->dist)
                {
                    if (returnData->size < k_neighbours)
                        returnData->size++;
                    returnData->neighbours[k_neighbours - 1]->point = currNeighbour->point;
                    returnData->neighbours[k_neighbours - 1]->dist = currNeighbour->dist;

                    // cout << "BEFORE SORT" << endl;
                    // for (int z = 0; z < k_neighbours; z++)
                    // {
                    //     cout << returnData->neighbours[z]->dist
                    //          << " ";
                    // }
                    // cout << endl;

                    sort_neighbours(returnData, k_neighbours);

                    // cout << "AFTER SORT" << endl;
                    // for (int z = 0; z < k_neighbours; z++)
                    // {
                    //     cout << returnData->neighbours[z]->dist
                    //          << " ";
                    // }
                    // cout << endl;
                }
            }
        }
    }
    // delete currNeighbour;
    return returnData;
}

kNeighboursPtr HashTables::find_k_true_neighbours(PointPtr queryPoint, int k_neighbours, vector<PointPtr> inputPoints)
{
    // PointPtr curPoint;
    // int curDist;

    NeighbourPtr currNeighbour = new Neighbour;

    kNeighboursPtr returnData = new kNeighbours;
    returnData->neighbours.resize(k_neighbours);
    returnData->size = 0;

    for (int i = 0; i < k_neighbours; i++)
    {
        returnData->neighbours[i] = new Neighbour;
        returnData->neighbours[i]->point = NULL;
        returnData->neighbours[i]->dist = INT32_MAX; // initialize distance with a very big value
    }

    for (int i = 0; i < inputPoints.size(); i++)
    {
        currNeighbour->point = inputPoints[i];
        currNeighbour->dist = euclideanDistance(queryPoint, currNeighbour->point, this->dim);

        if (currNeighbour->dist < returnData->neighbours[k_neighbours - 1]->dist) //&& currNeighbour->dist > 0)
        {
            if (returnData->size < k_neighbours)
                returnData->size++;
            returnData->neighbours[k_neighbours - 1]->point = currNeighbour->point;
            returnData->neighbours[k_neighbours - 1]->dist = currNeighbour->dist;

            sort_neighbours(returnData, k_neighbours);
        }
    }

    // delete currNeighbour;
    return returnData;
}

vector<PointPtr> HashTables::range_search(PointPtr queryPoint, double range)
{

    NeighbourPtr currNeighbour = new Neighbour;

    // kNeighboursPtr returnData = new kNeighbours;

    vector<PointPtr> returnData;
    //returnData->neighbours.resize(k_neighbours);
    //returnData->size = 0;

    // for (int i = 0; i < k_neighbours; i++)
    // {
    //     returnData->neighbours[i] = new Neighbour;
    //     returnData->neighbours[i]->point = NULL;
    //     returnData->neighbours[i]->dist = INT32_MAX; // initialize distance with a very big value
    // }

    for (int i = 0; i < this->numOfHashTables; i++) // for i from 1 to L do
    {
        int queryID = this->HashFunc(queryPoint, i);
        int g = euclideanModulo(queryID, this->TableSize);

        for (int j = 0; j < this->hash_tables[i][g].points.size(); j++) // for each item p in bucket gi(q) do
        {

            // if (this->hash_tables[i][g].ID[j] == queryID && (find(returnData.begin(), returnData.end(), this->hash_tables[i][g].points[j]) == returnData.end())) // if p,q actually belong in same bucket - test with ID(p)
            // {
            if (find(returnData.begin(), returnData.end(), this->hash_tables[i][g].points[j]) == returnData.end())
            {

                currNeighbour->point = this->hash_tables[i][g].points[j];
                currNeighbour->dist = euclideanDistance(queryPoint, currNeighbour->point, this->dim);
                // if dist(q,p) < range then add

                if (currNeighbour->dist < range)
                {
                    PointPtr tempPoint = new Point;
                    tempPoint = currNeighbour->point;
                    returnData.push_back(tempPoint);
                }

                // // if dist(q,p) < db then b <- p; db <- dist(q,p)
                // if (currNeighbour->dist < returnData->neighbours[k_neighbours - 1]->dist)
                // {
                //     if (returnData->size < k_neighbours)
                //         returnData->size++;
                //     returnData->neighbours[k_neighbours - 1]->point = currNeighbour->point;
                //     returnData->neighbours[k_neighbours - 1]->dist = currNeighbour->dist;

                //     cout << "BEFORE SORT" << endl;
                //     for (int z = 0; z < k_neighbours; z++)
                //     {
                //         cout << returnData->neighbours[z]->dist
                //              << " ";
                //     }
                //     cout << endl;

                //     sort_neighbours(returnData, k_neighbours);

                //     cout << "AFTER SORT" << endl;
                //     for (int z = 0; z < k_neighbours; z++)
                //     {
                //         cout << returnData->neighbours[z]->dist
                //              << " ";
                //     }
                //     cout << endl;
                // }
            }
        }
    }
    // delete currNeighbour;
    return returnData;
}

void HashTables::PrintHashTables()
{

    for (int i = 0; i < this->numOfHashTables; i++)
    {
        cout << "Hash Table " << i << ":" << endl;
        for (int j = 0; j < this->TableSize; j++)
        {
            cout << "\tBucket " << j << ":" << endl;
            for (PointPtr &point : this->hash_tables[i][j].points)
                cout << "\t\tPoint with id: " << point->id << endl;
        }
    }
}