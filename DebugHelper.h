#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#include <map>
#include <QVector>
#include "mesh.h"

std::ostream& operator<<(std::ostream& os, const Mesh::Vertex& vertex);
std::ostream& operator<<(std::ostream& os, const QVector<int>& indices);
std::ostream& operator<<(std::ostream& os, const QVector<Mesh::Vertex>& vertices);
std::ostream& operator<<(std::ostream& os, const Polyhedron::Halfedge_iterator& he);
std::ostream& operator<<(std::ostream& os, const std::map<Polyhedron::Halfedge_iterator, int>& edge_map);
std::ostream& operator<<(std::ostream& os, const Polyhedron::Vertex_iterator&  v);
std::ostream& operator<<(std::ostream& os, const std::map<Polyhedron::Vertex_iterator, int>& even_map);

#endif // DEBUGHELPER_H
