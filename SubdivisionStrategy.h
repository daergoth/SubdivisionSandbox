#ifndef SUBDIVISIONSTRATEGY_H
#define SUBDIVISIONSTRATEGY_H

#include "Mesh.h"

enum SubdivisionType {Approximating, Interpolating};

class SubdivisionStrategy {
public:
    virtual SurfaceMesh doSubdivision(SurfaceMesh evenMesh) = 0;

};

#endif // SUBDIVISIONSTRATEGY_H
