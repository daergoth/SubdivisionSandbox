#include "customscheme.h"

CustomScheme::MeshType CustomScheme::getMeshType() {
    return mesh_type;
}
void CustomScheme::setMeshType(CustomScheme::MeshType mesh_type) {
    this->mesh_type = mesh_type;
}

CustomScheme::RefinementType CustomScheme::getRefinementType() {
    return refinement_type;
}
void CustomScheme::getMeshType(CustomScheme::RefinementType refinement_type) {
    this->refinement_type = refinement_type;
}

int CustomScheme::getNeighbourLevel() {
    return neighbour_level;
}
void CustomScheme::setNeighbourLevel(int neighbour_level) {
    this->neighbour_level = neighbour_level;
}

CustomScheme::Weights CustomScheme::getWeights() {
    return weights;
}
void CustomScheme::setWeights(CustomScheme::Weights weights) {
    this->weights = weights;
}
