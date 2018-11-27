#ifndef BUTTERFLYSUBDIVISIONSTRATEGY_H
#define BUTTERFLYSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class ButterflySubdivisionStrategy : public SubdivisionStrategy
{
public:
    ButterflySubdivisionStrategy();

    Mesh doSubdivision(Mesh evenMesh);
};

#endif // BUTTERFLYSUBDIVISIONSTRATEGY_H
