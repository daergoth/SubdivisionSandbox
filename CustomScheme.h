#ifndef CUSTOMSCHEME_H
#define CUSTOMSCHEME_H

#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum CustomSchemeMeshType {Tri, Quad};
enum CustomSchemeRefinementType {Approx, Interp};
enum CustomSchemeOddWeightType {Edge, Face};

class OddWeight {
public:
    OddWeight(){}
    OddWeight(int level, CustomSchemeOddWeightType type, double value): level(level), type(type), value(value){}
    int level;
    CustomSchemeOddWeightType type;
    double value;
};
void to_json(json& j, const OddWeight& ow);
void from_json(const json& j, OddWeight& ow);
std::ostream& operator<<(std::ostream& os, const OddWeight& ow);

class EvenWeights {
public:
    EvenWeights(){}
    EvenWeights(double alfa, double beta): alfa(alfa), beta(beta){}
    double alfa;
    double beta;
};
void to_json(json& j, const EvenWeights& ew);
void from_json(const nlohmann::json& j, EvenWeights& ew);
std::ostream& operator<<(std::ostream& os, const EvenWeights& ew);

class Weights {
public:
    Weights(){}
    Weights(std::vector<OddWeight> odd, EvenWeights even): odd(odd), even(even){}
    std::vector<OddWeight> odd;
    EvenWeights even;


};
void to_json(json& j, const Weights& w);
void from_json(const json& j, Weights& w);
std::ostream& operator<<(std::ostream& os, const Weights& w);

class CustomScheme
{
public:  
    CustomScheme(){}
    CustomScheme(std::string name, CustomSchemeMeshType mesh_type, CustomSchemeRefinementType refinement_type, int neighbour_level, Weights weights):
        name(name),
        mesh_type(mesh_type),
        refinement_type(refinement_type),
        neighbour_level(neighbour_level),
        weights(weights){}

    std::string name;
    CustomSchemeMeshType mesh_type;
    CustomSchemeRefinementType refinement_type;
    int neighbour_level;
    Weights weights;
};

std::ostream& operator<<(std::ostream& os, const CustomScheme& s);
void to_json(json& j, const CustomScheme& s);
void from_json(const json& j, CustomScheme& s);

#endif // CUSTOMSCHEME_H
