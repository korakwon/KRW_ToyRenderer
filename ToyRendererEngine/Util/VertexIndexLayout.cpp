#include "VertexIndexLayout.h"
#include "vulkan/vulkan_core.h"

VkPipelineVertexInputStateCreateInfo Vertex::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(Vertex);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(Vertex, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexNormal::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexNormal);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexNormal, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Normal = {};
        {
            VertexInputAttributeDescription_Normal.location = 1;
            VertexInputAttributeDescription_Normal.binding = 0;
            VertexInputAttributeDescription_Normal.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Normal.offset = offsetof(VertexNormal, Normal);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Normal);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexColor::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexColor);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexColor, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Color = {};
        {
            VertexInputAttributeDescription_Color.location = 1;
            VertexInputAttributeDescription_Color.binding = 0;
            VertexInputAttributeDescription_Color.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Color.offset = offsetof(VertexColor, Color);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Color);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexNormalColor::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexNormalColor);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexNormalColor, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Normal = {};
        {
            VertexInputAttributeDescription_Normal.location = 1;
            VertexInputAttributeDescription_Normal.binding = 0;
            VertexInputAttributeDescription_Normal.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Normal.offset = offsetof(VertexNormalColor, Normal);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Normal);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Color = {};
        {
            VertexInputAttributeDescription_Color.location = 2;
            VertexInputAttributeDescription_Color.binding = 0;
            VertexInputAttributeDescription_Color.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Color.offset = offsetof(VertexNormalColor, Color);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Color);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexTexture::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexTexture);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexTexture, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_UV = {};
        {
            VertexInputAttributeDescription_UV.location = 1;
            VertexInputAttributeDescription_UV.binding = 0;
            VertexInputAttributeDescription_UV.format = VK_FORMAT_R32G32_SFLOAT;
            VertexInputAttributeDescription_UV.offset = offsetof(VertexTexture, UV);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_UV);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexTextureColor::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexTextureColor);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexTextureColor, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_UV = {};
        {
            VertexInputAttributeDescription_UV.location = 1;
            VertexInputAttributeDescription_UV.binding = 0;
            VertexInputAttributeDescription_UV.format = VK_FORMAT_R32G32_SFLOAT;
            VertexInputAttributeDescription_UV.offset = offsetof(VertexTextureColor, UV);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_UV);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Color = {};
        {
            VertexInputAttributeDescription_Color.location = 2;
            VertexInputAttributeDescription_Color.binding = 0;
            VertexInputAttributeDescription_Color.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Color.offset = offsetof(VertexTextureColor, Color);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Color);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexTextureNormal::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexTextureNormal);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexTextureNormal, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_UV = {};
        {
            VertexInputAttributeDescription_UV.location = 1;
            VertexInputAttributeDescription_UV.binding = 0;
            VertexInputAttributeDescription_UV.format = VK_FORMAT_R32G32_SFLOAT;
            VertexInputAttributeDescription_UV.offset = offsetof(VertexTextureNormal, UV);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_UV);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Normal = {};
        {
            VertexInputAttributeDescription_Normal.location = 2;
            VertexInputAttributeDescription_Normal.binding = 0;
            VertexInputAttributeDescription_Normal.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Normal.offset = offsetof(VertexTextureNormal, Normal);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Normal);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexTextureNormalTangent::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexTextureNormalTangent);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexTextureNormalTangent, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_UV = {};
        {
            VertexInputAttributeDescription_UV.location = 1;
            VertexInputAttributeDescription_UV.binding = 0;
            VertexInputAttributeDescription_UV.format = VK_FORMAT_R32G32_SFLOAT;
            VertexInputAttributeDescription_UV.offset = offsetof(VertexTextureNormalTangent, UV);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_UV);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Normal = {};
        {
            VertexInputAttributeDescription_Normal.location = 2;
            VertexInputAttributeDescription_Normal.binding = 0;
            VertexInputAttributeDescription_Normal.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Normal.offset = offsetof(VertexTextureNormalTangent, Normal);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Normal);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Tangent = {};
        {
            VertexInputAttributeDescription_Tangent.location = 3;
            VertexInputAttributeDescription_Tangent.binding = 0;
            VertexInputAttributeDescription_Tangent.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Tangent.offset = offsetof(VertexTextureNormalTangent, Tangent);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Tangent);
        }
        
        VkVertexInputAttributeDescription VertexInputAttributeDescription_BiTangent = {};
        {
            VertexInputAttributeDescription_BiTangent.location = 4;
            VertexInputAttributeDescription_BiTangent.binding = 0;
            VertexInputAttributeDescription_BiTangent.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_BiTangent.offset = offsetof(VertexTextureNormalTangent, BiTangent);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_BiTangent);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexCubeMap::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexCubeMap);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32B32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexCubeMap, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}

VkPipelineVertexInputStateCreateInfo VertexImGui::GetVkPipelineVertexInputStateCreateInfo()
{
    static VkVertexInputBindingDescription VertexInputBindingDescription = {};
    {
        VertexInputBindingDescription.binding = 0;
        VertexInputBindingDescription.stride = sizeof(VertexImGui);
        VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    static std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
    {
        VkVertexInputAttributeDescription VertexInputAttributeDescription_Position = {};
        {
            VertexInputAttributeDescription_Position.location = 0;
            VertexInputAttributeDescription_Position.binding = 0;
            VertexInputAttributeDescription_Position.format = VK_FORMAT_R32G32_SFLOAT;
            VertexInputAttributeDescription_Position.offset = offsetof(VertexImGui, Position);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Position);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_UV = {};
        {
            VertexInputAttributeDescription_UV.location = 1;
            VertexInputAttributeDescription_UV.binding = 0;
            VertexInputAttributeDescription_UV.format = VK_FORMAT_R32G32_SFLOAT;
            VertexInputAttributeDescription_UV.offset = offsetof(VertexImGui, UV);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_UV);
        }

        VkVertexInputAttributeDescription VertexInputAttributeDescription_Color = {};
        {
            VertexInputAttributeDescription_Color.location = 2;
            VertexInputAttributeDescription_Color.binding = 0;
            VertexInputAttributeDescription_Color.format = VK_FORMAT_R8G8B8A8_UNORM;
            VertexInputAttributeDescription_Color.offset = offsetof(VertexImGui, Color);

            VertexInputAttributeDescriptions.emplace_back(VertexInputAttributeDescription_Color);
        }
    }

    static VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
    {
        VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStateInfo.pNext = nullptr;
        VertexInputStateInfo.flags = 0;
        VertexInputStateInfo.vertexBindingDescriptionCount = 1;
        VertexInputStateInfo.pVertexBindingDescriptions = &VertexInputBindingDescription;
        VertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<UINT32>(VertexInputAttributeDescriptions.size());
        VertexInputStateInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();
    }

    return VertexInputStateInfo;
}
