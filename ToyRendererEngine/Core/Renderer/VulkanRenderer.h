#pragma once
#include "pch.h"

// ********************** Forward Declarations **********************
namespace Core
{
    class Engine;
    class VulkanViewport;
}
namespace RHI
{
    class VulkanCommandBuffer;
}

// ***************************** Core *******************************
namespace Core
{
    class VulkanRenderer : public ICoreExecute
    {
        DISABLE_COPY_CLASS(VulkanRenderer)

    public:
        VulkanRenderer();
        virtual ~VulkanRenderer() final;

    private:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;
        
    private:
        void OnResize(const UINT32& Width, const UINT32& Height);
        
    private:
        void CreateVulkanRenderTarget();

    private:
        bool IsResize = false;

    private:
        friend Core::Engine;
    };
}
