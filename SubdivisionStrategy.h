#ifndef SUBDIVISIONSTRATEGY_H
#define SUBDIVISIONSTRATEGY_H

#include "Mesh.h"

class SubdivisionStrategy {
public:
    virtual Mesh doSubdivision(Mesh evenMesh) = 0;

};

#endif // SUBDIVISIONSTRATEGY_H
