#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#include <iostream>
#include <string.h>
#include <random>
#include <cmath>
#include "HChashTable.h"

using namespace std;

double normalDistributionGenerator(double mi = 0.0, double sigma = 1.0);
double uniformDistributionGenerator(double alpha = 0.0, double beta = 1.0);
double euclideanDistance(Point *x, Point *y, int dimension);
int avoidOverFlowModulo(int a, int b, int m, char op);
int euclideanModulo(int x, int y);
int powerWithBase2(int exp);

#endif