#ifndef KOBBELTSUBDIVISIONSTRATEGY_H
#define KOBBELTSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class KobbeltSubdivisionStrategy : public SubdivisionStrategy
{
public:
    KobbeltSubdivisionStrategy();

    SurfaceMesh doSubdivision(SurfaceMesh evenMesh);
};

#endif // KOBBELTSUBDIVISIONSTRATEGY_H
