#ifndef KOBBELTSUBDIVISIONSTRATEGY_H
#define KOBBELTSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class KobbeltSubdivisionStrategy : public SubdivisionStrategy
{
public:
    KobbeltSubdivisionStrategy();

    Mesh doSubdivision(Mesh evenMesh);
};

#endif // KOBBELTSUBDIVISIONSTRATEGY_H
