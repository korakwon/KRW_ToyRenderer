#pragma once
#include "VulkanCommon.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    struct VulkanPipelineInfo;
    class VulkanPipeline;
    class VulkanShader;
    class VulkanUniformBuffer;
    class VulkanCommandBuffer;
    class VulkanTexture;
}

// ***************************** RHI ******************************
namespace RHI
{
    class VulkanMaterial
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(VulkanMaterial)
        
    public:
        VulkanMaterial(VulkanPipelineInfo& PipelineInfo, const std::shared_ptr<VulkanShader>& TargetShader);
        virtual ~VulkanMaterial();

    public:
        virtual void Destroy();

    public:
        void WriteUniformBuffer(const std::string& DescriptorSetLayoutName, VulkanUniformBuffer* UniformBuffer) const;
        void WriteTexture(const std::string& DescriptorSetLayoutName, VulkanTexture* Texture) const;

    public:
        void BindFrame(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, VkPipelineBindPoint PipelineBindPoint);
        void DrawFrame(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) const;
        void EndFrame();

    public:
        void UpdateViewportInfo(const VkViewport& ViewportInfo) const;
        void UpdateScissorInfo(const VkRect2D& ScissorInfo) const;
        
    private:
        bool IsActivate = false;
        
    private:
        std::shared_ptr<VulkanPipeline> Pipeline;
        std::shared_ptr<VulkanShader> Shader;
    };
}
