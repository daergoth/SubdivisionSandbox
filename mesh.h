#ifndef MESH_H
#define MESH_H

#include "QVector3D"
#include "QVector"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <QTextStream>
#include <QDebug>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> SurfaceMesh;
typedef SurfaceMesh::Vertex_index vertex_descriptor;
typedef SurfaceMesh::Face_index face_descriptor;

enum MeshType {Triangular, Quadrilateral};

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
    Mesh();

    /**
     * @brief Implicit indexeket beállító konstruktor.
     */
    Mesh(QVector<Vertex>const& p_vertices);

    /**
     * @brief Csúcsokat és indexeket beállító konstruktor.
     */
    Mesh(QVector<Vertex>const& p_vertices, QVector<int>const& p_indices);

    Mesh(SurfaceMesh surface_mesh);

    static Mesh makeTriangle();
    static Mesh makeCube();
    static Mesh makeTetrahedron();

    void generateIndices();

    SurfaceMesh convertToSurfaceMesh();

    QVector<Vertex> m_vertices;
    QVector<int> m_indices;
};

#endif // MESH_H
