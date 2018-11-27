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

Mesh::Mesh(SurfaceMesh surface_mesh)
{
    for (auto it = surface_mesh.vertices().begin(); it != surface_mesh.vertices().end(); ++it)
    {
        K::Point_3 vertex = surface_mesh.point(*it);
        QVector3D p(vertex.x(), vertex.y(), vertex.z());

        m_vertices.push_back({p,QVector3D(0.0f,0.0f,0.0f), QVector3D(1.0f,1.0f,1.0f)});
    }

    for (auto it = surface_mesh.faces().begin(); it != surface_mesh.faces().end(); ++it)
    {
        auto start_edge = surface_mesh.halfedge(*it);
        auto edge = start_edge;

        QVector<int> face_indices;

        do
        {
            auto source = surface_mesh.source(edge);
            face_indices.push_back(source);

            edge = surface_mesh.next(edge);
        }
        while (edge != start_edge);

        for (int i = 0; i < face_indices.size() - 2; ++i)
        {
            m_indices.push_back(face_indices[0]);
            m_indices.push_back(face_indices[i + 1]);
            m_indices.push_back(face_indices[i + 2]);
        }
    }
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
                        5,4,7,
                        5,7,6,
                        4,0,3,
                        4,3,7,
                        4,5,1,
                        4,1,0,
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

SurfaceMesh Mesh::convertToSurfaceMesh()
{
    SurfaceMesh resultMesh;
    QVector<vertex_descriptor> vertexDescriptors;

    for (int i = 0; i < m_vertices.size(); i++)
    {
        auto vd = resultMesh.add_vertex(K::Point_3(m_vertices[i].m_position[0], m_vertices[i].m_position[1], m_vertices[i].m_position[2]));
        vertexDescriptors.push_back(vd);
    }

    for (int i = 0; i < m_indices.size() / 3; i++)
    {
        resultMesh.add_face(
                vertexDescriptors[m_indices[i*3]],
                vertexDescriptors[m_indices[i*3+1]],
                vertexDescriptors[m_indices[i*3+2]]);
    }

    return resultMesh;
}

