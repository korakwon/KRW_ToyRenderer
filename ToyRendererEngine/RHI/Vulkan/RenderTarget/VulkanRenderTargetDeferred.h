#pragma once
#include "VulkanRenderTarget.h"

// ********************** Forward Declarations **********************
namespace Core
{
    class VulkanViewport;
}
namespace RHI
{
    class VulkanTexture;
}

// ****************************** RHI *******************************
namespace RHI
{
    class VulkanRenderTargetDeferred : public VulkanRenderTarget
    {
        DISABLE_COPY_CLASS(VulkanRenderTargetDeferred)

    public:
        VulkanRenderTargetDeferred();
        virtual ~VulkanRenderTargetDeferred() override;

    public:
        virtual void Create() override;
        virtual void Destroy() override;
        
    public:
        virtual void Update(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer) override;
        
    protected:
        virtual void CreateAttachmentTexture() override;
        virtual void CreateRenderPass() override;
        virtual void CreateFrameBuffer() override;

    private:
        void CreateQuadViewport();
        void DestroyQuadViewport();
        
    private:
        std::vector<std::shared_ptr<VulkanTexture>> GBufferAAttachmentTextures;   // ColorAttachmentTextures;
        std::vector<std::shared_ptr<VulkanTexture>> GBufferBAttachmentTextures;   // NormalAttachmentTextures;
        std::vector<std::shared_ptr<VulkanTexture>> GBufferCAttachmentTextures;   // PositionAttachmentTextures;
        
    private:
        std::shared_ptr<Core::VulkanViewport> QuadViewport;
    };
}
