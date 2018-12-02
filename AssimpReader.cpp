#include "AssimpReader.h"

AssimpReader::AssimpReader()
{

}

Mesh AssimpReader::readObj(std::string fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer
                .ReadFile(fileName.c_str(),
                          aiProcess_Triangulate|aiProcess_FixInfacingNormals);
    if(!scene){
        std::cout << "Mesh load failed!: " << fileName << std::endl;
        // TODO error dialog
        return Mesh();
    }

    aiMesh* aiMesh = scene->mMeshes[0];

    Mesh result;

    // Vertices from aiMesh.mVertices

    for(int i = 0; i < aiMesh->mNumVertices; i++){
        aiVector3D v = aiMesh->mVertices[i];

        result.m_vertices.push_back(
        {
            {v.x, v.y, v.z},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        });
    }

    // Indicies from aiMesh.mFaces
    for(int i = 0; i < aiMesh->mNumFaces; i++){
        aiFace f = aiMesh->mFaces[i];
        for(int  j = 0; j < f.mNumIndices; j++){
            unsigned int index = f.mIndices[j];
            result.m_indicesOriginal.push_back(index);
        }
    }

    result.m_numFaceVertices = 3;
    result.joinIdenticalVertices();
    result.generateIndices();

    return result;
}
