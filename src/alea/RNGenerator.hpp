#ifndef RANDOM_VARIABLE_GENERATOR_HPP
#define RANDOM_VARIABLE_GENERATOR_HPP

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

class RNGenerator {
public:
    RNGenerator();
    static int    hypergeometric(int populationSize, int successStates, int sampleSize);
    static int    bernoulli(double p);
    static int    uniformDiscrete(int min, int max);
    static int    geometric(double p);
    static double exponential(double lambda);
    static double laplace(double mu, double b);
    static double normal(double mu, double sigma);
    static double triangular(double a, double b, double c);
    static double RAND();
};

#endif
