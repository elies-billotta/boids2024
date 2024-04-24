#pragma once

#include <string>
#include <vector>
#include "RandomVariableGenerator.hpp" // Inclure la déclaration de RandomVariableGenerator

enum class MarkovChainTextureState {
    Texture1,
    Texture2
};

enum class MarkovChainDirection {
    Left,
    Right
};

class MarkovChain {
public:
    MarkovChain(std::vector<std::vector<double>> matrix, std::vector<int> states, RandomVariableGenerator& rng);

    int nextState(const int currentState);

private:
    std::vector<std::vector<double>> transitionMatrix;
    std::vector<int>         states;
    RandomVariableGenerator&         generator; // Référence vers RandomVariableGenerator
};