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
            odds(odds),
            evens(evens),
            n_odds(4),
            n_evens(evens.size()),
            mesh_type(Triangular),
            odds_type(Edge),
            subdivision_type(Approximating)
        {}
    };

    class ButterflyLikeWalk : public Walk {
    public:
        ButterflyLikeWalk(std::array<K::Point_3, 16> odds):
            odds(odds),
            n_odds(8),
            n_evens(0),
            mesh_type(Triangular),
            odds_type(Edge),
            subdivision_type(Interpolating)
        {}
    };

    class CatmullClarkLikeWalk : public Walk {
    public:
        CatmullClarkLikeWalk(std::array<K::Point_3, 16> odds, std::vector<K::Point_3> evens, OddsType odds_type):
            odds(odds),
            evens(evens),
            n_odds(odds_type == Face ? 4 : 6),
            n_evens(evens.size()),
            mesh_type(Quadrilateral),
            odds_type(odds_type),
            subdivision_type(Approximating)
        {}
    };

    class KobbeltLikeWalk : public Walk {
    public:
        KobbeltLikeWalk(std::array<K::Point_3, 16> odds, OddsType odds_type):
            odds(odds),
            n_odds(odds_type == Face ? 16 : 4),
            n_evens(0),
            mesh_type(Quadrilateral),
            odds_type(odds_type),
            subdivision_type(Interpolating)
        {}
    };

    static MeshWalkHandler& getInstance() {
        static MeshWalkHandler instance;
        return instance;
    }

    MeshWalkHandler(MeshWalkHandler const&) = delete;
    void operator=(MeshWalkHandler const&) = delete;

    Walk walk(SurfaceMesh:: mesh, long edge_index, int neighbour_level, SubdivisionType type);
    LoopLikeWalk loopLikeWalk(SurfaceMesh mesh, long edge_index, int neighbour_level);
    ButterflyLikeWalk butterflyLikeWalk(SurfaceMesh mesh, long edge_index, int neighbour_level);
    CatmullClarkLikeWalk catmullClarkLikeWalk(SurfaceMesh mesh, long edge_index, int neighbour_level, OddsType odds_type);
    KobbeltLikeWalk kobbeltLikeWalk(SurfaceMesh mesh, long edge_index, int neighbour_level, OddsType odds_type);

private:
    MeshWalkHandler() {}

};

#endif // MESHWALKHANDLER_H
