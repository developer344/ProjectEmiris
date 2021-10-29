#include "mathUtils.h"

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

double euclideanDistance(PointPtr x, PointPtr y, int dimension)
{
    double dist = 0.0;
    for (int i = 0; i < dimension; i++)
        dist += (x->coords[i] - y->coords[i]) * (x->coords[i] - y->coords[i]);
    return sqrt(dist);
}

int avoidOverFlowModulo(int a, int b, int m, char op)
{
    switch (op)
    {
    case '+':
        return euclideanModulo(euclideanModulo(a, m) + euclideanModulo(b, m), m);
    case '-':
        return euclideanModulo(euclideanModulo(a, m) - euclideanModulo(b, m), m);
    case '*':
        return euclideanModulo(euclideanModulo(a, m) * euclideanModulo(b, m), m);
    case '/':
        if (b == 0)
        {
            cerr << "Division by zero" << endl;
            exit(1);
        }
        return euclideanModulo(euclideanModulo(a, m) / euclideanModulo(b, m), m);
    case '%':
        return euclideanModulo(euclideanModulo(a, m) % euclideanModulo(b, m), m);
        ;
    default:
        cerr << "Wrong Operator" << endl;
        exit(1);
    }
}

int euclideanModulo(int x, int y)
{
    int returnValue = x % y;
    return returnValue >= 0 ? returnValue : returnValue + std::abs(y);
}

int powerWithBase2(int exp)
{

    int retValue;

    for (int i = 0; i < exp; i++)
    {
        retValue *= 2;
    }

    return retValue;
}