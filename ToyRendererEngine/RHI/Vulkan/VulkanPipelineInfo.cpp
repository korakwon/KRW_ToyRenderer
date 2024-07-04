#include "VulkanPipelineInfo.h"

using RHI::VulkanPipelineInfo;

// *************************** VulkanPipelineInfo ****************************
VulkanPipelineInfo::VulkanPipelineInfo()
{
    // VertexInputStateInfo
    {
        ZeroVulkanStruct(VertexInputStateInfo, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
    }

    // InputAssemblyStateInfo
    {
        ZeroVulkanStruct(InputAssemblyStateInfo, VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
        InputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        InputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;
    }
    
    // InputAssemblyStateInfo
    {
        ZeroVulkanStruct(TessellationStateCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO);
    }

    // ViewportStateInfo
    {
        ZeroVulkanStruct(ViewportStateInfo, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
        ViewportStateInfo.viewportCount = 1;
        ViewportStateInfo.scissorCount  = 1;
    }

    // RasterizationStateInfo
    {
        ZeroVulkanStruct(RasterizationStateInfo, VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
        RasterizationStateInfo.depthClampEnable = VK_FALSE;
        RasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
        RasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        RasterizationStateInfo.lineWidth = 1.0f;
        RasterizationStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        RasterizationStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        RasterizationStateInfo.depthBiasEnable = VK_FALSE;
    }

    // MultisampleStateInfo
    {
        ZeroVulkanStruct(MultisampleStateInfo, VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO);
        MultisampleStateInfo.sampleShadingEnable = VK_FALSE;
        MultisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }

    // DepthStencilStateInfo
    {
        ZeroVulkanStruct(DepthStencilStateInfo, VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
        DepthStencilStateInfo.depthTestEnable = VK_TRUE;
        DepthStencilStateInfo.depthWriteEnable = VK_TRUE;
        DepthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        DepthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        DepthStencilStateInfo.stencilTestEnable = VK_FALSE;
    }

    // ColorBlendStateInfo
    {
        ZeroVulkanStruct(ColorBlendStateInfo, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
    }

    // DynamicStateInfo
    {
        DynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        DynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
        
        ZeroVulkanStruct(DynamicStateInfo, VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
        DynamicStateInfo.dynamicStateCount = static_cast<UINT32>(DynamicStates.size());
        DynamicStateInfo.pDynamicStates = DynamicStates.data();
    }
    
    // ColorBlendAttachmentState
    {
        for(int i = 0; i < 8; i++)
        {
            // todo : Alpha 쓸지 말지 결정 필요
            VkPipelineColorBlendAttachmentState ColorBlendAttachmentState = {};
            ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            ColorBlendAttachmentState.blendEnable = VK_TRUE;
            ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            ColorBlendAttachmentState.colorBlendOp        = VK_BLEND_OP_ADD;
            ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            ColorBlendAttachmentState.alphaBlendOp        = VK_BLEND_OP_ADD;
            
            ColorBlendAttachmentStates.emplace_back(ColorBlendAttachmentState);
        }

    }
}

void VulkanPipelineInfo::SetVertexCreateInfo(const VkPipelineVertexInputStateCreateInfo& Info)
{
    UINT32 Size = sizeof(VkPipelineVertexInputStateCreateInfo);
    memcpy_s(&VertexInputStateInfo, Size, &Info, Size);
}
void VulkanPipelineInfo::SetLineList(const bool& State)
{
    if(State == true)
    {
        InputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }
    else
    {
        InputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
}
void VulkanPipelineInfo::SetCullMode(const ERasterizationInfo_CullModeType& Type)
{
    switch (Type)
    {
    case ERasterizationInfo_CullModeType::None:
        RasterizationStateInfo.cullMode = VK_CULL_MODE_NONE;
        break;
    case ERasterizationInfo_CullModeType::Front:
        RasterizationStateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
        break;
    case ERasterizationInfo_CullModeType::Back:
        RasterizationStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        break;
    case ERasterizationInfo_CullModeType::FrontAndBack:
        RasterizationStateInfo.cullMode = VK_CULL_MODE_FRONT_AND_BACK;
        break;
    }
}
void VulkanPipelineInfo::SetFrontFace(const ERasterizationInfo_FrontFaceModeType& Type)
{
    switch (Type)
    {
    case ERasterizationInfo_FrontFaceModeType::CW:
        RasterizationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        break;
    case ERasterizationInfo_FrontFaceModeType::CCW:
        RasterizationStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        break;
    }
}
void VulkanPipelineInfo::SetDepthStencilEnable(const bool& State)
{
    DepthStencilStateInfo.depthTestEnable = State;
    DepthStencilStateInfo.depthWriteEnable = State;
    DepthStencilStateInfo.stencilTestEnable = State;
}
void VulkanPipelineInfo::SetOutputBufferCount(const UINT32& Count)
{
    ColorBlendAttachmentStates.resize(Count);
}
void VulkanPipelineInfo::SetSubpassCount(const UINT32& Count)
{
    Subpass = Count;
}