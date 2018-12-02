#include "KobbeltSubdivisionStrategy.h"

namespace CGAL {
/// The geometry mask of Kobbelt subdivision
template <class Poly>
class Kobbelt_mask_3 : public CGAL::Linear_mask_3<Poly> {
public:
    typedef Poly                                        Polyhedron;

    typedef typename Polyhedron::Vertex_handle           Vertex_handle;
    typedef typename Polyhedron::Halfedge_handle         Halfedge_handle;
    typedef typename Polyhedron::Facet_handle            Facet_handle;

    typedef typename Polyhedron::Halfedge_around_facet_circulator
                                            Halfedge_around_facet_circulator;
    typedef typename Polyhedron::Halfedge_around_vertex_circulator
                                            Halfedge_around_vertex_circulator;

    typedef typename Polyhedron::Traits                  Traits;
    typedef typename Traits::Kernel                      Kernel;

    typedef typename Kernel::FT                          FT;
    typedef typename Kernel::Point_3                     Point;
    typedef typename Kernel::Vector_3                    Vector;

public:
    void edge_node(Halfedge_handle edge, Point& pt) {
        Point& p1 = edge->vertex()->point();
        Point& p2 = edge->opposite()->vertex()->point();
        Point& f1 = edge->next()->opposite()->next()->vertex()->point();
        Point& f2 = edge->opposite()->next()->opposite()->next()->vertex()->point();

        pt = Point(9*(p1[0]+p2[0])/16-(f1[0]+f2[0])/16,
               9*(p1[1]+p2[1])/16-(f1[1]+f2[1])/16,
               9*(p1[2]+p2[2])/16-(f1[2]+f2[2])/16);
    }

    void vertex_node(Vertex_handle vertex, Point& pt) {
        pt = vertex->point();
    }
    //
    void facet_node(Facet_handle facet, Point& pt) {
        Halfedge_handle edge = facet->halfedge();

        Point& p1 = edge->vertex()->point();
        Point& p2 = edge->next()->vertex()->point();
        Point& p3 = edge->next()->next()->vertex()->point();
        Point& p4 = edge->next()->next()->next()->vertex()->point();

        Point& p5 = edge->opposite()->next()->next()->next()->opposite()->next()->vertex()->point();
        Point& p6 = edge->next()->opposite()->next()->vertex()->point();
        Point& p7 = edge->next()->opposite()->next()->next()->vertex()->point();
        Point& p8 = edge->next()->next()->opposite()->next()->opposite()->next()->next()->vertex()->point();

        Point& p9 = edge->next()->next()->opposite()->next()->vertex()->point();
        Point& p10 = edge->next()->next()->opposite()->next()->next()->vertex()->point();
        Point& p11 = edge->next()->next()->next()->opposite()->next()->opposite()->next()->next()->vertex()->point();
        Point& p12 = edge->next()->next()->next()->opposite()->next()->vertex()->point();

        Point& p13 = edge->opposite()->next()->opposite()->next()->vertex()->point();
        Point& p14 = edge->opposite()->next()->opposite()->next()->next()->vertex()->point();
        Point& p15 = edge->opposite()->next()->vertex()->point();
        Point& p16 = edge->opposite()->next()->next()->vertex()->point();

        float x = 81*(p1[0]+p2[0]+p3[0]+p4[0])/256 +
                1*(p5[0]+p8[0]+p11[0]+p14[0])/256 +
                (-9)*(p6[0]+p7[0]+p9[0]+p10[0]+p12[0]+p13[0]+p15[0]+p16[0])/256;

        float y = 81*(p1[1]+p2[1]+p3[1]+p4[1])/256 +
                1*(p5[1]+p8[1]+p11[1]+p14[1])/256 +
                (-9)*(p6[1]+p7[1]+p9[1]+p10[1]+p12[1]+p13[1]+p15[1]+p16[1])/256;

        float z = 81*(p1[2]+p2[2]+p3[2]+p4[2])/256 +
                1*(p5[2]+p8[2]+p11[2]+p14[2])/256 +
                (-9)*(p6[2]+p7[2]+p9[2]+p10[2]+p12[2]+p13[2]+p15[2]+p16[2])/256;

        pt = Point(x,y,z);
    }
    //
    void border_node(Halfedge_handle edge, Point& ept, Point& vpt) {
        /*
        Point& ep1 = edge->vertex()->point();
        Point& ep2 = edge->opposite()->vertex()->point();
        ept = Point((ep1[0]+ep2[0])/2, (ep1[1]+ep2[1])/2, (ep1[2]+ep2[2])/2);

        Halfedge_around_vertex_circulator vcir = edge->vertex_begin();
        Point& vp1  = vcir->opposite()->vertex()->point();
        Point& vp0  = vcir->vertex()->point();
        Point& vp_1 = (--vcir)->opposite()->vertex()->point();
        vpt = Point((vp_1[0] + 6*vp0[0] + vp1[0])/8,
            (vp_1[1] + 6*vp0[1] + vp1[1])/8,
            (vp_1[2] + 6*vp0[2] + vp1[2])/8 );
        */
    }
    };

    namespace Subdivision_method_3 {
        //
        template <class Polyhedron>
        void Kobbelt_subdivision(Polyhedron& p, int step = 1) {
            PQQ(p, Kobbelt_mask_3<Polyhedron>() , step);
        }
    }
}//namespace CGAL

KobbeltSubdivisionStrategy::KobbeltSubdivisionStrategy()
{

}

Mesh KobbeltSubdivisionStrategy::doSubdivision(Mesh evenMesh) {
    Polyhedron initialMesh = evenMesh.convertToSurfaceMesh(false);

    CGAL::Subdivision_method_3::Kobbelt_subdivision(initialMesh, 1);

    Mesh resultMesh = Mesh(initialMesh);

    return resultMesh;
}
