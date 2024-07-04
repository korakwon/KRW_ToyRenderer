#pragma once
#include "VulkanRenderTarget.h"

// ****************************** RHI *******************************
namespace RHI
{
    class VulkanRenderTargetForward : public VulkanRenderTarget
    {
        DISABLE_COPY_CLASS(VulkanRenderTargetForward)

    public:
        VulkanRenderTargetForward();
        virtual ~VulkanRenderTargetForward() override;
        
    public:
        virtual void Update(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer) override;
        
    protected:
        virtual void CreateAttachmentTexture() override;
        virtual void CreateRenderPass() override;
        virtual void CreateFrameBuffer() override;
    };
}
