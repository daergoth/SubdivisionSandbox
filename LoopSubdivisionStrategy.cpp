#include "LoopSubdivisionStrategy.h"

LoopSubdivisionStrategy::LoopSubdivisionStrategy()
{

}

Mesh LoopSubdivisionStrategy::doSubdivision(Mesh evenMesh) {
    Polyhedron initialMesh = evenMesh.convertToSurfaceMesh();

    CGAL::Subdivision_method_3::Loop_subdivision(initialMesh, 1);

    Mesh resultMesh = Mesh(initialMesh);

    return resultMesh;
}
