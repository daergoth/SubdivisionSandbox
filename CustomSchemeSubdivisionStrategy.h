#ifndef CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
#define CUSTOMSCHEMESUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"
#include "CustomScheme.h"

class CustomSchemeSubdivisionStrategy : public SubdivisionStrategy
{
public:
    CustomSchemeSubdivisionStrategy(CustomScheme custom_scheme):
        custom_scheme(custom_scheme) {}

    Mesh doSubdivision(Mesh evenMesh);

    void setCustomScheme(CustomScheme custom_scheme);

private:
    CustomScheme custom_scheme;
};

#endif // CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
