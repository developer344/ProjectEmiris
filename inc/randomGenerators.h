#ifndef RANDOM_GENERATORS_H
#define RANDOM_GENERATORS_H

#include <random>
#include <cmath>
#include "hashTable.h"

double normalDistributionGenerator(double mi = 0.0, double sigma = 1.0);
double uniformDistributionGenerator(double alpha = 0.0, double beta = 1.0);
int euclideanModulo(int x, int y);
double euclideanDistance(PointPtr x, PointPtr y, int dimension);
void sort_neighbours(vector<double> k_nearest_dist, vector<PointPtr> k_nearest_points);

#endif