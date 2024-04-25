#pragma once

#include <string>
#include <vector>
#include "RandomVariableGenerator.hpp" // Inclure la déclaration de RandomVariableGenerator

enum class MarkovChainSparkState {
    TextureUpdate,
    PositionUpdate
};

enum class MarkovChainLightState {
    LightOff,
    LightOn
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