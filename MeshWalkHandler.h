#ifndef MESHWALKHANDLER_H
#define MESHWALKHANDLER_H

#include <array>
#include <vector>

#include "Mesh.h"
#include "SubdivisionStrategy.h"

class MeshWalkHandler
{
public:

    enum OddsType {Edge, Face};

    class Walk {
    public:
        Walk(std::array<K::Point_3, 16> odds,
             std::vector<K::Point_3> evens,
             int n_odds,
             int n_evens,
             MeshType mesh_type,
             OddsType odds_type,
             SubdivisionType subdivision_type):
            odds(odds),
            evens(evens),
            n_odds(n_odds),
            n_evens(n_evens),
            mesh_type(mesh_type),
            odds_type(odds_type),
            subdivision_type(subdivision_type)
        {}

        int n_odds;
        int n_evens;
        MeshType mesh_type;
        OddsType odds_type;
        SubdivisionType subdivision_type;

        K::Point_3 atOdds(int index) {
            if (index < n_odds) {
                return odds.at(index);
            }
            throw std::exception("Invalid odd vertex index: " + index);
        }

        K::Point_3 atEvens(int index) {
            if (index < n_evens) {
                return evens.at(index);
            }
            throw std::exception("Invalid even vertex index: " + index);
        }

        std::array<K::Point_3, 16> getOdds();

        std::vector<K::Point_3> getEvens() {
            return evens;
        }

    protected:
        std::array<K::Point_3, 16> odds;
        std::vector<K::Point_3> evens;
    };

    class LoopLikeWalk : public Walk {
    public:
        LoopLikeWalk(std::array<K::Point_3, 16> odds, std::vector<K::Point_3> evens):
            Walk(odds, evens, 4, evens.size(), Triangular, Edge, Approximating) {}
    };

    class ButterflyLikeWalk : public Walk {
    public:
        ButterflyLikeWalk(std::array<K::Point_3, 16> odds):
            Walk(odds, std::vector<K::Point_3>(), 8, 0, Triangular, Edge, Interpolating) {}
    };

    class CatmullClarkLikeWalk : public Walk {
    public:
        CatmullClarkLikeWalk(std::array<K::Point_3, 16> odds, std::vector<K::Point_3> evens, OddsType odds_type):
            Walk(odds, evens, odds_type == Face ? 4 : 6, evens.size(), Quadrilateral, odds_type, Approximating) {}
    };

    class KobbeltLikeWalk : public Walk {
    public:
        KobbeltLikeWalk(std::array<K::Point_3, 16> odds, OddsType odds_type):
            Walk(odds, std::vector<K::Point_3>(), odds_type == Face ? 16 : 4, 0, Quadrilateral, odds_type, Interpolating) {}
    };

    static MeshWalkHandler& getInstance() {
        static MeshWalkHandler instance;
        return instance;
    }

    MeshWalkHandler(MeshWalkHandler const&) = delete;
    void operator=(MeshWalkHandler const&) = delete;

    Walk walk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level, SubdivisionType type, OddsType odds_type);
    LoopLikeWalk loopLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level);
    ButterflyLikeWalk butterflyLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level);
    CatmullClarkLikeWalk catmullClarkLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level, OddsType odds_type);
    KobbeltLikeWalk kobbeltLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level, OddsType odds_type);

private:
    MeshWalkHandler() {}

    std::array<K::Point_3, 16> triOddVerticesOneNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);
    std::array<K::Point_3, 16> triOddVerticesTwoNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);
    inline SurfaceMesh::Halfedge_index secondNeighbourTriHelper(std::array<K::Point_3, 16>& vertices, int index, SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);

    std::array<K::Point_3, 16> quadOddVerticesOneNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, OddsType odds_type);
    std::array<K::Point_3, 16> quadOddVerticesTwoNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, OddsType odds_type);

    inline std::array<K::Point_3, 16> quadFaceOddTwo(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);
    inline SurfaceMesh::Halfedge_index quadFaceOddTwoCornerHelper(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies);
    inline SurfaceMesh::Halfedge_index quadFaceOddTwoSideHelper(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies);

    inline std::array<K::Point_3, 16> quadEdgeOddTwo(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);
    inline SurfaceMesh::Halfedge_index quadEdgeOddTwoStraightHelper(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);

    std::vector<K::Point_3> triEvenVertices(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);
    std::vector<K::Point_3> quadEvenVertices(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge);

};

#endif // MESHWALKHANDLER_H
