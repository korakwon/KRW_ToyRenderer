#pragma once
#include "pch.h"

// ****************************** Enum ******************************
enum class EVertexInfo;

// ****************************** Croe *******************************
namespace Core
{
    struct MaterialLoadData
    {
        // material name
        // filename
    };
    
    struct MeshLoadData
    {
	    std::string MeshName;

        // Vertex Info
        std::vector<Vector3> Vertices;
        std::vector<Vector3> Normals;
        std::vector<Vector3> Tangents;
        std::vector<Vector3> BiTangents;
        std::vector<Vector2> TexCoord;
        
        // Indices
        std::vector<UINT32> Indices;
        
        // Material
	    std::string MaterialName;

    };
    
    void MeshLoader_ReadMesh(const std::string& FileName, const EVertexInfo& Type, std::vector<Core::MeshLoadData>& Output);
}