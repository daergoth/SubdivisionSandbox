#include "MeshWalkHandler.h"

std::vector<K::Point_3> MeshWalkHandler::walk(SurfaceMesh mesh, int neighbour_level) {
    switch (neighbour_level) {
    case 0:

        break;
    case 1:

        break;
    default:
        throw std::exception("Higher neighbour levels than 2 are not supported!");
        break;
    }
}

MeshWalkHandler::Walk MeshWalkHandler::walk(SurfaceMesh mesh, int neighbour_level, SubdivisionType type) {

}

MeshWalkHandler::LoopLikeWalk MeshWalkHandler::loopLikeWalk(SurfaceMesh mesh, int neighbour_level) {

}

MeshWalkHandler::ButterflyLikeWalk MeshWalkHandler::butterflyLikeWalk(SurfaceMesh mesh, int neighbour_level) {

}

MeshWalkHandler::CatmullClarkLikeWalk MeshWalkHandler::catmullClarkLikeWalk(SurfaceMesh mesh, int neighbour_level) {

}

MeshWalkHandler::KobbeltLikeWalk MeshWalkHandler::kobbeltLikeWalk(SurfaceMesh mesh, int neighbour_level) {

}
