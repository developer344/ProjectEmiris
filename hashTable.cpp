#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>

#include "hashTable.h"
#include "randomGenerators.h"

using namespace std;

HashTables::HashTables(int L, int k, int n, int dimension)
{
    this->numOfHashTables = L;
    this->k = k;
    this->n = n;
    this->TableSize = n / 4;

    this->dim = dimension;
    this->hash_tables.resize(L);
    this->t.resize(L);
    this->ri.resize(L);
    this->v.resize(L);

    cout << "Number of hash tables" << this->hash_tables.size() << endl;
    for (int i = 0; i < L; i++)
    {
        this->hash_tables[i].resize(n / 4);
        cout << "Number of buckets in hash table "
             << i << " is "
             << this->hash_tables[i].size() << endl;
        this->t[i].resize(k);
        this->ri[i].resize(k);
        this->v[i].resize(k);

        for (int j = 0; j < k; j++)
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

    cout << "Problem10" << endl;
    for (int i = 0; i < this->numOfHashTables; i++)
    {
        cout << "Problem11" << endl;
        this->hash_tables[i][HashFunc(point, i)].push_back(point);
        cout << "Problem18" << endl;
    }
}

int HashTables::HashFunc(PointPtr point, int hashtableId)
{
    cout << "Problem13" << endl;
    int h;
    cout << "Problem14" << endl;
    for (int i = 0; i < k; i++)
    {
        cout << "Problem15" << endl;
        h += this->ri[hashtableId][i] * floor((inner_product(point->coords.begin(), point->coords.end(), this->v[hashtableId][i].begin(), 0) + this->t[hashtableId][i]) / W);
        cout << "Problem16" << endl;
    }
    cout << "Problem17" << (h % BIGM) % this->TableSize << h << endl;
    return euclideanModulo(euclideanModulo(h, BIGM), this->TableSize);
}

void HashTables::PrintHashTables()
{

    for (int i = 0; i < this->numOfHashTables; i++)
    {
        cout << "Hash Table " << i << ":" << endl;
        for (int j = 0; j < this->TableSize; j++)
        {
            cout << "\tBucket " << j << ":" << endl;
            for (PointPtr &point : this->hash_tables[i][j])
                cout << "\t\tPoint with id: " << point->id << endl;
        }
    }
}