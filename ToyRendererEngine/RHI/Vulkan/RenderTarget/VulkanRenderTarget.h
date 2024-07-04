#pragma once
#include "RHI/Vulkan/VulkanCommon.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanRHI;
    class VulkanCommandBuffer;
}

// ****************************** RHI *******************************
namespace RHI
{
    class VulkanRenderTarget
    {
        DISABLE_COPY_CLASS(VulkanRenderTarget)

    public:
        VulkanRenderTarget();
        virtual ~VulkanRenderTarget();

    public:
        virtual void Create();
        virtual void Destroy();

    public:
        virtual void Update(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer) = 0;
        
    public:
        VkRenderPass* GetRenderPassPtr() { return &RenderPass; }
        VkRenderPass GetRenderPass() const { return RenderPass; }

    public:
        UINT32 GetMaxSubpassCount() const { return MaxSubpassCount; }

    protected:
        void Begin(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer);
        bool NextSubpass();
        void End();

    protected:
        virtual void CreateAttachmentTexture() = 0;
        virtual void CreateRenderPass() = 0;
        virtual void CreateFrameBuffer() = 0;
        
    protected:
        std::shared_ptr<VulkanCommandBuffer> CommandBuffer = nullptr;

    protected:
        VkRenderPass RenderPass = VK_NULL_HANDLE;
        VkRenderPassBeginInfo RenderPassBeginInfo = {};
        std::vector<VkFramebuffer> FrameBuffers;

    protected:
        UINT32 CurrentSubpassIndex = 0;
        UINT32 MaxSubpassCount = 1;
        
    protected:
        std::vector<VkClearValue> ClearValues;

    protected:
        friend RHI::VulkanRHI;
    };
}
