#ifndef CATMULLCLARKSUBDIVISIONSTRATEGY_H
#define CATMULLCLARKSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class CatmullClarkSubdivisionStrategy : public SubdivisionStrategy
{
public:
    CatmullClarkSubdivisionStrategy();

    Mesh doSubdivision(Mesh evenMesh);
};

#endif // CATMULLCLARKSUBDIVISIONSTRATEGY_H
