#include "RNGenerator.hpp"

RNGenerator::RNGenerator()
{
    // Initialisation de la graine pour RAND() avec le temps actuel
    srand(static_cast<unsigned int>(time(nullptr)));
}

double RNGenerator::RAND()
{
    // Génération d'un nombre aléatoire entre 0 et 1
    return static_cast<double>(rand()) / RAND_MAX;
}

int RNGenerator::hypergeometric(int populationSize, int successStates, int sampleSize)
{
    if (successStates > populationSize || sampleSize > populationSize)
    {
        throw std::invalid_argument("Invalid hypergeometric parameters");
    }

    double p     = static_cast<double>(successStates) / populationSize;
    int    count = 0;
    for (int i = 0; i < sampleSize; ++i)
    {
        if (RAND() < p)
        {
            ++count;
        }
    }
    return count;
}

int RNGenerator::bernoulli(double p)
{
    if (p < 0 || p > 1)
    {
        throw std::invalid_argument("Invalid probability parameter for Bernoulli distribution");
    }
    return RAND() < p ? 1 : 0;
}

int RNGenerator::uniformDiscrete(int min, int max)
{
    if (min > max)
    {
        throw std::invalid_argument("Invalid range for uniform discrete distribution");
    }

    // Génération d'un entier aléatoire entre min et max inclusivement
    return min + static_cast<int>(RAND() * (max - min + 1));
}

int RNGenerator::geometric(double p)
{
    if (p < 0 || p > 1)
    {
        throw std::invalid_argument("Invalid probability parameter for geometric distribution");
    }
    return std::log(1.0 - RAND()) / std::log(1.0 - p);
}

double RNGenerator::exponential(double lambda)
{
    if (lambda <= 0)
    {
        throw std::invalid_argument("Invalid rate parameter for exponential distribution");
    }
    return -std::log(1.0 - RAND()) / lambda;
}

double RNGenerator::laplace(double mu, double b)
{
    double u = RAND() - 0.5;
    return mu - b * std::copysign(1.0, u) * std::log(1.0 - 2.0 * std::fabs(u));
}

double RNGenerator::normal(double mu, double sigma)
{
    double Z = std::sqrt(-2.0 * std::log(RAND())) * std::cos(2.0 * M_PI * RAND());
    return mu + sigma * Z;
}

double RNGenerator::triangular(double a, double b, double c)
{
    return RAND() < (c - a) / (b - a) ? a + std::sqrt(RAND() * (b - a) * (c - a)) : b - std::sqrt((1 - RAND()) * (b - a) * (b - c));
}
