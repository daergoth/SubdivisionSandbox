#include "KobbeltSubdivisionStrategy.h"

KobbeltSubdivisionStrategy::KobbeltSubdivisionStrategy()
{

}

Mesh KobbeltSubdivisionStrategy::doSubdivision(Mesh evenMesh) {
    Polyhedron initialMesh = evenMesh.convertToSurfaceMesh(true);

    CGAL::Subdivision_method_3::Sqrt3_subdivision(initialMesh, 1);

    Mesh resultMesh = Mesh(initialMesh);

    return resultMesh;
}
