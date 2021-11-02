#include "mathUtils.h"

double euclideanDistance(PointPtr x, PointPtr y, int dimension)
{
    double dist = 0.0;
    for (int i = 0; i < dimension; i++)
        dist += (x->coords[i] - y->coords[i]) * (x->coords[i] - y->coords[i]);
    return sqrt(dist);
}

double uniformDistributionGenerator(double alpha, double beta)
{
    // Source: https://www.cplusplus.com/reference/random/normal_distribution/
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<> distribution(alpha, beta);

    return distribution(generator);
}

bool is_number(const std::string &str)
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
{
    std::string::const_iterator iteration = str.begin();
    while (iteration != str.end() && std::isdigit(*iteration))
        ++iteration;
    return !str.empty() && iteration == str.end();
}