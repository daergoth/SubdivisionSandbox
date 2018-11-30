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

    Walk walk(Polyhedron::Halfedge_iterator& halfedge, int neighbour_level, SubdivisionType subdivision_type, OddsType odds_type, MeshType mesh_type);
    LoopLikeWalk loopLikeWalk(Polyhedron::Halfedge_iterator& halfedge);
    ButterflyLikeWalk butterflyLikeWalk(Polyhedron::Halfedge_iterator& halfedge);
    CatmullClarkLikeWalk catmullClarkLikeWalk(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type);
    KobbeltLikeWalk kobbeltLikeWalk(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type);

private:
    MeshWalkHandler() {}

    std::array<K::Point_3, 16> triOddVerticesOneNeighbour(Polyhedron::Halfedge_iterator& halfedge);
    std::array<K::Point_3, 16> triOddVerticesTwoNeighbour(Polyhedron::Halfedge_iterator& halfedge);
    inline Polyhedron::Halfedge_iterator secondNeighbourTriHelper(std::array<K::Point_3, 16>& vertices, int index, Polyhedron::Halfedge_iterator& halfedge);

    std::array<K::Point_3, 16> quadOddVerticesOneNeighbour(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type);
    std::array<K::Point_3, 16> quadOddVerticesTwoNeighbour(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type);

    inline std::array<K::Point_3, 16> quadFaceOddTwo(Polyhedron::Halfedge_iterator& halfedge);
    inline Polyhedron::Halfedge_iterator quadFaceOddTwoCornerHelper(Polyhedron::Halfedge_iterator& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies);
    inline Polyhedron::Halfedge_iterator quadFaceOddTwoSideHelper(Polyhedron::Halfedge_iterator& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies);

    inline std::array<K::Point_3, 16> quadEdgeOddTwo(Polyhedron::Halfedge_iterator& halfedge);
    inline Polyhedron::Halfedge_iterator quadEdgeOddTwoStraightHelper(Polyhedron::Halfedge_iterator& halfedge);

    std::vector<K::Point_3> triEvenVertices(Polyhedron::Halfedge_iterator& halfedge);
    std::vector<K::Point_3> quadEvenVertices(Polyhedron::Halfedge_iterator& halfedge);

};

#endif // MESHWALKHANDLER_H
