#include "mesh.h"

K::Point_3 Mesh::toKernelVector(QVector3D v)
{
    return K::Point_3(v[0], v[1], v[2]);
}

QVector3D Mesh::toQVector(K::Point_3 v)
{
    return QVector3D(v.x(), v.y(), v.z());
}

Mesh::Vertex Mesh::toVertex(K::Point_3 v) {
    return {toQVector(v), QVector3D(), QVector3D(1, 1, 1)};
}

Mesh::Vertex Mesh::toVertex(QVector3D v) {
    return {v, QVector3D(), QVector3D(1, 1, 1)};
}

template <class HDS>
class BuildPolyhedron : public CGAL::Modifier_base<HDS>
{
public:
    Mesh const& m_mesh;

    BuildPolyhedron(Mesh const& mesh):
        m_mesh(mesh)
    {}

    void operator()(HDS& hds)
    {
        CGAL::Polyhedron_incremental_builder_3<HDS> resultBuilder( hds, true);

        resultBuilder.begin_surface(m_mesh.m_vertices.size(), m_mesh.m_indices.size() / 3);

        for (int i = 0; i < m_mesh.m_vertices.size(); i++)
        {
            resultBuilder.add_vertex(Mesh::toKernelVector(m_mesh.m_vertices[i].m_position));
        }

        for (int i = 0; i < m_mesh.m_indices.size() / 3; i++)
        {
            resultBuilder.begin_facet();
            resultBuilder.add_vertex_to_facet(m_mesh.m_indices[i * 3]);
            resultBuilder.add_vertex_to_facet(m_mesh.m_indices[i * 3 + 1]);
            resultBuilder.add_vertex_to_facet(m_mesh.m_indices[i * 3 + 2]);
            resultBuilder.end_facet();
        }

        resultBuilder.end_surface();
    }
};

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

Mesh::Mesh(Polyhedron polyhedron)
{
    for (auto it = polyhedron.vertices_begin(); it != polyhedron.vertices_end(); ++it)
    {
        QVector3D vertex = toQVector(it->point());
        m_vertices.push_back({vertex, QVector3D(0.0f,0.0f,0.0f), QVector3D(1.0f,1.0f,1.0f)});
    }

    for (auto it = polyhedron.facets_begin(); it != polyhedron.facets_end(); ++it)
    {
        auto edge = it->facet_begin();

        do
        {
            int id = std::distance(polyhedron.vertices_begin(), edge->vertex());
            m_indices.push_back(id);
        }
        while (++edge != it->facet_begin());
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
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5,-0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5, 0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5, 0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5,-0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5,-0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5, 0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {-0.5, 0.5,-0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
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

Polyhedron Mesh::convertToSurfaceMesh()
{
    Polyhedron resultMesh;
    BuildPolyhedron<Polyhedron::HalfedgeDS> resultBuilder(*this);
    resultMesh.delegate(resultBuilder);

    return resultMesh;
}

