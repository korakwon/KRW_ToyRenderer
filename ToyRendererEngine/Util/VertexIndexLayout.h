#pragma once
#include <vector>
#include "Math.h"
#include "Application/Application.h"

// ****************************** Enum ******************************
enum class EVertexInfo
{
    Position = 1 << 0,
    Normal = 1 << 1,
    Tangent = 1 << 2,
    UV = 1 << 3,
    Color = 1 << 4,
};
inline EVertexInfo operator|(EVertexInfo lhs, EVertexInfo rhs) 
{
    return static_cast<EVertexInfo>(static_cast<char>(lhs) | static_cast<char>(rhs));
}

// ********************** Forward Declarations **********************
struct VkPipelineVertexInputStateCreateInfo;

// ***************************** Index ******************************
typedef uint32_t INDEX32;

// ***************************** Vertex *****************************
struct Vertex
{
    Vector3 Position;

    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(Vertex));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexNormal
{
    Vector3 Position;
    Vector3 Normal;

    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexNormal));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::Normal;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexColor
{
    Vector3 Position;
    Vector3 Color;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexColor));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::Color;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexNormalColor
{
    Vector3 Position;
    Vector3 Normal;
    Vector3 Color;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexNormalColor));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::Normal | EVertexInfo::Color;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexTexture
{
    Vector3 Position;
    Vector2 UV;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexTexture));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::UV;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexTextureColor
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Color;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexTextureColor));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::UV | EVertexInfo::Color;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexTextureNormal
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexTextureNormal));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::UV | EVertexInfo::Normal;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexTextureNormalTangent
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
    Vector3 Tangent;
    Vector3 BiTangent;

    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexTextureNormalTangent));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::UV | EVertexInfo::Normal | EVertexInfo::Tangent;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexCubeMap
{
    Vector3 Position;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexTexture));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};

struct VertexImGui
{
    Vector2 Position;
    Vector2 UV;
    uint32_t Color;
    
    static UINT32 GetSize()
    {
        static UINT32 Size = static_cast<UINT32>(sizeof(VertexImGui));
        return Size; 
    }
    static EVertexInfo GetVertexInfo()
    {
        static EVertexInfo Info = EVertexInfo::Position | EVertexInfo::UV | EVertexInfo::Color;
        return Info;
    }
    
    static VkPipelineVertexInputStateCreateInfo GetVkPipelineVertexInputStateCreateInfo();
};