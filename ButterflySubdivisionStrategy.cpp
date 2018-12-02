#include "ButterflySubdivisionStrategy.h"
#include <QDebug>

#include <CGAL/circulator.h>
#include <CGAL/Subdivision_method_3.h>
#include <CGAL/Subdivision_mask_3.h>
#include <CGAL/Subdivision_method_impl_3.h>

# define PI           3.14159265358979323846

namespace CGAL {
/// The geometry mask of Butterfly subdivision
template <class Poly>
class Butterfly_mask_3 : public CGAL::PQQ_stencil_3<Poly> {
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
    void edge_node_extraordinary(Halfedge_handle edge, Point& pt, int valence) {
        int j = -1;

        pt = Point(FT(3) * edge->vertex()->point().x() / FT(4),
                   FT(3) * edge->vertex()->point().y() / FT(4),
                   FT(3) * edge->vertex()->point().z() / FT(4));

        for (auto it = edge->vertex()->vertex_begin(); j < valence; --it)
        {
            if (it->opposite()->vertex() == edge->opposite()->vertex())
            {
                j = 0;
            }

            if (j >= 0)
            {
                Point cpt = it->opposite()->vertex()->point();

                if (valence == 3)
                {
                    FT weights3[] = { FT(5) / FT(12), FT(-1) / FT(12), FT(-1) / FT(12) };

                    pt = Point(pt.x() + cpt.x() * weights3[j],
                               pt.y() + cpt.y() * weights3[j],
                               pt.z() + cpt.z() * weights3[j]);
                }
                else if (valence == 4)
                {
                    FT weights4[] = { FT(3) / FT(8), FT(0), FT(-1) / FT(8), FT(0) };

                    pt = Point(pt.x() + cpt.x() * weights4[j],
                               pt.y() + cpt.y() * weights4[j],
                               pt.z() + cpt.z() * weights4[j]);
                }
                else if (valence >= 5)
                {
                    FT weight =
                            (
                                FT(1) / FT(4) +
                                cos((FT(2) * FT(PI) * FT(j)) / FT(valence)) +
                                (FT(1) / FT(2)) * cos((FT(4) * FT(PI) * FT(j)) / FT(valence))
                             ) / FT(valence);

                    pt = Point(pt.x() + cpt.x() * weight,
                               pt.y() + cpt.y() * weight,
                               pt.z() + cpt.z() * weight);
                }

                ++j;
            }
        }
    }

    void edge_node_odd(Halfedge_handle edge, Point& pt) {
        Point& p1 = edge->vertex()->point();
        Point& p2 = edge->opposite()->vertex()->point();
        Point& f1 = edge->next()->vertex()->point();
        Point& f2 = edge->opposite()->next()->vertex()->point();

        Point& p3 = edge->next()->next()->opposite()->next()->vertex()->point();
        Point& p4 = edge->next()->opposite()->next()->vertex()->point();
        Point& p5 = edge->opposite()->next()->opposite()->next()->vertex()->point();
        Point& p6 = edge->opposite()->next()->next()->opposite()->next()->vertex()->point();

        pt = Point((p1[0]+p2[0])/2+(f1[0]+f2[0])/8 - (p3[0]+p4[0]+p5[0]+p6[0])/16,
               (p1[1]+p2[1])/2+(f1[1]+f2[1])/8 - (p3[1]+p4[1]+p5[1]+p6[1])/16,
               (p1[2]+p2[2])/2+(f1[2]+f2[2])/8 - (p3[2]+p4[2]+p5[2]+p6[2])/16);
    }

    //
    void edge_node(Halfedge_handle edge, Point& pt) {
        Halfedge_around_vertex_circulator vcir1 = edge->vertex()->vertex_begin();
        size_t n1 = circulator_size(vcir1);

        Halfedge_around_vertex_circulator vcir2 = edge->opposite()->vertex()->vertex_begin();
        size_t n2 = circulator_size(vcir2);

        if (n1==6 && n2==6)
        {
            edge_node_odd(edge, pt);
        }
        else
        {
            Point pt1(0, 0, 0), pt2(0, 0, 0);
            FT div = FT(0);

            if (n1 != 6)
            {
                edge_node_extraordinary(edge, pt1, n1);
                div = div + FT(1);
            }

            if (n2 != 6)
            {
                edge_node_extraordinary(edge->opposite(), pt2, n2);
                div = div + FT(1);
            }

            pt = Point((pt1.x() + pt2.x()) / div,
                       (pt1.y() + pt2.y()) / div,
                       (pt1.z() + pt2.z()) / div);
        }
    }
    //
    void vertex_node(Vertex_handle vertex, Point& pt) {
        pt = vertex->point();
    }
    //
    //void facet_node(Facet_handle facet, Point& pt) {};
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

// ======================================================================
namespace Subdivision_method_3 {
    //
    template <class Polyhedron>
    void Butterfly_subdivision(Polyhedron& p, int step = 1) {
        PTQ(p, Butterfly_mask_3<Polyhedron>() , step);
    }
}

} //namespace CGAL

ButterflySubdivisionStrategy::ButterflySubdivisionStrategy()
{

}

Mesh ButterflySubdivisionStrategy::doSubdivision(Mesh evenMesh) {
    Polyhedron initialMesh = evenMesh.convertToSurfaceMesh(true);

    CGAL::Subdivision_method_3::Butterfly_subdivision(initialMesh, 1);

    Mesh resultMesh = Mesh(initialMesh);

    return resultMesh;
}
