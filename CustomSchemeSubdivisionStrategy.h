#ifndef CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
#define CUSTOMSCHEMESUBDIVISIONSTRATEGY_H

#include "SubdivisionStrategy.h"
#include "CustomScheme.h"
#include "mesh.h"
#include "MeshWalkHandler.h"
#include "WeightArrayGenerator.h"
#include "DebugHelper.h"

class CustomSchemeSubdivisionStrategy : public SubdivisionStrategy
{
public:
    CustomSchemeSubdivisionStrategy(CustomScheme custom_scheme):
        custom_scheme(custom_scheme) {}

    Mesh doSubdivision(Mesh evenMesh);

    void setCustomScheme(CustomScheme custom_scheme);

private:
    CustomScheme custom_scheme;

    Mesh::Vertex calculateNewEdgeVert(Polyhedron::Halfedge_iterator halfedge);
    Mesh::Vertex calculateNewFaceVert(Polyhedron::Facet_iterator facet);
    Mesh::Vertex calculateNewEvenVert(Polyhedron::Vertex_iterator vertex);

    void calculateFacesTri(
            QVector<Mesh::Vertex>& vertices,
            QVector<int>& face_indicies,
            std::map<Polyhedron::Halfedge_iterator, Mesh::Vertex>& new_edge_vertices,
            std::map<Polyhedron::Vertex_iterator, Mesh::Vertex>& new_even_vertices,
            Polyhedron::Facet_iterator it,
            std::map<Polyhedron::Halfedge_iterator, int>& edge_map,
            std::map<Polyhedron::Vertex_iterator, int>& even_map);

    void calculateFacesQuad(
            QVector<Mesh::Vertex>& vertices,
            QVector<int>& face_indicies,
            std::map<Polyhedron::Halfedge_iterator, Mesh::Vertex>& new_edge_vertices,
            std::map<Polyhedron::Facet_iterator, Mesh::Vertex>& new_face_vertices,
            std::map<Polyhedron::Vertex_iterator, Mesh::Vertex>& new_even_vertices,
            Polyhedron::Facet_iterator it,
            std::map<Polyhedron::Halfedge_iterator, int>& edge_map,
            std::map<Polyhedron::Vertex_iterator, int>& even_map);
};

#endif // CUSTOMSCHEMESUBDIVISIONSTRATEGY_H
