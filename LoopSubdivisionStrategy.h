#ifndef LOOPSUBDIVISIONSTRATEGY_H
#define LOOPSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class LoopSubdivisionStrategy : public SubdivisionStrategy
{
public:
    LoopSubdivisionStrategy();

    SurfaceMesh doSubdivision(SurfaceMesh evenMesh);
};

#endif // LOOPSUBDIVISIONSTRATEGY_H
