#include "CustomSchemeSubdivisionStrategy.h"

Mesh CustomSchemeSubdivisionStrategy::doSubdivision(Mesh evenMesh) {



    return evenMesh;
}

void CustomSchemeSubdivisionStrategy::setCustomScheme(CustomScheme custom_scheme) {
    this->custom_scheme = custom_scheme;
}
