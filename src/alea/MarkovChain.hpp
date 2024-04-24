#pragma once

#include <vector>
#include "RandomVariableGenerator.hpp" // Inclure la déclaration de RandomVariableGenerator

class MarkovChain {
public:
    MarkovChain(std::vector<std::vector<double>> matrix, std::vector<int> states, RandomVariableGenerator& rng);

    int nextState(int currentState);

private:
    std::vector<std::vector<double>> transitionMatrix;
    std::vector<int> states;
    RandomVariableGenerator& generator; // Référence vers RandomVariableGenerator
};
