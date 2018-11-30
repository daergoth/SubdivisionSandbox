#include "CustomSchemeSubdivisionStrategy.h"

SurfaceMesh CustomSchemeSubdivisionStrategy::doSubdivision(SurfaceMesh evenMesh) {
    return evenMesh;
}

void CustomSchemeSubdivisionStrategy::setCustomScheme(CustomScheme custom_scheme) {
    this->custom_scheme = custom_scheme;
}
