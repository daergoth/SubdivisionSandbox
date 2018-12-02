#include "CustomSchemeSubdivisionStrategy.h"

Mesh CustomSchemeSubdivisionStrategy::doSubdivision(Mesh evenMesh) {
    Polyhedron old_mesh = evenMesh.convertToSurfaceMesh();

    std::map<Polyhedron::Halfedge_iterator, Mesh::Vertex> new_edge_vertices;
    std::map<Polyhedron::Facet_iterator, Mesh::Vertex> new_face_vertices;

    // kiszámol új edge vertices
    for (Polyhedron::Halfedge_iterator it = old_mesh.halfedges_begin(); it != old_mesh.halfedges_end(); ++it) {
        if (new_edge_vertices.find(it) == new_edge_vertices.end()) {
            Mesh::Vertex new_edge_vert = calculateNewEdgeVert(it);
            new_edge_vertices[it] = new_edge_vert;
            new_edge_vertices[it->opposite()] = new_edge_vert;
        }
    }

    // kiszámol új face vertices, csak ha QUAD
    if (custom_scheme.mesh_type == CustomSchemeMeshType::Quad) {
        for (Polyhedron::Facet_iterator it = old_mesh.facets_begin(); it != old_mesh.facets_end(); ++it) {
            Mesh::Vertex new_face_vert = calculateNewFaceVert(it);
            new_face_vertices[it] = new_face_vert;
        }
    }

    // frissít even vertices, csak ha APPROX
    std::map<Polyhedron::Vertex_iterator, Mesh::Vertex> new_even_vertices;
    if (custom_scheme.refinement_type == CustomSchemeRefinementType::Approx) {
        for (Polyhedron::Vertex_iterator it = old_mesh.vertices_begin(); it != old_mesh.vertices_end(); ++it) {
            Mesh::Vertex new_even_vert = calculateNewEvenVert(it);
            new_even_vertices[it] = new_even_vert;
        }
    }

    // felépít faces
    QVector<Mesh::Vertex> vertices;
    QVector<int> face_indicies;
    std::map<Polyhedron::Halfedge_iterator, int> edge_map;
    std::map<Polyhedron::Vertex_iterator, int> even_map;
    for (Polyhedron::Facet_iterator it = old_mesh.facets_begin(); it != old_mesh.facets_end(); ++it) {
        if (custom_scheme.mesh_type == CustomSchemeMeshType::Tri) {
            calculateFacesTri(vertices, face_indicies, new_edge_vertices, new_even_vertices, it, edge_map, even_map);
        } else {
            calculateFacesQuad(vertices, face_indicies, new_edge_vertices, new_face_vertices, new_even_vertices, it, edge_map, even_map);
        }
    }

    Mesh result(vertices, face_indicies, custom_scheme.mesh_type == CustomSchemeMeshType::Tri ? 3 : 4);
    result.generateIndices(false);

    return result;
}

Mesh::Vertex CustomSchemeSubdivisionStrategy::calculateNewEdgeVert(Polyhedron::Halfedge_iterator halfedge) {
    MeshWalkHandler& mwh = MeshWalkHandler::getInstance();
    MeshWalkHandler::Walk walk = mwh.walk(
                halfedge,
                custom_scheme.neighbour_level,
                static_cast<SubdivisionType>(custom_scheme.refinement_type),
                MeshWalkHandler::OddsType::Edge,
                static_cast<MeshType>(custom_scheme.mesh_type));

    // generate weight array
    WeightArrayGenerator& wag = WeightArrayGenerator::getInstance();
    std::array<float, 16> weights = wag.generateWeights(custom_scheme, MeshWalkHandler::OddsType::Edge);

    /*
    std::cout << "Odds_num: " << walk.n_odds << std::endl;
    std::cout << "Weight array" << std::endl;
    for(float f : weights) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
    */

    // do multiply
    QVector3D result;
    for (int i = 0; i < walk.n_odds; ++i) {
        result += Mesh::toQVector(walk.atOdds(i)) * weights[i];
    }

    return Mesh::toVertex(result);
}

Mesh::Vertex CustomSchemeSubdivisionStrategy::calculateNewFaceVert(Polyhedron::Facet_iterator facet) {
    MeshWalkHandler& mwh = MeshWalkHandler::getInstance();
    MeshWalkHandler::Walk walk = mwh.walk(
                facet->halfedge(),
                custom_scheme.neighbour_level,
                static_cast<SubdivisionType>(custom_scheme.refinement_type),
                MeshWalkHandler::OddsType::Edge,
                static_cast<MeshType>(custom_scheme.mesh_type));

    // generate weight array
    WeightArrayGenerator& wag = WeightArrayGenerator::getInstance();
    std::array<float, 16> weights = wag.generateWeights(custom_scheme, MeshWalkHandler::OddsType::Face);

    // do multiply
    QVector3D result;
    for (int i = 0; i < walk.n_odds; ++i) {
        result += Mesh::toQVector(walk.atOdds(i)) * weights[i];
    }

    return Mesh::toVertex(result);
}

Mesh::Vertex CustomSchemeSubdivisionStrategy::calculateNewEvenVert(Polyhedron::Vertex_iterator vertex) {
    MeshWalkHandler& mwh = MeshWalkHandler::getInstance();
    MeshWalkHandler::Walk walk = mwh.walk(
                vertex->halfedge()->next(),
                custom_scheme.neighbour_level,
                SubdivisionType::Approximating,
                MeshWalkHandler::OddsType::Edge,
                static_cast<MeshType>(custom_scheme.mesh_type));

    // generate weight array
    WeightArrayGenerator& wag = WeightArrayGenerator::getInstance();
    std::vector<float> weights = wag.generateEvenWeights(custom_scheme, walk.n_evens - 1);

    /*
    std::cout << "Evens_num: " << walk.n_evens << std::endl;
    std::cout << "Weight array" << std::endl;
    for(float f : weights) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
    */

    // do multiply
    QVector3D result;
    for (int i = 0; i < walk.n_evens; ++i) {
        result += Mesh::toQVector(walk.atEvens(i)) * weights[i];
    }

    return Mesh::toVertex(result);
}

void CustomSchemeSubdivisionStrategy::calculateFacesTri(QVector<Mesh::Vertex>& vertices,
                                                        QVector<int>& face_indicies,
                                                        std::map<Polyhedron::Halfedge_iterator, Mesh::Vertex>& new_edge_vertices,
                                                        std::map<Polyhedron::Vertex_iterator, Mesh::Vertex>& new_even_vertices,
                                                        Polyhedron::Facet_iterator it,
                                                        std::map<Polyhedron::Halfedge_iterator, int>& edge_map,
                                                        std::map<Polyhedron::Vertex_iterator, int>& even_map) {

    std::vector<int> new_face_indices;

    Polyhedron::Halfedge_iterator c = it->halfedge();
    do {
        auto edge_map_it = edge_map.find(c);
        if ( edge_map_it == edge_map.end()) {
            int new_vert_index = vertices.size();
            vertices.push_back(new_edge_vertices[c]);

            edge_map[c] = new_vert_index;
            edge_map[c->opposite()] = new_vert_index;

            new_face_indices.push_back(new_vert_index);
        } else {
            new_face_indices.push_back(edge_map_it->second);
        }

        auto even_map_it = even_map.find(c->vertex());
        if (even_map_it == even_map.end()) {
            int new_even_index = vertices.size();

            Mesh::Vertex v =
                    custom_scheme.refinement_type == CustomSchemeRefinementType::Approx
                    ? new_even_vertices[c->vertex()]
                    : Mesh::toVertex(c->vertex()->point());

            vertices.push_back(v);

            even_map[c->vertex()] = new_even_index;

            new_face_indices.push_back(new_even_index);
        } else {
            new_face_indices.push_back(even_map_it->second);
        }

        c = c->next();

    } while(c != it->halfedge());

    //std::cout << "edge_map:" << std::endl << edge_map << std::endl;
    //std::cout << "even_map:" << std::endl << even_map << std::endl;

    face_indicies.push_back(new_face_indices[0]);
    face_indicies.push_back(new_face_indices[1]);
    face_indicies.push_back(new_face_indices[2]);

    face_indicies.push_back(new_face_indices[2]);
    face_indicies.push_back(new_face_indices[3]);
    face_indicies.push_back(new_face_indices[4]);

    face_indicies.push_back(new_face_indices[4]);
    face_indicies.push_back(new_face_indices[5]);
    face_indicies.push_back(new_face_indices[0]);

    face_indicies.push_back(new_face_indices[0]);
    face_indicies.push_back(new_face_indices[2]);
    face_indicies.push_back(new_face_indices[4]);

}

void CustomSchemeSubdivisionStrategy::calculateFacesQuad(QVector<Mesh::Vertex>& vertices,
                                                         QVector<int>& face_indicies,
                                                         std::map<Polyhedron::Halfedge_iterator, Mesh::Vertex>& new_edge_vertices,
                                                         std::map<Polyhedron::Facet_iterator, Mesh::Vertex>& new_face_vertices,
                                                         std::map<Polyhedron::Vertex_iterator, Mesh::Vertex>& new_even_vertices,
                                                         Polyhedron::Facet_iterator it,
                                                         std::map<Polyhedron::Halfedge_iterator, int>& edge_map,
                                                         std::map<Polyhedron::Vertex_iterator, int>& even_map) {
    std::vector<int> new_face_indices;

    Polyhedron::Halfedge_iterator c = it->halfedge();
    do {
        auto edge_map_it = edge_map.find(c);
        if ( edge_map_it == edge_map.end()) {
            int new_vert_index = vertices.size();
            vertices.push_back(new_edge_vertices[c]);

            edge_map[c] = new_vert_index;
            edge_map[c->opposite()] = new_vert_index;

            new_face_indices.push_back(new_vert_index);
        } else {
            new_face_indices.push_back(edge_map_it->second);
        }

        auto even_map_it = even_map.find(c->vertex());
        if (even_map_it == even_map.end()) {
            int new_even_index = vertices.size();

            Mesh::Vertex v =
                    custom_scheme.refinement_type == CustomSchemeRefinementType::Approx
                    ? new_even_vertices[c->vertex()]
                    : Mesh::toVertex(c->vertex()->point());

            vertices.push_back(v);

            even_map[c->vertex()] = new_even_index;

            new_face_indices.push_back(new_even_index);
        } else {
            new_face_indices.push_back(even_map_it->second);
        }

        c = c->next();

    } while(c != it->halfedge());

    int new_face_vert_index = vertices.size();
    vertices.push_back(new_face_vertices[it]);

    face_indicies.push_back(new_face_indices[0]);
    face_indicies.push_back(new_face_indices[1]);
    face_indicies.push_back(new_face_indices[2]);
    face_indicies.push_back(new_face_vert_index);

    face_indicies.push_back(new_face_vert_index);
    face_indicies.push_back(new_face_indices[2]);
    face_indicies.push_back(new_face_indices[3]);
    face_indicies.push_back(new_face_indices[4]);

    face_indicies.push_back(new_face_indices[6]);
    face_indicies.push_back(new_face_vert_index);
    face_indicies.push_back(new_face_indices[4]);
    face_indicies.push_back(new_face_indices[5]);

    face_indicies.push_back(new_face_indices[7]);
    face_indicies.push_back(new_face_indices[0]);
    face_indicies.push_back(new_face_vert_index);
    face_indicies.push_back(new_face_indices[6]);
}

void CustomSchemeSubdivisionStrategy::setCustomScheme(CustomScheme custom_scheme) {
    this->custom_scheme = custom_scheme;
}
