#pragma once
#include "VulkanCommon.h"

struct IVertexInfo;
struct VkPipelineVertexInputStateCreateInfo;

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanPipeline;
}

// ***************************** RHI ******************************
namespace RHI
{
    struct VulkanPipelineInfo
    {
    public:
        VulkanPipelineInfo();

    public:
        void SetVertexCreateInfo(const VkPipelineVertexInputStateCreateInfo& Info);
        void SetLineList(const bool& State);
        void SetCullMode(const ERasterizationInfo_CullModeType& Type);
        void SetFrontFace(const ERasterizationInfo_FrontFaceModeType& Type);
        void SetDepthStencilEnable(const bool& State);
        void SetOutputBufferCount(const UINT32& Count);
        void SetSubpassCount(const UINT32& Count);

    private:
        VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
        VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateInfo;
        VkPipelineTessellationStateCreateInfo TessellationStateCreateInfo;
        VkPipelineViewportStateCreateInfo ViewportStateInfo;
        VkPipelineRasterizationStateCreateInfo RasterizationStateInfo;
        VkPipelineMultisampleStateCreateInfo MultisampleStateInfo;
        VkPipelineDepthStencilStateCreateInfo DepthStencilStateInfo;
        VkPipelineColorBlendStateCreateInfo ColorBlendStateInfo;
        VkPipelineDynamicStateCreateInfo DynamicStateInfo;

    private:
        std::vector<VkDynamicState> DynamicStates;
        std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachmentStates;

    private:
        UINT32 Subpass = 0;

    private:
        friend VulkanPipeline;
    };

    template <ERasterizationInfo_CullModeType CullMode = ERasterizationInfo_CullModeType::Back,
              ERasterizationInfo_FrontFaceModeType FrontFaceMode = ERasterizationInfo_FrontFaceModeType::CCW,
              bool IsDepthStencil = true,
              UINT32 OutputBufferCount = 3>
    struct VulkanPipelineInfoProxy
    {
        static VulkanPipelineInfo Create()
        {
            VulkanPipelineInfo PipelineInfo;

            PipelineInfo.SetCullMode(CullMode);
            PipelineInfo.SetFrontFace(FrontFaceMode);
            PipelineInfo.SetDepthStencilEnable(IsDepthStencil);
            PipelineInfo.SetOutputBufferCount(OutputBufferCount);

            return PipelineInfo;
        }
    };
}
