#ifndef RANDOM_GENERATORS_H
#define RANDOM_GENERATORS_H

#include <random>
#include <cmath>
#include "hashTable.h"

double normalDistributionGenerator(double mi = 0.0, double sigma = 1.0);
double uniformDistributionGenerator(double alpha = 0.0, double beta = 1.0);
int euclideanModulo(int x, int y);
double euclideanDistance(PointPtr x, PointPtr y, int dimension);
void sort_neighbours(kNeighboursPtr k_nearest_neighbours, int k_neighbours);
int notAlreadyExists(kNeighboursPtr k_nearest_neighbours, string pointID);
kNeighboursPtr find_k_true_neighbours(PointPtr queryPoint, int k_neighbours, vector<PointPtr> inputPoints, int dim);

#endif