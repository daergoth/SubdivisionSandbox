#ifndef WEIGHTARRAYGENERATOR_H
#define WEIGHTARRAYGENERATOR_H

#include <array>

#include "CustomScheme.h"
#include "MeshWalkHandler.h"

class WeightArrayGenerator
{
public:
    static WeightArrayGenerator& getInstance() {
        static WeightArrayGenerator instance;
        return instance;
    }

    WeightArrayGenerator(WeightArrayGenerator const&) = delete;
    void operator=(WeightArrayGenerator const&) = delete;

    std::array<float, 16> generateWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type);
    std::vector<float> generateEvenWeights(CustomScheme custom_scheme, int neighbour_num);

private:
    WeightArrayGenerator();

    std::array<float, 16> generateTriWeights(CustomScheme custom_scheme);
    std::array<float, 16> generateQuadWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type);

    std::array<float, 16> generateQuadOneWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type);
    std::array<float, 16> generateQuadTwoWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type);
};

#endif // WEIGHTARRAYGENERATOR_H
