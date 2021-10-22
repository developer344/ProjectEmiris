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
