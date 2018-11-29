#ifndef BUTTERFLYSUBDIVISIONSTRATEGY_H
#define BUTTERFLYSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class ButterflySubdivisionStrategy : public SubdivisionStrategy
{
public:
    ButterflySubdivisionStrategy();

    SurfaceMesh doSubdivision(SurfaceMesh evenMesh);
};

#endif // BUTTERFLYSUBDIVISIONSTRATEGY_H
