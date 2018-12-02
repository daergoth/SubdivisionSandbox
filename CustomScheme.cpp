#include "CustomScheme.h"

// OddWeight
void to_json(json& j, const OddWeight& ow) {
    j = json{{"level", ow.level}, {"type", ow.type}, {"value", ow.value}};
}
void from_json(const json& j, OddWeight& ow) {
    j.at("level").get_to(ow.level);
    j.at("type").get_to(ow.type);
    j.at("value").get_to(ow.value);
}
std::ostream& operator<<(std::ostream& os, const OddWeight& ow) {
    // write obj to stream
    os << "[level:" << ow.level
       << ", type:" << std::to_string(ow.type)
       << ", value:" << std::to_string(ow.value) << "]";
    return os;
}


// EvenWeights
void to_json(json& j, const EvenWeights& ew) {
    j = json{{"alfa", ew.alfa}, {"beta", ew.beta}};
}
void from_json(const nlohmann::json& j, EvenWeights& ew) {
    j.at("alfa").get_to(ew.alfa);
    j.at("beta").get_to(ew.beta);
}
std::ostream& operator<<(std::ostream& os, const EvenWeights& ew) {
    // write obj to stream
    os << "[alfa:" << std::to_string(ew.alfa)
       << ", beta:" << std::to_string(ew.beta) << "]";
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
        {"name", s.name},
        {"meshtype", s.mesh_type},
        {"refinementtype", s.refinement_type},
        {"neighbourlevel", s.neighbour_level},
        {"weights", s.weights}
    };
}
void from_json(const json& j, CustomScheme& s) {
    s.name = j.at("name");
    s.mesh_type = j.at("meshtype");
    s.refinement_type = j.at("refinementtype");
    s.neighbour_level = j.at("neighbourlevel");
    s.weights = j.at("weights");
}
std::ostream& operator<<(std::ostream& os, const CustomScheme& s) {
    os << "[name:" << s.name
       << ", mesh_type:" << s.mesh_type
       << ", refinement_type:" << s.refinement_type
       << ", neighbour_level:" << s.neighbour_level
       << ", weights:" << s.weights << "]";
    return os;
}
