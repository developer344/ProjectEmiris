#include "HChashtable.h"

HChashTable::HChashTable(int dimension,
                         int projectionDimension,
                         int probes,
                         int maxcantidatesPoints)
{

    this->dimension = dimension;
    this->projectionDimension = projectionDimension;
    this->probes = probes;
    this->maxcantidatesPoints = maxcantidatesPoints;

    this->bucketCount = powerWithBase2(this->projectionDimension);

    this->Table.resize(this->bucketCount);
    for (int i = 0; i < this->bucketCount; i++)
    {
        this->Table[i] = NULL;
        this->t[i] = uniformDistributionGenerator(0.0, W * 1.0);
        this->ri[i] = rand() % 2000 - 1000;
        this->v[i].resize(this->projectionDimension);
        for (int j = 0; j < this->projectionDimension; j++)
            this->v[i][j] = normalDistributionGenerator(0.0, 1.0);
    }
}

HChashTable::~HChashTable()
{
    for (int i = 0; i < this->bucketCount; i++)
        if (this->Table[i] != NULL)
            delete this->Table[i];
}

void HChashTable::Insert(PointPtr point)
{
    vector<int> h;
    vector<bool> f;
    unsigned long hashValue;
    h.resize(this->projectionDimension);
    for (int i = 0; i < this->projectionDimension; i++)
    {
        h[i] = floor((inner_product(point->coords.begin(), point->coords.end(), this->v[i].begin(), 0) + this->t[i]) / W);
        f[i] = mapFunction(h[i]);
        hashValue += (f[i] == true) ? powerWithBase2(i) : 0; // adds 2^i if true, else 0
    }

    if (this->Table[hashValue] == NULL)
        this->Table[hashValue] = new Bucket;
    this->Table[hashValue]->points.push_back(point);
}