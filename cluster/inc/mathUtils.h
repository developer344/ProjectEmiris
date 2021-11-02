#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "clusterUtils.h"

double euclideanDistance(PointPtr x, PointPtr y, int dimension);
double uniformDistributionGenerator(double alpha = 0.0, double beta = 1.0);
bool is_number(const std::string &s);