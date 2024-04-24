#ifndef RANDOM_VARIABLE_GENERATOR_HPP
#define RANDOM_VARIABLE_GENERATOR_HPP

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

class RandomVariableGenerator {
public:
    RandomVariableGenerator();
    int    hypergeometric(int populationSize, int successStates, int sampleSize);
    int    bernoulli(double p);
    int    uniformDiscrete(int min, int max);
    int    geometric(double p);
    double exponential(double lambda);
    double laplace(double mu, double b);
    double normal(double mu, double sigma);
    double triangular(double a, double b, double c);

private:
    double RAND();
};

#endif /* RANDOM_VARIABLE_GENERATOR_HPP */
