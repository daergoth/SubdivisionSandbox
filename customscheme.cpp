#include "customscheme.h"

// OddWeight
void to_json(json& j, const OddWeight& ow) {
    j = json{{"level", ow.level}, {"value", ow.value}};
}
void from_json(const json& j, OddWeight& ow) {
    j.at("level").get_to(ow.level);
    j.at("value").get_to(ow.value);
}
std::ostream& operator<<(std::ostream& os, const OddWeight& ow) {
    // write obj to stream
    os << "[level:" << ow.level
       << ", value:" << std::to_string(ow.value) << "]";
    return os;
}


// EvenWeights
void to_json(json& j, const EvenWeights& ew) {
    j = json{{"beta", ew.beta}, {"gamma", ew.gamma}};
}
void from_json(const nlohmann::json& j, EvenWeights& ew) {
    j.at("beta").get_to(ew.beta);
    j.at("gamma").get_to(ew.gamma);
}
std::ostream& operator<<(std::ostream& os, const EvenWeights& ew) {
    // write obj to stream
    os << "[beta:" << std::to_string(ew.beta)
       << ", gamma:" << std::to_string(ew.gamma) << "]";
    return os;
}

// Weights
void to_json(json& j, const Weights& w) {
    j = json{{"odd", w.odd}, {"even", w.even}};
}
void from_json(const json& j, Weights& w) {
    w.odd = j.at("odd");
    w.even = j.at("even");
}
std::ostream& operator<<(std::ostream& os, const Weights& w) {
    os << "[odd:[";
    for (OddWeight ow : w.odd) {
        os << ow;
    }
    os << "], even:" << w.even << "]";
    return os;
}


// CustomScheme
void to_json(json& j, const CustomScheme& s) {
    j = json{
        {"meshtype", s.mesh_type},
        {"refinementtype", s.refinement_type},
        {"neighbourlevel", s.neighbour_level},
        {"weights", s.weights}
    };
}
void from_json(const json& j, CustomScheme& s) {
    s.mesh_type = j.at("meshtype");
    s.refinement_type = j.at("refinementtype");
    s.neighbour_level = j.at("neighbourlevel");
    s.weights = j.at("weights");
}
std::ostream& operator<<(std::ostream& os, const CustomScheme& s) {
    os << "[mesh_type:" << s.mesh_type
       << ", refinement_type:" << s.refinement_type
       << ", neighbour_level:" << s.neighbour_level
       << ", weights:" << s.weights << "]";
    return os;
}
