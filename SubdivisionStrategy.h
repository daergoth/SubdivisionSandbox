#ifndef SUBDIVISIONSTRATEGY_H
#define SUBDIVISIONSTRATEGY_H

#include <vector>
#include <CGAL/Subdivision_method_3.h>
#include <CGAL/Surface_mesh.h>

#include "Mesh.h"

enum SubdivisionType {Approximating, Interpolating};

class SubdivisionStrategy {
public:
    virtual Mesh doSubdivision(Mesh evenMesh) = 0;

};

#endif // SUBDIVISIONSTRATEGY_H
