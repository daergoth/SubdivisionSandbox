#include "MeshWalkHandler.h"

MeshWalkHandler::Walk MeshWalkHandler::walk(Polyhedron::Halfedge_iterator& halfedge, int neighbour_level, SubdivisionType subdivision_type, OddsType odds_type, MeshType mesh_type) {
    std::array<K::Point_3, 16> odds;
    std::vector<K::Point_3> evens;
    switch (mesh_type) {
    case Triangular:

        switch (neighbour_level) {
        case 1:
            odds = triOddVerticesOneNeighbour(halfedge);
            break;
        case 2:
            odds = triOddVerticesTwoNeighbour(halfedge);
            break;
        default:
            throw std::exception("Unsupported neighbour level: " + neighbour_level);
            break;
        }

        if (subdivision_type == Approximating) {
            evens = triEvenVertices(halfedge);
        }

        break;
    case Quadrilateral:

        switch (neighbour_level) {
        case 1:
            odds = quadOddVerticesOneNeighbour(halfedge, odds_type);
            break;
        case 2:
            odds = quadOddVerticesTwoNeighbour(halfedge, odds_type);
            break;
        default:
            throw std::exception("Unsupported neighbour level: " + neighbour_level);
            break;
        }

        if (subdivision_type == Approximating) {
            evens = quadEvenVertices(halfedge);
        }

        break;
    default:
        throw std::exception("Unsupported MeshType: " + mesh_type);
        break;
    }

    return Walk(odds, evens, odds.size(), evens.size(), mesh_type, odds_type, subdivision_type);
}

MeshWalkHandler::LoopLikeWalk MeshWalkHandler::loopLikeWalk(Polyhedron::Halfedge_iterator& halfedge) {
    std::array<K::Point_3, 16> odds = triOddVerticesOneNeighbour(halfedge);
    std::vector<K::Point_3> evens = triEvenVertices(halfedge);
    return LoopLikeWalk(odds, evens);
}

MeshWalkHandler::ButterflyLikeWalk MeshWalkHandler::butterflyLikeWalk(Polyhedron::Halfedge_iterator& halfedge) {
    std::array<K::Point_3, 16> odds = triOddVerticesTwoNeighbour(halfedge);
    return ButterflyLikeWalk(odds);
}

MeshWalkHandler::CatmullClarkLikeWalk MeshWalkHandler::catmullClarkLikeWalk(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type) {
    std::array<K::Point_3, 16> odds = quadOddVerticesOneNeighbour(halfedge, odds_type);
    std::vector<K::Point_3> evens = quadEvenVertices(halfedge);
    return CatmullClarkLikeWalk(odds, evens, odds_type);
}

MeshWalkHandler::KobbeltLikeWalk MeshWalkHandler::kobbeltLikeWalk(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type) {
    std::array<K::Point_3, 16> odds = quadOddVerticesTwoNeighbour(halfedge, odds_type);
    return KobbeltLikeWalk(odds, odds_type);
}

std::array<K::Point_3, 16> MeshWalkHandler::triOddVerticesOneNeighbour(Polyhedron::Halfedge_iterator& halfedge) {
    std::array<K::Point_3, 16> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;

    vertices[2] = c->vertex()->point();
    vertices[1] = c->prev()->vertex()->point();
    c = c->next();
    vertices[0] = c->vertex()->point();
    c = c->prev()->opposite()->next();
    vertices[3] = c->vertex()->point();

    return vertices;
}

std::array<K::Point_3, 16> MeshWalkHandler::triOddVerticesTwoNeighbour(Polyhedron::Halfedge_iterator& halfedge) {
    std::array<K::Point_3, 16> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;

    vertices[4] = c->vertex()->point();

    c = c->next();
    c = secondNeighbourTriHelper(vertices, 2, c);
    vertices[1] = c->vertex()->point();
    c = c->next();
    c = secondNeighbourTriHelper(vertices, 0, c);
    vertices[3] = c->vertex()->point();

    c = c->next()->opposite()->next();

    c = secondNeighbourTriHelper(vertices, 5, c);
    vertices[6] = c->vertex()->point();
    c = c->next();
    c = secondNeighbourTriHelper(vertices, 7, c);

    return vertices;
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::secondNeighbourTriHelper(std::array<K::Point_3, 16>& vertices, int index, Polyhedron::Halfedge_iterator& halfedge) {
    halfedge = halfedge->opposite()->next();
    vertices[index] = halfedge->vertex()->point();
    return halfedge->prev()->opposite();
}

std::array<K::Point_3, 16> MeshWalkHandler::quadOddVerticesOneNeighbour(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type) {
    std::array<K::Point_3, 16> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;

    vertices[3] = c->vertex()->point();
    c = c->next();
    vertices[1] = c->vertex()->point();
    c = c->next();
    vertices[0] = c->vertex()->point();
    c = c->next();
    vertices[2] = c->vertex()->point();

    if (odds_type == Edge) {
        c = c->next()->opposite()->next();
        vertices[4] = c->vertex()->point();
        c = c->next();
        vertices[5] = c->vertex()->point();
    }

    return vertices;
}

std::array<K::Point_3, 16> MeshWalkHandler::quadOddVerticesTwoNeighbour(Polyhedron::Halfedge_iterator& halfedge, OddsType odds_type) {
    switch (odds_type) {
    case Face:
        return quadFaceOddTwo(halfedge);
        break;
    case Edge:
        return quadEdgeOddTwo(halfedge);
        break;
    default:
        throw std::exception("Unknown OddsType: " + odds_type);
    }
}

inline std::array<K::Point_3, 16> MeshWalkHandler::quadFaceOddTwo(Polyhedron::Halfedge_iterator& halfedge) {
    std::array<K::Point_3, 16> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;

    c = c->next()->opposite()->next();

    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(7, 6));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int,int>(3, 2));
    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(1, 5));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int, int>(0, 4));
    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(8, 9));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int, int>(12, 13));
    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(14, 10));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int, int>(15, 11));

    return vertices;
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::quadFaceOddTwoCornerHelper(Polyhedron::Halfedge_iterator& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies) {
    halfedge = halfedge->next();
    vertices[indicies.first] = halfedge->vertex()->point();
    halfedge = halfedge->next();
    vertices[indicies.second] = halfedge->vertex()->point();
    return halfedge->next()->opposite();
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::quadFaceOddTwoSideHelper(Polyhedron::Halfedge_iterator& halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies) {
    halfedge = halfedge->next();
    vertices[indicies.first] = halfedge->vertex()->point();
    halfedge = halfedge->next();
    vertices[indicies.second] = halfedge->vertex()->point();
    return halfedge->opposite();
}

inline std::array<K::Point_3, 16> MeshWalkHandler::quadEdgeOddTwo(Polyhedron::Halfedge_iterator& halfedge) {
    std::array<K::Point_3, 16> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;

    vertices[1] = c->prev()->vertex()->point();
    vertices[2] = c->vertex()->point();

    c = quadEdgeOddTwoStraightHelper(c);

    vertices[3] = c->vertex()->point();

    c = c->opposite();
    c = quadEdgeOddTwoStraightHelper(c);
    c = quadEdgeOddTwoStraightHelper(c);

    vertices[0] = c->vertex()->point();

    return vertices;
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::quadEdgeOddTwoStraightHelper(Polyhedron::Halfedge_iterator& halfedge) {
    return halfedge->next()->opposite()->next();
}


std::vector<K::Point_3> MeshWalkHandler::triEvenVertices(Polyhedron::Halfedge_iterator& halfedge) {
    std::vector<K::Point_3> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;
    vertices.push_back(c->prev()->vertex()->point());

    do {
        vertices.push_back(c->vertex()->point());
        c = c->next()->next()->opposite();
    } while(c != halfedge);

    return vertices;
}

std::vector<K::Point_3> MeshWalkHandler::quadEvenVertices(Polyhedron::Halfedge_iterator& halfedge) {
    std::vector<K::Point_3> vertices;

    Polyhedron::Halfedge_iterator& c = halfedge;
    vertices.push_back(c->prev()->vertex()->point());

    do {
        vertices.push_back(c->vertex()->point());
        c = c->next();
        vertices.push_back(c->vertex()->point());
        c = c->next()->next()->opposite();
    } while(c != halfedge);

    return vertices;
}
