#pragma once
#include "VulkanCommon.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

// ********************** Forward Declarations **********************
namespace Core
{
    class Engine;
}

namespace RHI
{
    class VulkanDevice;
    class VulkanSwapChain;
    class VulkanPipeline;
    class VulkanCommandBuffer;
    class VulkanRenderTarget;
}

// ****************************** RHI *******************************
namespace RHI
{
    class VulkanRHI : public ICoreExecute
    {
        DISABLE_COPY_CLASS(VulkanRHI)

    public:
        VulkanRHI();
        virtual ~VulkanRHI() final;

    public:
        void BeginRenderFrame();
        void EndRenderFrame();

    public:
        void BeginSingleTimeCommandSubmit(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const;
        void EndSingleTimeCommandSubmit(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const;

    public:
        VkInstance GetVulkanInstance() const { return Instance; }
        VkSurfaceKHR GetVulkanSurface() const { return Surface; }
        std::shared_ptr<VulkanDevice> GetVulkanDevice() const { return Device; }
        std::shared_ptr<VulkanSwapChain> GetVulkanSwapChain() const { return SwapChain; }

    public:
        VkPhysicalDevice GetPhysicalDevice() const
        {
            ASSERT_NOT_NULL(Device->PhysicalDevice);
            return Device->PhysicalDevice;
        }
        VkDevice GetLogicalDevice() const
        {
            ASSERT_NOT_NULL(Device->LogicalDevice);
            return Device->LogicalDevice;
        }

    public:
        std::shared_ptr<VulkanQueue> GetGraphicsQueue() const { return Device->GraphicsQueue; }
        std::shared_ptr<VulkanQueue> GetComputeQueue() const { return Device->ComputeQueue; }
        std::shared_ptr<VulkanQueue> GetTransferQueue() const { return Device->TransferQueue; }
        std::shared_ptr<VulkanQueue> GetPresentQueue() const { return Device->PresentQueue; }

    public:
        std::shared_ptr<VulkanCommandBuffer> GetCommandBuffer(UINT32 Index)
        {
            ASSERT(CommandBuffers.size() > Index);
            return CommandBuffers[Index];
        }
        std::shared_ptr<VulkanCommandBuffer> GetActivateCommandBuffer()
        {
            return GetCommandBuffer(SwapChain->GetCurrentBackBufferFrameIndex());
        }
        VkCommandPool GetCommandPool() const { return CommandPool; }

    public:
        void SetRenderTarget(const std::shared_ptr<VulkanRenderTarget>& TargetRenderTarget);
        std::shared_ptr<VulkanRenderTarget> GetRenderTarget();

    public:
        bool IsResized() const { return IsResize; }

    private:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;

    private:
        void OnResize(const UINT32& Width, const UINT32& Height);

    public:
        void WaitDestroy() const;

    private:
        void CreateVulkanInstance();
        void CreateVulkanSurface();
        void CreateVulkanDevice();
        void CreateVulkanSwapChain();
        void CreateVulkanCommandPool();
        void CreateVulkanCommandBuffers();

    private:
        void DestroyVulkanInstance();
        void DestroyVulkanSurface();
        void DestroyVulkanDevice();
        void DestroyVulkanSwapChain();
        void DestroyVulkanCommandPool();
        void DestroyVulkanCommandBuffers();
        void DestroyVulkanRenderTarget();

    private:
        VkInstance Instance = VK_NULL_HANDLE;
        VkSurfaceKHR Surface = VK_NULL_HANDLE;
        std::shared_ptr<VulkanDevice> Device = nullptr;
        std::shared_ptr<VulkanSwapChain> SwapChain = nullptr;

    private:
        std::vector<std::shared_ptr<VulkanCommandBuffer>> CommandBuffers;
        VkCommandPool CommandPool;

    private:
        std::shared_ptr<VulkanRenderTarget> RenderTarget;

    private:
        bool IsResize = false;

    private:
        friend Core::Engine;
    };
}
