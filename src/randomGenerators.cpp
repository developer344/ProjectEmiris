#include "randomGenerators.h"

double normalDistributionGenerator(double mi, double sigma)
{
    // Source: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mi, sigma);

    return distribution(generator);
}

double uniformDistributionGenerator(double alpha, double beta)
{
    // Source: https://www.cplusplus.com/reference/random/normal_distribution/
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<> distribution(alpha, beta);

    return distribution(generator);
}

int euclideanModulo(int x, int y)
{
    int returnValue = x % y;
    return returnValue >= 0 ? returnValue : returnValue + std::abs(y);
}

double euclideanDistance(PointPtr x, PointPtr y, int dimension)
{
    double dist = 0.0;
    for (int i = 0; i < dimension; i++)
        dist += (x->coords[i] - y->coords[i]) * (x->coords[i] - y->coords[i]);
    return sqrt(dist);
}

void sort_neighbours(kNeighboursPtr k_nearest_neighbours, int k_neighbours) // sort distance in a vector of k distances
{
    int k = k_neighbours; // number of neighbours
    NeighbourPtr tempNeighbour;

    for (int i = k - 1; i > 0; i--)
    {
        if (k_nearest_neighbours->neighbours[i]->dist < k_nearest_neighbours->neighbours[i - 1]->dist)
        {
            tempNeighbour = k_nearest_neighbours->neighbours[i - 1];
            k_nearest_neighbours->neighbours[i - 1] = k_nearest_neighbours->neighbours[i];
            k_nearest_neighbours->neighbours[i] = tempNeighbour;
        }
    }
}

int notAlreadyExists(kNeighboursPtr k_nearest_neighbours, string pointID)
{

    for (int i = 0; i < k_nearest_neighbours->size; i++)
        if (k_nearest_neighbours->neighbours[i]->point->id == pointID)
            return 0;
    return 1;
}