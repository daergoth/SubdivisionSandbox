#ifndef MESH_H
#define MESH_H

#include "QVector3D"
#include "QVector"


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

    static Mesh makeTriangle();
    static Mesh makeCube();
    static Mesh makeTetrahedron();

    void generateIndices();

    QVector<Vertex> m_vertices;
    QVector<int> m_indices;
};

#endif // MESH_H
