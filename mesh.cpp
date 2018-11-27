#include "mesh.h"

Mesh::Mesh()
{


}

Mesh::Mesh(QVector<Vertex>const& p_vertices):
    m_vertices(p_vertices)
{
    generateIndices();
}

Mesh::Mesh(QVector<Vertex>const& p_vertices, QVector<int>const& p_indices):
    m_vertices(p_vertices),
    m_indices(p_indices)
{

}

Mesh Mesh::makeTriangle()
{
    Mesh result;

    result.m_vertices.push_back(
    {
        {0.0f, 0.707f, -2.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5f, -0.5f, -2.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });
    result.m_vertices.push_back(
    {
        {0.5f, -0.5f, -2.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });

    result.generateIndices();

    return result;
}

Mesh Mesh::makeCube()
{
    // TODO: implement

    Mesh result;

    result.m_vertices.push_back(
    {
        {-0.5,-0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5,-0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5, 0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5, 0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5,-0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5,-0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5, 0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5, 0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    });

    result.m_indices = {0,1,2,
                        0,2,3,
                        1,5,6,
                        1,6,2,
                        4,5,6,
                        4,6,7,
                        0,3,7,
                        0,7,4,
                        0,1,5,
                        0,5,4,
                        3,2,6,
                        3,6,7};

    return result;
}

Mesh Mesh::makeTetrahedron()
{
    // TODO: implement
    return Mesh();
}

void Mesh::generateIndices()
{
    m_indices.resize(m_vertices.size());

    for (int i = 0; i<m_vertices.size();i++)
    {
        m_indices[i] = i;
    }
}

