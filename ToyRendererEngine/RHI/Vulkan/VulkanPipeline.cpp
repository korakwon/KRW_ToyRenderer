#include "VulkanPipeline.h"

#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanPipelineInfo.h"
#include "RHI/Vulkan/RenderTarget/VulkanRenderTarget.h"
#include "RHI/Vulkan/Buffer/VulkanCommandBuffer.h"
#include "RHI/Vulkan/VulkanShader.h"

#include "Core/Engine.h"

using RHI::VulkanPipelineInfo;
using RHI::VulkanPipeline;

// ***************************** VulkanPipeline ******************************
VulkanPipeline::VulkanPipeline()
{
}
VulkanPipeline::~VulkanPipeline()
{
    Destroy();
}

void VulkanPipeline::Destroy()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    // Pipeline
    if (Pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(Device, Pipeline, nullptr);
        Pipeline = VK_NULL_HANDLE;
    }

    // PipelineLayout
    if (PipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(Device, PipelineLayout, nullptr);
        PipelineLayout = VK_NULL_HANDLE;
    }
}

void VulkanPipeline::Init(VulkanPipelineInfo& PipelineInfo, const std::shared_ptr<RHI::VulkanShader>& Shader)
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    // VertexInputStateInfo
    {
        // PipelineInfo.VertexInputStateInfo;
    }

    // InputAssemblyStateInfo
    {
        // PipelineInfo.InputAssemblyStateInfo;
    }
    
    // InputAssemblyStateInfo
    {
        // PipelineInfo.TessellationStateCreateInfo;
    }

    // ViewportStateInfo
    {
        /*UINT32 Width = 0, Height = 0;
        APPLICATION->GetWindow()->GetExtend(Width, Height);

        VkViewport Viewport;
        Viewport.x = 0.0f;
        Viewport.y = 0.0f;
        Viewport.width = static_cast<float>(Width);
        Viewport.height = static_cast<float>(Height);
        Viewport.minDepth = 0.0f;
        Viewport.maxDepth = 1.0f;

        VkRect2D Scissor;
        Scissor.offset = { 0, 0 };
        Scissor.extent = { Width, Height };

        PipelineInfo.ViewportStateInfo.viewportCount = 1;
        PipelineInfo.ViewportStateInfo.pViewports = &Viewport;
        PipelineInfo.ViewportStateInfo.scissorCount = 1;
        PipelineInfo.ViewportStateInfo.pScissors = &Scissor;*/
    }

    // RasterizationStateInfo
    {
        // PipelineInfo.RasterizationStateInfo;
    }

    // MultisampleStateInfo
    {
        // PipelineInfo.MultisampleStateInfo;
    }

    // DepthStencilStateInfo
    {
        // PipelineInfo.DepthStencilStateInfo;
    }

    // ColorBlendStateInfo
    {
        PipelineInfo.ColorBlendStateInfo.logicOpEnable = VK_FALSE;
        PipelineInfo.ColorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
        PipelineInfo.ColorBlendStateInfo.attachmentCount = static_cast<UINT32>(PipelineInfo.ColorBlendAttachmentStates.size());
        PipelineInfo.ColorBlendStateInfo.pAttachments = PipelineInfo.ColorBlendAttachmentStates.data();
        PipelineInfo.ColorBlendStateInfo.blendConstants[0] = 0.0f;
        PipelineInfo.ColorBlendStateInfo.blendConstants[1] = 0.0f;
        PipelineInfo.ColorBlendStateInfo.blendConstants[2] = 0.0f;
        PipelineInfo.ColorBlendStateInfo.blendConstants[3] = 0.0f;
    }

    // DynamicStateInfo;
    {
        //PipelineInfo.DynamicStateInfo;
    }

    VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo;
    {
        ZeroVulkanStruct(PipelineLayoutCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
        PipelineLayoutCreateInfo.setLayoutCount = 1;
        PipelineLayoutCreateInfo.pSetLayouts = Shader->GetDescriptorSetLayout();

        ASSERT_VK_RESULT(vkCreatePipelineLayout(Device, &PipelineLayoutCreateInfo, nullptr, &PipelineLayout));
    }

    VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo;
    {
        ZeroVulkanStruct(GraphicsPipelineCreateInfo, VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);
        GraphicsPipelineCreateInfo.stageCount = static_cast<UINT32>(Shader->GetPipelineShaderStageInfos().size());
        GraphicsPipelineCreateInfo.pStages = Shader->GetPipelineShaderStageInfos().data();
        GraphicsPipelineCreateInfo.pVertexInputState = &PipelineInfo.VertexInputStateInfo;
        GraphicsPipelineCreateInfo.pInputAssemblyState = &PipelineInfo.InputAssemblyStateInfo;
        GraphicsPipelineCreateInfo.pTessellationState = &PipelineInfo.TessellationStateCreateInfo;
        GraphicsPipelineCreateInfo.pViewportState = &PipelineInfo.ViewportStateInfo;
        GraphicsPipelineCreateInfo.pRasterizationState = &PipelineInfo.RasterizationStateInfo;
        GraphicsPipelineCreateInfo.pMultisampleState = &PipelineInfo.MultisampleStateInfo;
        GraphicsPipelineCreateInfo.pDepthStencilState = &PipelineInfo.DepthStencilStateInfo;
        GraphicsPipelineCreateInfo.pColorBlendState = &PipelineInfo.ColorBlendStateInfo;
        GraphicsPipelineCreateInfo.pDynamicState = &PipelineInfo.DynamicStateInfo;
        GraphicsPipelineCreateInfo.layout = PipelineLayout;
        GraphicsPipelineCreateInfo.renderPass = RHI->GetRenderTarget()->GetRenderPass();
        GraphicsPipelineCreateInfo.subpass = PipelineInfo.Subpass;
        GraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        GraphicsPipelineCreateInfo.basePipelineIndex = -1;
    }

    ASSERT_VK_RESULT(vkCreateGraphicsPipelines(Device, nullptr, 1, &GraphicsPipelineCreateInfo, nullptr, &Pipeline));
}

void VulkanPipeline::UpdateViewportInfo(const VkViewport& ViewportInfo)
{
    memcpy(&Viewport, &ViewportInfo, sizeof(VkViewport));
}
void VulkanPipeline::UpdateScissorInfo(const VkRect2D& ScissorInfo)
{
    memcpy(&Scissor, &ScissorInfo, sizeof(VkRect2D));
}

void VulkanPipeline::Bind(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, VkPipelineBindPoint PipelineBindPoint) const
{
    ASSERT_NOT_NULL(CommandBuffer);
    
    vkCmdBindPipeline(CommandBuffer->GetBuffer(), PipelineBindPoint, Pipeline);
}
void VulkanPipeline::Draw(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const
{
    ASSERT_NOT_NULL(CommandBuffer);
    
    vkCmdSetViewport(CommandBuffer->GetBuffer(), 0, 1, &Viewport);
    vkCmdSetScissor(CommandBuffer->GetBuffer(), 0, 1, &Scissor);
}

