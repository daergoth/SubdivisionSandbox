#ifndef CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
#define CUSTOMSCHEMESUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"

class CustomSchemeSubdivisionStrategy : public SubdivisionStrategy
{
public:
    CustomSchemeSubdivisionStrategy();

    Mesh doSubdivision(Mesh evenMesh);
};

#endif // CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
