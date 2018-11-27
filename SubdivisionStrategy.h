#ifndef SUBDIVISIONSTRATEGY_H
#define SUBDIVISIONSTRATEGY_H

#include <vector>

// TODO: temporary until CGAL is included
typedef std::vector<double> Mesh;

class SubdivisionStrategy {
public:
    virtual Mesh doSubdivision(Mesh evenMesh) = 0;

};

#endif // SUBDIVISIONSTRATEGY_H
