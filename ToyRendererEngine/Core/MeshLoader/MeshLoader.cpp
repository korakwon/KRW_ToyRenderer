#include "MeshLoader.h"
#include "Util/VertexIndexLayout.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

bool IsHaveVertexFlag(const EVertexInfo& SrcType, const EVertexInfo& CheckType)
{
	return (static_cast<UINT32>(SrcType) & static_cast<UINT32>(CheckType)) == static_cast<UINT32>(CheckType);
}
UINT32 GetVertexFlagInfo(const EVertexInfo& Type)
{
	//UINT32 Flag = aiProcess_ConvertToLeftHanded;
	UINT32 Flag = aiProcess_Triangulate | aiProcess_FlipUVs;

	if(IsHaveVertexFlag(Type, EVertexInfo::Position))
	{
		Flag |= aiProcess_Triangulate;
	}
	if(IsHaveVertexFlag(Type, EVertexInfo::Normal))
	{
		Flag |= aiProcess_GenSmoothNormals;
	}
	if(IsHaveVertexFlag(Type, EVertexInfo::Tangent))
	{
		Flag |= aiProcess_CalcTangentSpace;
	}
	if(IsHaveVertexFlag(Type, EVertexInfo::UV))
	{
		Flag |= aiProcess_GenUVCoords;
	}
	if(IsHaveVertexFlag(Type, EVertexInfo::Color))
	{
	}
	
	return Flag;
}

void Core::MeshLoader_ReadMesh(const std::string& FileName, const EVertexInfo& Type, std::vector<Core::MeshLoadData>& Output)
{
	Output.clear();
	
	Assimp::Importer* AssimpImporter = new Assimp::Importer();
	const aiScene* Scene = AssimpImporter->ReadFile(FileName, GetVertexFlagInfo(Type));
	ASSERT_NOT_NULL(Scene);

	Output.resize(Scene->mNumMeshes);
	for(UINT32 i = 0; i < Scene->mNumMeshes; i++)
	{
		const aiMesh* AssimpMesh = Scene->mMeshes[i];
		UINT32 VertexNum = AssimpMesh->mNumVertices;

		// Init Name
		if(AssimpMesh->mName.length > 0)
		{
			Output[i].MeshName = AssimpMesh->mName.C_Str();
		}

		// Init Vertices
		{
			Output[i].Vertices.resize(VertexNum);
			{
				memcpy_s(Output[i].Vertices.data(), VertexNum * sizeof(Vector3), &AssimpMesh->mVertices[0], VertexNum * sizeof(Vector3));
			}
		}

		// Init Normals
		{
			Output[i].Normals.resize(VertexNum);
			if(AssimpMesh->HasNormals() == true)
			{
				memcpy_s(Output[i].Normals.data(), VertexNum * sizeof(Vector3), &AssimpMesh->mNormals[0], VertexNum * sizeof(Vector3));
			}
		}
		
		// Init Tangent
		{
			Output[i].Tangents.resize(VertexNum);
			Output[i].BiTangents.resize(VertexNum);
			if(AssimpMesh->HasTangentsAndBitangents() == true)
			{
				memcpy_s(Output[i].Tangents.data(), VertexNum * sizeof(Vector3), &AssimpMesh->mTangents[0], VertexNum * sizeof(Vector3));
				memcpy_s(Output[i].BiTangents.data(), VertexNum * sizeof(Vector3), &AssimpMesh->mBitangents[0], VertexNum * sizeof(Vector3));
			}
		}
		
		// Init UV
		{
			Output[i].TexCoord.resize(VertexNum);
			if(AssimpMesh->HasTextureCoords(0) == true)
			{
				for(UINT32 j = 0; j < VertexNum; j++)
				{
					Output[i].TexCoord[j].x = AssimpMesh->mTextureCoords[0][j].x;
					Output[i].TexCoord[j].y = AssimpMesh->mTextureCoords[0][j].y;
				}
			}
		}

		// Init Indices
		if(AssimpMesh->HasFaces() == true)
		{
			UINT32 FaceNum = AssimpMesh->mNumFaces;
			Output[i].Indices.resize(static_cast<UINT32>(FaceNum * 3));
			for(UINT32 j = 0; j < FaceNum; j++)
			{
				aiFace& Face = AssimpMesh->mFaces[j];
				memcpy_s(&Output[i].Indices[j * 3], 3 * sizeof(UINT32), &Face.mIndices[0], 3 * sizeof(UINT32));
			}
		}

		// Init Material
		aiMaterial* AssimpMaterial = Scene->mMaterials[i];
		{
			if(AssimpMaterial->GetName().length > 0)
			{
				Output[i].MaterialName = AssimpMaterial->GetName().C_Str();
			}
		}
	}
}
