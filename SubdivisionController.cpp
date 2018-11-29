#include "SubdivisionController.h"

SubdivisionController::SubdivisionController() {
    strategyMap[SubdivisionScheme::Loop] = new LoopSubdivisionStrategy();
    strategyMap[SubdivisionScheme::Butterfly] = new ButterflySubdivisionStrategy();
    strategyMap[SubdivisionScheme::CatmullClark] = new CatmullClarkSubdivisionStrategy();
    strategyMap[SubdivisionScheme::Kobbelt] = new KobbeltSubdivisionStrategy();
    strategyMap[SubdivisionScheme::Custom] = new CustomSchemeSubdivisionStrategy();
}

SubdivisionController::~SubdivisionController() {
    for (std::pair<SubdivisionScheme, SubdivisionStrategy*> strategy : strategyMap) {
        delete strategy.second;
    }
}

void SubdivisionController::switchTo(SubdivisionScheme scheme) {
    std::cout << "Switching to scheme: " << scheme << std::endl;
    currentStrategy = strategyMap[scheme];
    SurfaceMesh baseMesh = meshHistory[0];
    meshHistory.clear();
    meshHistory.push_back(baseMesh);
    currentMeshIndex = 0;
}

void SubdivisionController::doSubdivision() {
    if (currentMeshIndex < meshHistory.size() - 1) {
        ++currentMeshIndex;
    } else {
        SurfaceMesh oddMesh = currentStrategy->doSubdivision(meshHistory.back());
        meshHistory.push_back(oddMesh);
        ++currentMeshIndex;
    }
}

void SubdivisionController::doBackwardStep() {
    currentMeshIndex = --currentMeshIndex >= 0 ? --currentMeshIndex : 0;
}

SurfaceMesh SubdivisionController::getCurrentMesh() {
    return meshHistory[currentMeshIndex];
}

void SubdivisionController::setBaseMesh(SurfaceMesh baseMesh) {
    if (meshHistory.size() > 0) {
        meshHistory[0] = baseMesh;
    } else {
        meshHistory.push_back(baseMesh);
    }
}
