#ifndef RANDOM_GENERATORS_H
#define RANDOM_GENERATORS_H

#include <random>

double normalDistributionGenerator(double mi = 0.0, double sigma = 1.0);
double uniformDistributionGenerator(double alpha = 0.0, double beta = 1.0);
int euclideanModulo(int x, int y);

#endif