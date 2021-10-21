#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include "hashTable.h"

using namespace std;

HashTables::HashTables(int L, int k, int n)
{
    this->numOfHashTables = L;
    this->k = k;
    this->n = n;
    this->TableSize = n / 4;
    this->hash_tables.resize(L);
    this->t.resize(L);
    this->ri.resize(L);
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
            this->t[i][j] = rand() % W; //uniform random [0,W) todo
            this->ri[i][j] = rand() % 100 + 1;
        }
    }
}

void HashTables::InsertPoint(PointPtr point)
{
    for (int i = 0; i < this->numOfHashTables; i++)
    {
        this->hash_tables[i][HashFunc(point, i)].push_back(point);
    }
}

int HashTables::HashFunc(PointPtr point, int hashtableId)
{
    int h;
    for (int i = 0; i < k; i++)
    {
        h += this->ri[hashtableId][i] * floor((inner_product(point->coords, this->v[hashtableId][i]) + this->t[hashtableId][i]) / W);
    }
    return (h % BIGM) % this->TableSize;
}

HashTables::~HashTables()
{
}
