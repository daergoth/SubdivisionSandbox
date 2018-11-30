#ifndef MESH_H
#define MESH_H

#include "QVector3D"
#include "QVector"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <QTextStream>
#include <QDebug>

typedef CGAL::Simple_cartesian<double>                                    K;
typedef CGAL::Polyhedron_3<K>                                             Polyhedron;

/**
 * @brief Mesh osztály, amely csúcsok és indexek formájában ábrázol egy háromszögelt mesht.
 */
class Mesh
{
public:
    /**
     * @brief Egy vertexhez tartozó adatokat összefogó struktúra.
     */
    struct Vertex
    {
        QVector3D m_position;
        QVector3D m_normal;
        QVector3D m_color;
    };

    /**
     * @brief Default üres konstruktor.
     */
    Mesh(int p_numFaceVertices = 3);

    /**
     * @brief Implicit indexeket beállító konstruktor.
     */
    Mesh(QVector<Vertex>const& p_vertices, int p_numFaceVertices = 3);

    /**
     * @brief Csúcsokat és indexeket beállító konstruktor.
     */
    Mesh(QVector<Vertex>const& p_vertices, QVector<int>const& p_indices, int p_numFaceVertices = 3);

    /**
     * @brief CGAL-ról Mesh-re konvertáló konstruktor.
     */
    Mesh(Polyhedron surface_mesh);

    static Mesh makeTriangle();
    static Mesh makeCube(bool quads);
    static Mesh makeTetrahedron();

    /**
     * @brief Automatically generates indices for the vertices currently present in the mesh.
     */
    void generateIndices(bool regenerate = false);

    /**
     * @brief Converts the mesh to a CGAL-compatible representation.
     */
    Polyhedron convertToSurfaceMesh(bool triangulate = false);

    QVector<Vertex> m_vertices;
    QVector<int> m_indicesOriginal;
    QVector<int> m_indicesTriangulated;
    QVector<int> m_indicesSilhouette;
    int m_numFaceVertices;
};

#endif // MESH_H
