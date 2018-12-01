#include "CatmullClarkSubdivisionStrategy.h"

CatmullClarkSubdivisionStrategy::CatmullClarkSubdivisionStrategy()
{

}

Mesh CatmullClarkSubdivisionStrategy::doSubdivision(Mesh evenMesh){
    Polyhedron initialMesh = evenMesh.convertToSurfaceMesh(true);

    CGAL::Subdivision_method_3::CatmullClark_subdivision(initialMesh, 1);

    Mesh resultMesh = Mesh(initialMesh);

    return resultMesh;
}
