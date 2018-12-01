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
    QVector<Mesh::Vertex> const& m_vertices;
    QVector<int> const& m_indices;
    int m_numFaceVertices;

    BuildPolyhedron(
            QVector<Mesh::Vertex> const& p_vertices,
            QVector<int> const& p_indices,
            int p_numFaceVertices):
        m_vertices(p_vertices),
        m_indices(p_indices),
        m_numFaceVertices(p_numFaceVertices)
    {}

    void operator()(HDS& hds)
    {
        CGAL::Polyhedron_incremental_builder_3<HDS> resultBuilder( hds, true);

        resultBuilder.begin_surface(m_vertices.size(), m_indices.size() / m_numFaceVertices);

        for (int i = 0; i < m_vertices.size(); i++)
        {
            resultBuilder.add_vertex(Mesh::toKernelVector(m_vertices[i].m_position));
        }

        for (int i = 0; i < m_indices.size() / m_numFaceVertices; i++)
        {
            resultBuilder.begin_facet();
            for (int j = 0; j < m_numFaceVertices; ++j)
            {
                resultBuilder.add_vertex_to_facet(m_indices[i * m_numFaceVertices + j]);
            }
            resultBuilder.end_facet();
        }

        resultBuilder.end_surface();
    }
};

Mesh::Mesh(int p_numFaceVertices):
    m_numFaceVertices(p_numFaceVertices)
{


}

Mesh::Mesh(QVector<Vertex>const& p_vertices, int p_numFaceVertices):
    m_vertices(p_vertices),
    m_numFaceVertices(p_numFaceVertices)
{
    generateIndices();
}

Mesh::Mesh(QVector<Vertex>const& p_vertices, QVector<int>const& p_indices, int p_numFaceVertices):
    m_vertices(p_vertices),
    m_indicesOriginal(p_indices),
    m_numFaceVertices(p_numFaceVertices)
{

}

Mesh::Mesh(Polyhedron polyhedron)
{
    // Csúcsok
    for (auto it = polyhedron.vertices_begin(); it != polyhedron.vertices_end(); ++it)
    {
        QVector3D vertex = toQVector(it->point());
        m_vertices.push_back({vertex, QVector3D(0.0f,0.0f,0.0f), QVector3D(1.0f,1.0f,1.0f)});
    }

    // Eredeti indexek
    for (auto it = polyhedron.facets_begin(); it != polyhedron.facets_end(); ++it)
    {
        auto edge = it->facet_begin();

        int numVertices = 0;
        do
        {
            int id = std::distance(polyhedron.vertices_begin(), edge->vertex());
            m_indicesOriginal.push_back(id);
            ++numVertices;
        }
        while (++edge != it->facet_begin());

        m_numFaceVertices = numVertices;
    }

    // Háromszög és körvonal indexek
    generateIndices();
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

    result.m_numFaceVertices = 3;

    result.generateIndices();

    return result;
}

Mesh Mesh::makeCube(bool quads)
{
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

    if (quads)
    {
        result.m_indicesOriginal =
        {
            0,1,2,3,
            1,5,6,2,
            5,4,7,6,
            4,0,3,7,
            4,5,1,0,
            3,2,6,7
        };

        result.m_numFaceVertices = 4;
    }
    else
    {
        result.m_indicesOriginal =
        {
            0,1,2,
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
            3,6,7
        };

        result.m_numFaceVertices = 3;
    }

    result.generateIndices();

    return result;
}

Mesh Mesh::makeTetrahedron()
{
    Mesh result;

    result.m_vertices.push_back(
    {
        {-0.5,-0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.5, -0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        { 0.0, 0.5, 0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });
    result.m_vertices.push_back(
    {
        {0.0, 0.0,-0.5},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    });

    result.m_indices = {0,2,1,
                        0,3,2,
                        1,2,3,
                        0,1,3};

    return result;
}
void Mesh::generateIndices(bool regenerate)
{
    // Eredeti indexek generálása
    if (m_indicesOriginal.empty() || regenerate)
    {
        m_indicesOriginal.resize(m_vertices.size());

        for (int i = 0; i < m_vertices.size();i++)
        {
            m_indicesOriginal[i] = i;
        }
    }

    // Háromszögelt indexek generálása
    if (m_numFaceVertices == 3)
    {
        m_indicesTriangulated = m_indicesOriginal;
    }
    else
    {
        int numFaces = m_indicesOriginal.size() / m_numFaceVertices;
        int numTrianglesPerFace = m_numFaceVertices - 2;
        m_indicesTriangulated.resize(numFaces * numTrianglesPerFace * 3);

        for (int i = 0; i < numFaces; i++)
        {
            for (int j = 0; j < numTrianglesPerFace; ++j)
            {
                m_indicesTriangulated[(i * numTrianglesPerFace + j) * 3 + 0] = m_indicesOriginal[i * m_numFaceVertices];
                m_indicesTriangulated[(i * numTrianglesPerFace + j) * 3 + 1] = m_indicesOriginal[i * m_numFaceVertices + j + 1];
                m_indicesTriangulated[(i * numTrianglesPerFace + j) * 3 + 2] = m_indicesOriginal[i * m_numFaceVertices + j + 2];
            }
        }
    }

    // Körvonal indexek generálása
    m_indicesSilhouette.resize(m_indicesOriginal.size() * 2);

    for (int i = 0; i < m_indicesOriginal.size() / m_numFaceVertices; ++i)
    {
        for (int j = 0; j < m_numFaceVertices; ++j)
        {
            m_indicesSilhouette[(i * m_numFaceVertices + j) * 2 + 0] = m_indicesOriginal[i * m_numFaceVertices + (j + 0) % m_numFaceVertices];
            m_indicesSilhouette[(i * m_numFaceVertices + j) * 2 + 1] = m_indicesOriginal[i * m_numFaceVertices + (j + 1) % m_numFaceVertices];
        }
    }
}

Polyhedron Mesh::convertToSurfaceMesh(bool triangulate)
{
    Polyhedron resultMesh;
    //BuildPolyhedron<Polyhedron::HalfedgeDS> resultBuilder(*this);
    if (triangulate)
    {
        BuildPolyhedron<Polyhedron::HalfedgeDS> resultBuilder(m_vertices, m_indicesTriangulated, 3);
        resultMesh.delegate(resultBuilder);
    }
    else
    {
        BuildPolyhedron<Polyhedron::HalfedgeDS> resultBuilder(m_vertices, m_indicesOriginal, m_numFaceVertices);
        resultMesh.delegate(resultBuilder);
    }

    return resultMesh;
}

