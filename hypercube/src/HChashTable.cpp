
#include "mathUtils.h"
#include "hypercubeUtils.h"

#include <algorithm>

#include "HChashTable.h"
using namespace std;
HChashTable::HChashTable(int dimension,
                         int projectionDimension,
                         int probes,
                         int maxcandidatesPoints)
{
    this->dimension = dimension;
    this->projectionDimension = projectionDimension;
    this->probes = probes;
    this->maxcandidatesPoints = maxcandidatesPoints;
    this->bucketCount = powerWithBase2(this->projectionDimension + 1) - 1;
    this->Table.resize(this->bucketCount);
    this->t.resize(this->projectionDimension);
    this->ri.resize(this->projectionDimension);
    this->v.resize(this->projectionDimension);
    for (int i = 0; i < this->bucketCount; i++)
        this->Table[i] = NULL;
    for (int i = 0; i < this->projectionDimension; i++)
    {

        this->t[i] = uniformDistributionGenerator(0.0, W * 1.0);
        this->ri[i] = rand() % 2000 - 1000;
        this->v[i].resize(this->dimension);
        for (int j = 0; j < this->dimension; j++)
            this->v[i][j] = normalDistributionGenerator(0.0, 1.0);
    }
}

HChashTable::~HChashTable()
{
    for (int i = 0; i < this->bucketCount; i++)
        if (this->Table[i] != NULL)
            delete this->Table[i];
}

void HChashTable::InsertPoint(PointPtr point)
{
    unsigned long hashValue = this->HChashTable::HashFunc(point);
    // hashValue = hashValue % this->bucketCount;
    if (this->Table[hashValue] == NULL)
    {
        this->Table[hashValue] = new Bucket;
    }
    this->Table[hashValue]->points.push_back(point);
}

unsigned long HChashTable::HashFunc(PointPtr point)
{
    int h;

    bool f;

    unsigned long hashValue = 0;

    for (int i = 0; i < this->projectionDimension; i++)
    {

        h = floor((inner_product(point->coords.begin(), point->coords.end(), this->v[i].begin(), 0) + this->t[i]) / W);
        f = mapFunction(h, i + 1);

        hashValue += (f == true) ? powerWithBase2(i) : 0; // adds 2^i if true, else 0
    }

    return hashValue;
}

// Return a vector with all non-empty buckets with the hamming distance we asked for
std::vector<unsigned long> *HChashTable::find_n_hamming_distance(unsigned long currBucketValue, int hammingDistance)
{
    if (hammingDistance <= 0) // avoid unnecessary calculations
    {
        return NULL;
    }

    std::vector<unsigned long> *returnList = new std::vector<unsigned long>;

    // generate all unsigned long with n bits different
    //      check if they are not null
    //          if not null, add them to return list

    for (unsigned long i = 0; i < this->bucketCount; i++) // for each bucket number possible
    {
        int x = currBucketValue ^ i;
        int setBits = 0;

        // https://www.geeksforgeeks.org/hamming-distance-between-two-integers/
        while (x > 0)
        {
            setBits += x & 1;
            x >>= 1;
        }

        if (setBits == hammingDistance)
        {
            if (this->Table[i] != NULL)
                returnList->push_back(i);
        }
    }

    return returnList;
}

kNeighboursPtr HChashTable::find_k_nearest_neighbours(PointPtr queryPoint, int k_neighbours)
{

    std::vector<unsigned long> *bucketsToCheck = new std::vector<unsigned long>;

    int count = 0;
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

    unsigned long queryHash = this->HChashTable::HashFunc(queryPoint);
    bucketsToCheck->push_back(queryHash);
    int pointsChecked = 0, probesChecked = 0;
    unsigned long currBucket = queryHash;
    int currHammingDistance = 0;

    while (pointsChecked < this->maxcandidatesPoints && probesChecked < this->probes)
    {
        if (bucketsToCheck->empty())
        {
            do
            {
                delete bucketsToCheck;
                bucketsToCheck = this->HChashTable::find_n_hamming_distance(currBucket, ++currHammingDistance);
                if (bucketsToCheck == NULL)
                    bucketsToCheck = new std::vector<unsigned long>;
            } while (bucketsToCheck->empty() && currHammingDistance < this->projectionDimension);

            if (currHammingDistance > this->projectionDimension)
            {
                if (bucketsToCheck != NULL)
                    delete bucketsToCheck;
                delete currNeighbour;
                return returnData;
            }
        }
        currBucket = bucketsToCheck->back();
        bucketsToCheck->pop_back();
        // Checks currBucket
        for (int k = 0; k < this->Table[currBucket]->points.size() && pointsChecked < this->maxcandidatesPoints; k++)
        {

            if (notAlreadyExists(returnData, this->Table[currBucket]->points[k]->id))
            {
                currNeighbour->point = this->Table[currBucket]->points[k];
                currNeighbour->dist = euclideanDistance(queryPoint, currNeighbour->point, this->dimension);

                if (currNeighbour->dist < returnData->neighbours[k_neighbours - 1]->dist)
                {

                    if (returnData->size < k_neighbours)
                        returnData->size++;
                    returnData->neighbours[k_neighbours - 1]->point = currNeighbour->point;
                    returnData->neighbours[k_neighbours - 1]->dist = currNeighbour->dist;

                    count++;
                    pointsChecked++;
                    sort_neighbours(returnData, k_neighbours);
                }
            }
        }
        probesChecked++;
    }

    if (bucketsToCheck != NULL)
    {
        delete bucketsToCheck;
    }
    delete currNeighbour;
    return returnData;
}

vector<PointPtr> HChashTable::range_search(PointPtr queryPoint, double range)
{
    std::vector<unsigned long> *bucketsToCheck = new std::vector<unsigned long>;
    NeighbourPtr currNeighbour = new Neighbour;

    vector<PointPtr> returnData;

    unsigned long queryHash = this->HChashTable::HashFunc(queryPoint);
    bucketsToCheck->push_back(queryHash);
    int pointsChecked = 0, probesChecked = 0;
    unsigned long currBucket = queryHash;
    int currHammingDistance = 0;

    while (pointsChecked < this->maxcandidatesPoints && probesChecked < this->probes)
    {
        if (bucketsToCheck->empty())
        {
            do
            {
                delete bucketsToCheck;
                bucketsToCheck = this->HChashTable::find_n_hamming_distance(currBucket, ++currHammingDistance);
                if (bucketsToCheck == NULL)
                    bucketsToCheck = new std::vector<unsigned long>;
            } while (bucketsToCheck->empty() && currHammingDistance < this->projectionDimension);

            if (currHammingDistance > this->projectionDimension)
            {
                if (bucketsToCheck != NULL)
                    delete bucketsToCheck;
                delete currNeighbour;
                return returnData;
            }
        }
        currBucket = bucketsToCheck->back();
        bucketsToCheck->pop_back();
        // Checks currBucket
        for (int k = 0; k < this->Table[currBucket]->points.size() && pointsChecked < this->maxcandidatesPoints; k++)
        {

            bool found = binary_search(returnData.begin(), returnData.end(), this->Table[currBucket]->points[k], BY_ID());

            if (!found)
            {
                currNeighbour->point = this->Table[currBucket]->points[k];
                currNeighbour->dist = euclideanDistance(queryPoint, currNeighbour->point, this->dimension);

                if (currNeighbour->dist < range)
                {
                    returnData.push_back(this->Table[currBucket]->points[k]);
                    sort_points(&returnData);
                }
            }
        }
        probesChecked++;
    }

    if (bucketsToCheck != NULL)
    {
        delete bucketsToCheck;
    }

    delete currNeighbour;
    return returnData;
}