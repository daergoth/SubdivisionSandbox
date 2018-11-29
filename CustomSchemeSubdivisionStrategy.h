#ifndef CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
#define CUSTOMSCHEMESUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class CustomSchemeSubdivisionStrategy : public SubdivisionStrategy
{
public:
    CustomSchemeSubdivisionStrategy();

    SurfaceMesh doSubdivision(SurfaceMesh evenMesh);
};

#endif // CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
