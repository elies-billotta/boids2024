#include "MarkovChain.hpp"

MarkovChain::MarkovChain(std::vector<std::vector<double>> matrix, std::vector<int> states, RandomVariableGenerator& rng)
    : transitionMatrix(matrix), states(states), generator(rng) {}

int MarkovChain::nextState(int currentState)
{
    double randomValue = generator.RAND();

    int currentStateIndex = -1;
    for (size_t i = 0; i < states.size(); ++i)
    {
        if (states[i] == currentState)
        {
            currentStateIndex = i;
            break;
        }
    }
    if (currentStateIndex == -1)
    {
        // État actuel non trouvé, retourner l'état actuel
        return currentState;
    }

    double cumulativeProbability = 0.0;
    for (size_t nextState = 0; nextState < states.size(); ++nextState)
    {
        cumulativeProbability += transitionMatrix[currentStateIndex][nextState];
        if (randomValue < cumulativeProbability)
        {
            return states[nextState];
        }
    }
    return currentState;
}