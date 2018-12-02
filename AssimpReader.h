#ifndef ASSIMPREADER_H
#define ASSIMPREADER_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "Mesh.h"

class AssimpReader
{
public:
    AssimpReader();
    static Mesh readObj(std::string fileName);
};

#endif // ASSIMPREADER_H
