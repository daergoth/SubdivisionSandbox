#ifndef LOOPSUBDIVISIONSTRATEGY_H
#define LOOPSUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class LoopSubdivisionStrategy : public SubdivisionStrategy
{
public:
    LoopSubdivisionStrategy();

    Mesh doSubdivision(Mesh evenMesh);
};

#endif // LOOPSUBDIVISIONSTRATEGY_H
