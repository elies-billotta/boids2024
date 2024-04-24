#include "MarkovChain.hpp"

MarkovChain::MarkovChain(std::vector<std::vector<double>> matrix, std::vector<int> states, RandomVariableGenerator& rng)
    : transitionMatrix(matrix), states(states), generator(rng) {}

int MarkovChain::nextState(int currentState)
{
    double randomValue = generator.RAND();

    double cumulativeProbability = 0.0;
    for (int nextState = 0; nextState < states.size(); ++nextState)
    {
        cumulativeProbability += transitionMatrix[currentState][nextState];
        if (randomValue < cumulativeProbability)
        {
            return states[nextState];
        }
    }
    return currentState;
}
