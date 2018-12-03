#include "MeshWalkHandler.h"

MeshWalkHandler::Walk MeshWalkHandler::walk(Polyhedron::Halfedge_iterator halfedge, int neighbour_level, SubdivisionType subdivision_type, OddsType odds_type, MeshType mesh_type) {
    std::array<K::Point_3, 16> odds;
    std::vector<K::Point_3> evens;

    int n_odds = 0;

    switch (mesh_type) {
    case Triangular:

        switch (neighbour_level) {
        case 1:
            n_odds = triOddVerticesOneNeighbour(odds, halfedge);
            break;
        case 2:
            n_odds = triOddVerticesTwoNeighbour(odds, halfedge);
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
            n_odds = quadOddVerticesOneNeighbour(odds, halfedge, odds_type);
            break;
        case 2:
            n_odds = quadOddVerticesTwoNeighbour(odds, halfedge, odds_type);
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

    return Walk(odds, evens, n_odds, evens.size(), mesh_type, odds_type, subdivision_type);
}

MeshWalkHandler::LoopLikeWalk MeshWalkHandler::loopLikeWalk(Polyhedron::Halfedge_iterator halfedge) {
    std::array<K::Point_3, 16> odds;
    triOddVerticesOneNeighbour(odds, halfedge);
    std::vector<K::Point_3> evens = triEvenVertices(halfedge);
    return LoopLikeWalk(odds, evens);
}

MeshWalkHandler::ButterflyLikeWalk MeshWalkHandler::butterflyLikeWalk(Polyhedron::Halfedge_iterator halfedge) {
    std::array<K::Point_3, 16> odds;
    triOddVerticesTwoNeighbour(odds, halfedge);
    return ButterflyLikeWalk(odds);
}

MeshWalkHandler::CatmullClarkLikeWalk MeshWalkHandler::catmullClarkLikeWalk(Polyhedron::Halfedge_iterator halfedge, OddsType odds_type) {
    std::array<K::Point_3, 16> odds;
    quadOddVerticesOneNeighbour(odds, halfedge, odds_type);
    std::vector<K::Point_3> evens = quadEvenVertices(halfedge);
    return CatmullClarkLikeWalk(odds, evens, odds_type);
}

MeshWalkHandler::KobbeltLikeWalk MeshWalkHandler::kobbeltLikeWalk(Polyhedron::Halfedge_iterator halfedge, OddsType odds_type) {
    std::array<K::Point_3, 16> odds;
    quadOddVerticesTwoNeighbour(odds, halfedge, odds_type);
    return KobbeltLikeWalk(odds, odds_type);
}

int MeshWalkHandler::triOddVerticesOneNeighbour(std::array<K::Point_3, 16>& vertices, Polyhedron::Halfedge_iterator halfedge) {

    Polyhedron::Halfedge_iterator c = halfedge;

    vertices[2] = c->vertex()->point();
    vertices[1] = c->prev()->vertex()->point();
    c = c->next();
    vertices[0] = c->vertex()->point();
    c = c->prev()->opposite()->next();
    vertices[3] = c->vertex()->point();

    return 4;
}

int MeshWalkHandler::triOddVerticesTwoNeighbour(std::array<K::Point_3, 16>& vertices, Polyhedron::Halfedge_iterator halfedge) {
    Polyhedron::Halfedge_iterator c = halfedge;

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

    return 8;
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::secondNeighbourTriHelper(std::array<K::Point_3, 16>& vertices, int index, Polyhedron::Halfedge_iterator halfedge) {
    halfedge = halfedge->opposite()->next();
    vertices[index] = halfedge->vertex()->point();
    return halfedge->prev()->opposite();
}

int MeshWalkHandler::quadOddVerticesOneNeighbour(std::array<K::Point_3, 16>& vertices, Polyhedron::Halfedge_iterator halfedge, OddsType odds_type) {
    Polyhedron::Halfedge_iterator c = halfedge;

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

        return 6;
    }

    return 4;
}

int MeshWalkHandler::quadOddVerticesTwoNeighbour(std::array<K::Point_3, 16>& vertices, Polyhedron::Halfedge_iterator halfedge, OddsType odds_type) {
    switch (odds_type) {
    case Face:
        return quadFaceOddTwo(vertices, halfedge);
        break;
    case Edge:
        return quadEdgeOddTwo(vertices, halfedge);
        break;
    default:
        throw std::exception("Unknown OddsType: " + odds_type);
    }
}

inline int MeshWalkHandler::quadFaceOddTwo(std::array<K::Point_3, 16>& vertices, Polyhedron::Halfedge_iterator halfedge) {
    Polyhedron::Halfedge_iterator edge = halfedge;

    /*
    c = c->next()->opposite()->next();

    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(7, 6));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int,int>(3, 2));
    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(1, 5));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int, int>(0, 4));
    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(8, 9));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int, int>(12, 13));
    c = quadFaceOddTwoSideHelper(c, vertices, std::pair<int, int>(14, 10));
    c = quadFaceOddTwoCornerHelper(c, vertices, std::pair<int, int>(15, 11));
    */

    Polyhedron::Point& p1 = edge->vertex()->point();
    Polyhedron::Point& p2 = edge->next()->vertex()->point();
    Polyhedron::Point& p3 = edge->next()->next()->vertex()->point();
    Polyhedron::Point& p4 = edge->next()->next()->next()->vertex()->point();

    Polyhedron::Point& p5 = edge->opposite()->next()->next()->next()->opposite()->next()->vertex()->point();
    Polyhedron::Point& p6 = edge->next()->opposite()->next()->vertex()->point();
    Polyhedron::Point& p7 = edge->next()->opposite()->next()->next()->vertex()->point();
    Polyhedron::Point& p8 = edge->next()->next()->opposite()->next()->opposite()->next()->next()->vertex()->point();

    Polyhedron::Point& p9 = edge->next()->next()->opposite()->next()->vertex()->point();
    Polyhedron::Point& p10 = edge->next()->next()->opposite()->next()->next()->vertex()->point();
    Polyhedron::Point& p11 = edge->next()->next()->next()->opposite()->next()->opposite()->next()->next()->vertex()->point();
    Polyhedron::Point& p12 = edge->next()->next()->next()->opposite()->next()->vertex()->point();

    Polyhedron::Point& p13 = edge->opposite()->next()->opposite()->next()->vertex()->point();
    Polyhedron::Point& p14 = edge->opposite()->next()->opposite()->next()->next()->vertex()->point();
    Polyhedron::Point& p15 = edge->opposite()->next()->vertex()->point();
    Polyhedron::Point& p16 = edge->opposite()->next()->next()->vertex()->point();

    // f: 1 2 3 4
    // s: 5, 8, 11, 14
    // t: 6 7 9 10 12 13 15 16

    vertices[0] = p5;
    vertices[1] = p6;
    vertices[2] = p7;
    vertices[3] = p8;

    vertices[4] = p9;
    vertices[5] = p1;
    vertices[6] = p2;
    vertices[7] = p10;

    vertices[8] = p15;
    vertices[9] = p3;
    vertices[10] = p4;
    vertices[11] = p16;

    vertices[12] = p11;
    vertices[13] = p12;
    vertices[14] = p13;
    vertices[15] = p14;

    return 16;
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::quadFaceOddTwoCornerHelper(Polyhedron::Halfedge_iterator halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies) {
    halfedge = halfedge->next();
    vertices[indicies.first] = halfedge->vertex()->point();
    halfedge = halfedge->next();
    vertices[indicies.second] = halfedge->vertex()->point();
    return halfedge->next()->opposite();
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::quadFaceOddTwoSideHelper(Polyhedron::Halfedge_iterator halfedge, std::array<K::Point_3, 16>& vertices, std::pair<int, int> indicies) {
    halfedge = halfedge->next();
    vertices[indicies.first] = halfedge->vertex()->point();
    halfedge = halfedge->next();
    vertices[indicies.second] = halfedge->vertex()->point();
    return halfedge->opposite();
}

inline int MeshWalkHandler::quadEdgeOddTwo(std::array<K::Point_3, 16>& vertices, Polyhedron::Halfedge_iterator halfedge) {
    Polyhedron::Halfedge_iterator c = halfedge;

    vertices[1] = c->opposite()->vertex()->point();
    vertices[2] = c->vertex()->point();

    Polyhedron::Halfedge_iterator cTmp = quadEdgeOddTwoStraightHelper(c);

    vertices[3] = cTmp->vertex()->point();

    c = c->opposite();
    c = quadEdgeOddTwoStraightHelper(c);

    vertices[0] = c->vertex()->point();

    return 4;
}

inline Polyhedron::Halfedge_iterator MeshWalkHandler::quadEdgeOddTwoStraightHelper(Polyhedron::Halfedge_iterator halfedge) {
    return halfedge->next()->opposite()->next();
}


std::vector<K::Point_3> MeshWalkHandler::triEvenVertices(Polyhedron::Halfedge_iterator halfedge) {
    std::vector<K::Point_3> vertices;

    Polyhedron::Halfedge_iterator c = halfedge;
    vertices.push_back(c->prev()->vertex()->point());

    do {
        vertices.push_back(c->vertex()->point());
        c = c->next()->next()->opposite();
    } while(c != halfedge);

    return vertices;
}

std::vector<K::Point_3> MeshWalkHandler::quadEvenVertices(Polyhedron::Halfedge_iterator halfedge) {
    std::vector<K::Point_3> vertices;

    Polyhedron::Halfedge_iterator c = halfedge;
    vertices.push_back(c->prev()->vertex()->point());

    do {
        vertices.push_back(c->vertex()->point());
        c = c->next();
        vertices.push_back(c->vertex()->point());
        c = c->next()->next()->opposite();
    } while(c != halfedge);

    return vertices;
}
