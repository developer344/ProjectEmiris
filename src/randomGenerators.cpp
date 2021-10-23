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

void sort_neighbours(vector<double> k_nearest_dist, vector<PointPtr> k_nearest_points) // sort distance in a vector of k distances
{
    int k = k_nearest_points.size(); // number of neighbours
    double tempDist;
    PointPtr tempPoint;

    for (int i = k - 1; i > 1; i--)
    {
        if (k_nearest_dist[i] < k_nearest_dist[i - 1])
        {
            tempDist = k_nearest_dist[i - 1];
            k_nearest_dist[i - 1] = k_nearest_dist[i];
            k_nearest_dist[i] = tempDist;

            tempPoint = k_nearest_points[i - 1];
            k_nearest_points[i - 1] = k_nearest_points[i];
            k_nearest_points[i] = tempPoint;
        }
    }
}