#include "MarkovChain.hpp"

#include <utility>
#include "alea/RNGenerator.hpp"

MarkovChain::MarkovChain(std::vector<std::vector<double>> matrix, std::vector<int> states)
    : transitionMatrix(std::move(matrix)), states(std::move(states)) {}

int MarkovChain::nextState(int currentState)
{
    double randomValue = RNGenerator::RAND();

    size_t currentStateIndex = -1;
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