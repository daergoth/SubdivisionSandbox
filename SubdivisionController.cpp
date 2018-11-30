#include "SubdivisionController.h"

SubdivisionController::SubdivisionController() {
    strategyMap[SubdivisionScheme::Loop] = new LoopSubdivisionStrategy();
    strategyMap[SubdivisionScheme::Butterfly] = new ButterflySubdivisionStrategy();
    strategyMap[SubdivisionScheme::CatmullClark] = new CatmullClarkSubdivisionStrategy();
    strategyMap[SubdivisionScheme::Kobbelt] = new KobbeltSubdivisionStrategy();
    strategyMap[SubdivisionScheme::Custom] = nullptr;
}

SubdivisionController::~SubdivisionController() {
    for (std::pair<SubdivisionScheme, SubdivisionStrategy*> strategy : strategyMap) {
        delete strategy.second;
    }
}

void SubdivisionController::switchTo(SubdivisionScheme scheme) {
    std::cout << "Switching to scheme: " << scheme << std::endl;

    if (scheme == SubdivisionScheme::Custom) {
        CustomSchemeHandler& csh = CustomSchemeHandler::getInstance();
        std::shared_ptr<CustomScheme> sptr_scheme = csh.getCurrentCustomScheme();

        if (strategyMap[SubdivisionScheme::Custom] == nullptr) {
            if (sptr_scheme) {
                strategyMap[SubdivisionScheme::Custom] = new CustomSchemeSubdivisionStrategy(*sptr_scheme);
            } else {
                throw std::exception("Cannot switch to CustomScheme while there are no weights for it in CustomSchemeHandler!");
            }
        } else {
            if (sptr_scheme) {
                ((CustomSchemeSubdivisionStrategy*)strategyMap[SubdivisionScheme::Custom])->setCustomScheme(*sptr_scheme);
            } else {
                throw std::exception("Cannot switch to CustomScheme while there are no weights for it in CustomSchemeHandler!");
            }
        }
    }

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
        if (currentStrategy != nullptr) {
            SurfaceMesh oddMesh = currentStrategy->doSubdivision(meshHistory.back());
            meshHistory.push_back(oddMesh);
            ++currentMeshIndex;
        } else {
            throw std::exception("Invalid scheme switch (possibly custom scheme has no weights)!");
        }
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
    currentMeshIndex = 0;
}
