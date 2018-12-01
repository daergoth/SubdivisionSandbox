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
//typedef CGAL::Surface_mesh<K::Point_3>    PolygonMesh;

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

    static K::Point_3 toKernelVector(QVector3D v);

    static QVector3D toQVector(K::Point_3 v);

    static Vertex toVertex(K::Point_3 v);
    static Vertex toVertex(QVector3D v);

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

    Mesh(Polyhedron surface_mesh);

    static Mesh makeTriangle();
    static Mesh makeCube();
    static Mesh makeTetrahedron();

    void generateIndices();

    Polyhedron convertToSurfaceMesh();

    QVector<Vertex> m_vertices;
    QVector<int> m_indices;
};

#endif // MESH_H
