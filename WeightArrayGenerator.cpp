#include "WeightArrayGenerator.h"

WeightArrayGenerator::WeightArrayGenerator() {

}

std::array<float, 16> WeightArrayGenerator::generateWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type) {
    switch (custom_scheme.mesh_type) {
    case CustomSchemeMeshType::Tri:
        return generateTriWeights(custom_scheme);
        break;
    case CustomSchemeMeshType::Quad:
        return generateQuadWeights(custom_scheme, odds_type);
    default:
        throw std::exception("Unknown CustomSchemeMeshType during weight array generation: " + custom_scheme.mesh_type);
        break;
    }
}
std::vector<float> WeightArrayGenerator::generateEvenWeights(CustomScheme custom_scheme, int neighbour_num) {
    std::vector<float> weights;
    float alfa, beta, gamma;
    switch (custom_scheme.mesh_type) {
    case CustomSchemeMeshType::Tri:
        weights.push_back(custom_scheme.weights.even.alfa);
        alfa = custom_scheme.weights.even.alfa;
        beta = (1.0 - alfa) / neighbour_num;
        for (int i = 0; i < neighbour_num; ++i) {
            weights.push_back(beta);
        }

        return weights;
        break;
    case CustomSchemeMeshType::Quad:
        weights.push_back(custom_scheme.weights.even.alfa);

        alfa = custom_scheme.weights.even.alfa;
        beta = custom_scheme.weights.even.beta;
        gamma = (1.0 - alfa - (neighbour_num/2.0) * beta) / (neighbour_num/2.0);

        for (int i = 0; i < (neighbour_num/2.0); ++i) {
            weights.push_back(beta);
            weights.push_back(gamma);
        }

        return weights;
        break;
    default:
        throw std::exception("Unknown CustomSchemeMeshType during even weight array generation: " + custom_scheme.mesh_type);
        break;
    }
}

std::array<float, 16> WeightArrayGenerator::generateTriWeights(CustomScheme custom_scheme) {
    float f, s, t;
    switch (custom_scheme.neighbour_level) {
    case 1:
        // Like Loop odd
        for(OddWeight w : custom_scheme.weights.odd) {
            if (w.level == 0) {
                f = w.value;
                s = (1.0 - 2.0 * f) / 2.0;
                return std::array<float, 16>{{s, f, f, s}};
            }
        }
        break;
    case 2:
        // Like Butterfly odd
        for (OddWeight w : custom_scheme.weights.odd) {
            if (w.level == 0) {
                f = w.value;
            } else if (w.level == 1) {
                s = w.value;
            }
        }
        t = (1.0 - 2.0 * f - 2.0 * s) / 4.0;
        return std::array<float, 16>{{t, s, t, f, f, t, s, t}};
        break;
    default:
        throw std::exception("Invalid neighbour level during triangular weight generation: " + custom_scheme.neighbour_level);
        break;
    }
}

std::array<float, 16> WeightArrayGenerator::generateQuadWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type) {
    switch (custom_scheme.neighbour_level) {
    case 1:
        // Like CatmullClark odd
        return generateQuadOneWeights(custom_scheme, odds_type);
        break;
    case 2:
        // Like Kobbelt odd
        return generateQuadTwoWeights(custom_scheme, odds_type);
        break;
    default:
        throw std::exception("Invalid neighbour level during quad weight generation: " + custom_scheme.neighbour_level);
        break;
    }
}

std::array<float, 16> WeightArrayGenerator::generateQuadOneWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type) {
    float f, s;

    switch (odds_type) {
    case MeshWalkHandler::OddsType::Face:
        // Like CatmullClark face odd
        f =  1.0 / 4.0;
        return std::array<float, 16>{{f, f, f, f}};
        break;
    case MeshWalkHandler::OddsType::Edge:
        // Like CatmullClark edge odd
        for (OddWeight w : custom_scheme.weights.odd) {
            if (w.level == 0 && w.type == CustomSchemeOddWeightType::Edge) {
                f = w.value;
                s = (1.0 - 2.0 * f) / 4.0;
                return std::array<float, 16>{{s, s, f, f, s, s}};
            }
        }
        break;
    default:
        throw std::exception("Unknown odds type during quad one weight generation: " + odds_type);
        break;
    }
}

std::array<float, 16> WeightArrayGenerator::generateQuadTwoWeights(CustomScheme custom_scheme, MeshWalkHandler::OddsType odds_type) {
    float f, s, t;
    switch (odds_type) {
    case MeshWalkHandler::OddsType::Face:
        // Like Kobbelt face odd
        for (OddWeight w : custom_scheme.weights.odd) {
            if (w.level == 0 && w.type == CustomSchemeOddWeightType::Face) {
                f = w.value;
            } else if (w.level == 1) {
                s = w.value;
            }
        }
        t = (1.0 - 4.0 * f - 8.0 * s) / 4.0;
        return std::array<float, 16>{{t, s, s, t, s, f, f, s, s, f, f, s, t, s, s, t}};
        break;
    case MeshWalkHandler::OddsType::Edge:
        // Like Kobbelt edge odd
        for (OddWeight w : custom_scheme.weights.odd) {
            if (w.level == 0 && w.type == CustomSchemeOddWeightType::Edge) {
                f = w.value;
                s = (1.0 - 2.0 * f) / 2.0;
                return std::array<float, 16>{{s, f, f, s}};
            }
        }
        break;
    default:
        throw std::exception("Unknown odds type during quad two weight generation: " + odds_type);
        break;
    }
}
