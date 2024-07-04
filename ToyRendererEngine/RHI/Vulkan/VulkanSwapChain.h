#pragma once
#include "VulkanCommon.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanRHI;
    class VulkanDevice;
    class VulkanQueue;
    class VulkanCommandBuffer;
    class VulkanTexture;
}

// ****************************** RHI *******************************
namespace RHI
{
    class VulkanSwapChain
    {
        DECLARE_PRIVATE_CLASS(VulkanSwapChain)

    public:
        ~VulkanSwapChain();

    public:
        void Initialize();
        void Destroy();

    public:
        VkSwapchainKHR GetSwapChain() const { return SwapChain; }

    public:
        VkSurfaceFormatKHR GetSurfaceFormat() const { return SurfaceFormat; }
        VkFormat GetDepthStencilFormat() const { return DepthStencilFormat; }
        
    public:
        std::shared_ptr<VulkanTexture>& GetBackBufferTexture(UINT32 Index)
        {
            ASSERT(BackBufferTextures.empty() == false);
            ASSERT(Index < MaxBackBufferCount);

            return BackBufferTextures[Index];
        }
        std::shared_ptr<VulkanTexture>& GetDepthStencilTexture(UINT32 Index)
        {
            ASSERT(DepthStencilTextures.empty() == false);
            ASSERT(Index < MaxBackBufferCount);

            return DepthStencilTextures[Index];
        }

    public:
        UINT32 GetBackBufferCount() const { return MaxBackBufferCount; }
        UINT32 GetCurrentBackBufferFrameIndex() const { return CurrentBackBufferFrameIndex; }

    private:
        void ReCreateSwapChain();
        void CleanUpSwapChain();
        
    private:
        void BeginPresent();
        void EndPresent(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, const std::shared_ptr<VulkanQueue>& GraphicsQueue, const std::shared_ptr<VulkanQueue> PresentQueue);
        
    private:
        void CreateSwapChain();
        void DestroySwapChain();

    private:
        void CreateBackBufferImageViews();
        void DestroyBackBufferImageViews();
        void CreateDepthStencilBufferImageViews();
        void DestroyDepthStencilBufferImageViews();

    private:
        void QuerySwapChainInfos(VkSurfaceCapabilitiesKHR& Capabilities, std::vector<VkSurfaceFormatKHR>& Formats, std::vector<VkPresentModeKHR>& PresentModes) const;
        void ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& Formats, VkSurfaceFormatKHR& OutputFormat);
        void ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& PresentModes, VkPresentModeKHR& OutputPresentMode);
        VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

    private:
        void CreateSemaphores();
        void DestroySemaphores();
        void CreateFences();
        void DestroyFences();
        
    private:
        VkSwapchainKHR SwapChain = VK_NULL_HANDLE;

    private:
        VkSurfaceCapabilitiesKHR Capabilities;
        VkSurfaceFormatKHR SurfaceFormat;
        VkPresentModeKHR PresentMode;

    private:
        std::vector<std::shared_ptr<VulkanTexture>> BackBufferTextures;
        std::vector<std::shared_ptr<VulkanTexture>> DepthStencilTextures;
        VkFormat DepthStencilFormat = VK_FORMAT_UNDEFINED;

    private:
        VkSemaphore PresentWaitSemaphore = VK_NULL_HANDLE;
        VkSemaphore PresentSignalSemaphore = VK_NULL_HANDLE;
        std::vector<VkSemaphore> ImageAcquireSemaphores;

    private:
        std::vector<VkFence> Fences;
        VkFence RenderFence = VK_NULL_HANDLE;

    private:
        UINT32 MaxBackBufferCount = 3;
        UINT32 CurrentBackBufferFrameIndex = 0;

    private:
        friend RHI::VulkanRHI;
    };
}
