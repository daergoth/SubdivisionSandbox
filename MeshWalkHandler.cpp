#include "MeshWalkHandler.h"

MeshWalkHandler::Walk MeshWalkHandler::walk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level, SubdivisionType type, OddsType odds_type) {
    throw std::exception("Unimplemented!");
}

MeshWalkHandler::LoopLikeWalk MeshWalkHandler::loopLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level) {
    throw std::exception("Unimplemented!");
}

MeshWalkHandler::ButterflyLikeWalk MeshWalkHandler::butterflyLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level) {
    throw std::exception("Unimplemented!");
}

MeshWalkHandler::CatmullClarkLikeWalk MeshWalkHandler::catmullClarkLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level, OddsType odds_type) {
    throw std::exception("Unimplemented!");
}

MeshWalkHandler::KobbeltLikeWalk MeshWalkHandler::kobbeltLikeWalk(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, int neighbour_level, OddsType odds_type) {
    throw std::exception("Unimplemented!");
}

std::array<K::Point_3, 16> MeshWalkHandler::triOddVerticesOneNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    std::array<K::Point_3, 16> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;

    vertices[2] = sm.point(sm.target(c));
    vertices[1] = sm.point(sm.source(c));
    c = sm.next(c);
    vertices[0] = sm.point(sm.target(c));
    c = sm.prev(c);
    c = sm.opposite(c);
    c = sm.next(c);
    vertices[3] = sm.point(sm.target(c));

    return vertices;
}

std::array<K::Point_3, 16> MeshWalkHandler::triOddVerticesTwoNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    std::array<K::Point_3, 16> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;

    vertices[4] = sm.point(sm.target(c));

    c = sm.next(c);
    c = secondNeighbourTriHelper(vertices, 2, sm, c);
    vertices[1] = sm.point(sm.target(c));
    c = sm.next(c);
    c = secondNeighbourTriHelper(vertices, 0, sm, c);
    vertices[3] = sm.point(sm.target(c));

    c = sm.next(c);
    c = sm.opposite(c);

    c = sm.next(c);
    c = secondNeighbourTriHelper(vertices, 5, sm, c);
    vertices[6] = sm.point(sm.target(c));
    c = sm.next(c);
    c = secondNeighbourTriHelper(vertices, 7, sm, c);

    return vertices;
}

inline SurfaceMesh::Halfedge_index MeshWalkHandler::secondNeighbourTriHelper(std::array<K::Point_3, 16>& vertices, int index, SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    halfedge = sm.opposite(halfedge);
    halfedge = sm.next(halfedge);
    vertices[index] = sm.point(sm.target(halfedge));
    halfedge = sm.prev(halfedge);
    return sm.opposite(halfedge);
}

std::array<K::Point_3, 16> MeshWalkHandler::quadOddVerticesOneNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, OddsType odds_type) {
    std::array<K::Point_3, 16> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;

    vertices[3] = sm.point(sm.target(c));
    c = sm.next(c);
    vertices[1] = sm.point(sm.target(c));
    c = sm.next(c);
    vertices[0] = sm.point(sm.target(c));
    c = sm.next(c);
    vertices[2] = sm.point(sm.target(c));

    if (odds_type == Edge) {
        c = sm.next(c);
        c = sm.opposite(c);
        c = sm.next(c);
        vertices[4] = sm.point(sm.target(c));
        c = sm.next(c);
        vertices[5] = sm.point(sm.target(c));
    }

    return vertices;
}

std::array<K::Point_3, 16> MeshWalkHandler::quadOddVerticesTwoNeighbour(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, OddsType odds_type) {
    switch (odds_type) {
    case Face:
        return quadFaceOddTwo(sm, halfedge);
        break;
    case Edge:
        return quadEdgeOddTwo(sm, halfedge);
        break;
    default:
        throw std::exception("Unknown OddsType: " + odds_type);
    }
}

inline std::array<K::Point_3, 16> MeshWalkHandler::quadFaceOddTwo(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    std::array<K::Point_3, 16> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;

    c = sm.next(c);
    c = sm.opposite(c);
    c = sm.next(c);

    c = quadFaceOddTwoSideHelper(sm, c, vertices, std::pair<int, int>(7, 6));
    c = quadFaceOddTwoCornerHelper(sm, c, vertices, std::pair<int,int>(3, 2));
    c = quadFaceOddTwoSideHelper(sm, c, vertices, std::pair<int, int>(1, 5));
    c = quadFaceOddTwoCornerHelper(sm, c, vertices, std::pair<int, int>(0, 4));
    c = quadFaceOddTwoSideHelper(sm, c, vertices, std::pair<int, int>(8, 9));
    c = quadFaceOddTwoCornerHelper(sm, c, vertices, std::pair<int, int>(12, 13));
    c = quadFaceOddTwoSideHelper(sm, c, vertices, std::pair<int, int>(14, 10));
    c = quadFaceOddTwoCornerHelper(sm, c, vertices, std::pair<int, int>(15, 11));

    return vertices;
}

inline SurfaceMesh::Halfedge_index MeshWalkHandler::quadFaceOddTwoCornerHelper(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies) {
    halfedge = sm.next(halfedge);
    vertices[indicies.first] = sm.point(sm.target(halfedge));
    halfedge = sm.next(halfedge);
    vertices[indicies.second] = sm.point(sm.target(halfedge));
    halfedge = sm.next(halfedge);
    halfedge = sm.opposite(halfedge);
    return halfedge;
}

inline SurfaceMesh::Halfedge_index MeshWalkHandler::quadFaceOddTwoSideHelper(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies) {
    halfedge = sm.next(halfedge);
    vertices[indicies.first] = sm.point(sm.target(halfedge));
    halfedge = sm.next(halfedge);
    vertices[indicies.second] = sm.point(sm.target(halfedge));
    halfedge = sm.opposite(halfedge);
    return halfedge;
}

inline std::array<K::Point_3, 16> MeshWalkHandler::quadEdgeOddTwo(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    std::array<K::Point_3, 16> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;

    vertices[1] = sm.point(sm.source(c));
    vertices[2] = sm.point(sm.target(c));

    c = quadEdgeOddTwoStraightHelper(sm, c);

    vertices[3] = sm.point(sm.target(c));

    c = sm.opposite(c);
    c = quadEdgeOddTwoStraightHelper(sm, c);
    c = quadEdgeOddTwoStraightHelper(sm, c);

    vertices[0] = sm.point(sm.target(c));

    return vertices;
}

inline SurfaceMesh::Halfedge_index MeshWalkHandler::quadEdgeOddTwoStraightHelper(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    halfedge = sm.next(halfedge);
    halfedge = sm.opposite(halfedge);
    halfedge = sm.next(halfedge);

    return halfedge;
}


std::vector<K::Point_3> MeshWalkHandler::triEvenVertices(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    std::vector<K::Point_3> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;
    vertices.push_back(sm.point(sm.source(c)));

    do {
        vertices.push_back(sm.point(sm.target(c)));
        c = sm.next(c);
        c = sm.next(c);
        c = sm.opposite(c);
    } while(c != halfedge);

    return vertices;
}

std::vector<K::Point_3> MeshWalkHandler::quadEvenVertices(SurfaceMesh& sm, SurfaceMesh::Halfedge_index& halfedge) {
    std::vector<K::Point_3> vertices;

    SurfaceMesh::Halfedge_index& c = halfedge;
    vertices.push_back(sm.point(sm.source(c)));

    do {
        vertices.push_back(sm.point(sm.target(c)));
        c = sm.next(c);
        vertices.push_back(sm.point(sm.target(c)));
        c = sm.next(c);
        c = sm.next(c);
        c = sm.opposite(c);
    } while(c != halfedge);

    return vertices;
}
