#pragma once
#include "VulkanCommon.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanBuffer;
    class VulkanTexture;
    class VulkanUniformBuffer;
    class VulkanCommandBuffer;
    class VulkanShader;
    struct VulkanPipelineInfo;
}

// ***************************** RHI ******************************
namespace RHI
{
    class VulkanPipeline
    {
        DISABLE_COPY_CLASS(VulkanPipeline)

    public:
        VulkanPipeline();
        ~VulkanPipeline();

    public:
        void Destroy();

    public:
        void Init(VulkanPipelineInfo& PipelineInfo, const std::shared_ptr<RHI::VulkanShader>& Shader);

    public:
        void UpdateViewportInfo(const VkViewport& ViewportInfo);
        void UpdateScissorInfo(const VkRect2D& ScissorInfo);
        
    public:
        void Bind(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, VkPipelineBindPoint PipelineBindPoint) const;
        void Draw(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const;

    private:
        VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
        VkPipeline Pipeline = VK_NULL_HANDLE;

    private:
        VkViewport Viewport = {};
        VkRect2D Scissor = {};
        
    private:
        friend RHI::VulkanShader;
    };
}
