#include "RandomVariableGenerator.hpp"

RandomVariableGenerator::RandomVariableGenerator()
{
    RAND();
}

double RandomVariableGenerator::RAND() {
    // Initialisation de la graine pour RAND() avec le temps actuel
    srand(static_cast<unsigned int>(time(nullptr)));

    // Génération d'un nombre aléatoire entre 0 et 1
    return static_cast<double>(rand()) / RAND_MAX;
}

int RandomVariableGenerator::hypergeometric(int populationSize, int successStates, int sampleSize)
{
    if (successStates > populationSize || sampleSize > populationSize)
    {
        throw std::invalid_argument("Invalid hypergeometric parameters");
    }

    double p     = static_cast<double>(successStates) / populationSize;
    int    count = 0;
    for (int i = 0; i < sampleSize; ++i)
    {
        if (RAND() < p * RAND_MAX)
        {
            ++count;
        }
    }
    return count;
}

int RandomVariableGenerator::bernoulli(double p)
{
    if (p < 0 || p > 1)
    {
        throw std::invalid_argument("Invalid probability parameter for Bernoulli distribution");
    }
    return RAND() < p * RAND_MAX ? 1 : 0;
}

int RandomVariableGenerator::uniformDiscrete(int min, int max)
{
    if (min > max)
    {
        throw std::invalid_argument("Invalid range for uniform discrete distribution");
    }
    return min + static_cast<int>(RAND()) % (max - min + 1);
}

int RandomVariableGenerator::geometric(double p)
{
    if (p < 0 || p > 1)
    {
        throw std::invalid_argument("Invalid probability parameter for geometric distribution");
    }
    return std::log(1.0 - static_cast<double>(RAND()) / RAND_MAX) / std::log(1.0 - p);
}

double RandomVariableGenerator::exponential(double lambda)
{
    if (lambda <= 0)
    {
        throw std::invalid_argument("Invalid rate parameter for exponential distribution");
    }
    return -std::log(1.0 - static_cast<double>(RAND()) / RAND_MAX) / lambda;
}

double RandomVariableGenerator::laplace(double mu, double b)
{
    double u = static_cast<double>(RAND()) / RAND_MAX - 0.5;
    return mu - b * std::copysign(1.0, u) * std::log(1.0 - 2.0 * std::fabs(u));
}

double RandomVariableGenerator::normal(double mu, double sigma)
{
    double U1 = static_cast<double>(RAND()) / RAND_MAX;
    double U2 = static_cast<double>(RAND()) / RAND_MAX;
    double Z  = std::sqrt(-2.0 * std::log(U1)) * std::cos(2.0 * M_PI * U2);
    return mu + sigma * Z;
}

double RandomVariableGenerator::triangular(double a, double b, double c)
{
    double U = static_cast<double>(RAND()) / RAND_MAX;
    return U < (c - a) / (b - a) ? a + std::sqrt(U * (b - a) * (c - a)) : b - std::sqrt((1 - U) * (b - a) * (b - c));
}
