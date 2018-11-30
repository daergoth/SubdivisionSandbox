#include "CatmullClarkSubdivisionStrategy.h"

CatmullClarkSubdivisionStrategy::CatmullClarkSubdivisionStrategy()
{

}

Mesh CatmullClarkSubdivisionStrategy::doSubdivision(Mesh evenMesh){
    //CGAL::Subdivision_method_3::CatmullClark_subdivision(evenMesh.convertToSurfaceMesh(), 2);
    return evenMesh;
}
