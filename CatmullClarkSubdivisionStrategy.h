#ifndef CATMULLCLARKSUBDIVISIONSTRATEGY_H
#define CATMULLCLARKSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class CatmullClarkSubdivisionStrategy : public SubdivisionStrategy
{
public:
    CatmullClarkSubdivisionStrategy();

    SurfaceMesh doSubdivision(SurfaceMesh evenMesh);
};

#endif // CATMULLCLARKSUBDIVISIONSTRATEGY_H
