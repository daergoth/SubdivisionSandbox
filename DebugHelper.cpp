#include "DebugHelper.h"

std::ostream& operator<<(std::ostream& os, const QVector<int>& indicies) {
    // write obj to stream
    os << "[" << std::endl;

    for (int j = 0; j < indicies.size();j+=3) {
        int i1 = indicies[j];
        int i2 = indicies[j + 1];
        int i3 = indicies[j + 2];
        os << "(" << i1 << ", " << i2 << ", " << i3 << ")" << std::endl;
    }
    os << "]" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<Polyhedron::Halfedge_iterator, int>& edge_map) {
    // write obj to stream
    os << "[" << std::endl;

    for (auto p : edge_map) {
        os << "(" << &(p.first) << ", " << p.second<< ")" << std::endl;
    }
    os << "]" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<Polyhedron::Vertex_iterator, int>& even_map) {
    // write obj to stream
    os << "[" << std::endl;

    for (auto p : even_map) {
        os << "(" << &(p.first) << ", " << p.second<< ")" << std::endl;
    }
    os << "]" << std::endl;
    return os;
}
