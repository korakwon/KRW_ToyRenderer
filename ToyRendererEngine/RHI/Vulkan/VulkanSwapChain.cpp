#include "VulkanSwapChain.h"
#include "VulkanRHI.h"
#include "VulkanQueue.h"
#include "Buffer/VulkanCommandBuffer.h"
#include "Texture/VulkanTexture.h"

#include "Application/Application.h"
#include "Platform/BaseWindow.h"
#include "Core/Engine.h"

using RHI::VulkanSwapChain;

VulkanSwapChain::VulkanSwapChain()
{
    Initialize();
}
VulkanSwapChain::~VulkanSwapChain()
{
    Destroy();
}

void VulkanSwapChain::Initialize()
{
    Destroy();

    ReCreateSwapChain();
    CreateSemaphores();
    CreateFences();
}
void VulkanSwapChain::Destroy()
{
    DestroyFences();
    DestroySemaphores();
    CleanUpSwapChain();
}

void VulkanSwapChain::ReCreateSwapChain()
{
    CleanUpSwapChain();

    CreateSwapChain();
    CreateBackBufferImageViews();
    CreateDepthStencilBufferImageViews();
}
void VulkanSwapChain::CleanUpSwapChain()
{
    CurrentBackBufferFrameIndex = 0;

    DestroyDepthStencilBufferImageViews();
    DestroyBackBufferImageViews();
    DestroySwapChain();
}

void VulkanSwapChain::BeginPresent()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    VkResult Result = vkAcquireNextImageKHR(Device, SwapChain, UINT64_MAX, ImageAcquireSemaphores[CurrentBackBufferFrameIndex], nullptr, &CurrentBackBufferFrameIndex);
    if ((Result == VK_ERROR_OUT_OF_DATE_KHR) || (Result == VK_SUBOPTIMAL_KHR) || (RHI->IsResized() == true))
    {
        // Resize RHI->OnResize
        return;
    }
    else
    {
        ASSERT_VK_RESULT(Result);
    }

    VkFence LastRenderFence = RenderFence;

    if (LastRenderFence != VK_NULL_HANDLE)
    {
        vkWaitForFences(Device, 1, &LastRenderFence, VK_TRUE, UINT64_MAX);
    }

    PresentWaitSemaphore = ImageAcquireSemaphores[CurrentBackBufferFrameIndex];
    RenderFence = Fences[CurrentBackBufferFrameIndex];
}
void VulkanSwapChain::EndPresent(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, const std::shared_ptr<VulkanQueue>& GraphicsQueue, const std::shared_ptr<VulkanQueue> PresentQueue)
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    VkSubmitInfo SubmitInfo;
    {
        VkPipelineStageFlags WaitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        ZeroVulkanStruct(SubmitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO);
        SubmitInfo.pWaitDstStageMask = &WaitStageMask;
        SubmitInfo.pWaitSemaphores = &PresentWaitSemaphore;
        SubmitInfo.waitSemaphoreCount = 1;
        SubmitInfo.pSignalSemaphores = &PresentSignalSemaphore;
        SubmitInfo.signalSemaphoreCount = 1;
        SubmitInfo.pCommandBuffers = CommandBuffer->GetBufferPtr();
        SubmitInfo.commandBufferCount = 1;
    }

    if (RenderFence != nullptr)
    {
        vkResetFences(Device, 1, &RenderFence);

        ASSERT_VK_RESULT(vkQueueSubmit(GraphicsQueue->GetHandle(), 1, &SubmitInfo, RenderFence));
    }

    VkPresentInfoKHR PresentInfo;
    {
        ZeroVulkanStruct(PresentInfo, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
        PresentInfo.waitSemaphoreCount = 1;
        PresentInfo.pWaitSemaphores = &PresentSignalSemaphore;
        PresentInfo.swapchainCount = 1;
        PresentInfo.pSwapchains = &SwapChain;
        PresentInfo.pImageIndices = &CurrentBackBufferFrameIndex;
    }

    VkResult Result = vkQueuePresentKHR(PresentQueue->GetHandle(), &PresentInfo);
    if ((Result == VK_ERROR_OUT_OF_DATE_KHR) || (Result == VK_SUBOPTIMAL_KHR) || (RHI->IsResized() == true))
    {
        // Resize RHI->OnResize
        return;
    }
    else
    {
        ASSERT_VK_RESULT(Result);
    }

    CurrentBackBufferFrameIndex = (CurrentBackBufferFrameIndex + 1) % MaxBackBufferCount;
}

void VulkanSwapChain::CreateSwapChain()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR> PresentModes;

    QuerySwapChainInfos(Capabilities, Formats, PresentModes);

    ChooseSwapChainFormat(Formats, SurfaceFormat);

    ChooseSwapChainPresentMode(PresentModes, PresentMode);

    VkExtent2D Extent = ChooseSwapChainExtent(Capabilities);

    MaxBackBufferCount = std::clamp(MaxBackBufferCount, Capabilities.minImageCount, Capabilities.maxImageCount);

    // CreateSwapChain
    VkSwapchainCreateInfoKHR SwapChainInfo;
    ZeroVulkanStruct(SwapChainInfo, VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);
    SwapChainInfo.surface = RHI->GetVulkanSurface();
    SwapChainInfo.minImageCount = MaxBackBufferCount;
    SwapChainInfo.imageFormat = SurfaceFormat.format;
    SwapChainInfo.imageColorSpace = SurfaceFormat.colorSpace;
    SwapChainInfo.imageExtent = Extent;
    SwapChainInfo.imageArrayLayers = 1;
    SwapChainInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    SwapChainInfo.preTransform = Capabilities.currentTransform;
    SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SwapChainInfo.presentMode = PresentMode;
    SwapChainInfo.clipped = VK_TRUE;

    ASSERT_VK_RESULT(vkCreateSwapchainKHR(Device, &SwapChainInfo, nullptr, &SwapChain));
}
void VulkanSwapChain::DestroySwapChain()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    if (SwapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(Device, SwapChain, nullptr);
        SwapChain = VK_NULL_HANDLE;
    }
}
void VulkanSwapChain::CreateBackBufferImageViews()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    UINT32 BackBufferImageCount = 0;
    ASSERT_VK_RESULT(vkGetSwapchainImagesKHR(Device, SwapChain, &BackBufferImageCount, nullptr));

    std::vector<VkImage> BackBufferImages;
    BackBufferImages.resize(BackBufferImageCount);
    ASSERT_VK_RESULT(vkGetSwapchainImagesKHR(Device, SwapChain, &BackBufferImageCount, BackBufferImages.data()));

    MaxBackBufferCount = BackBufferImageCount;

    for (UINT32 i = 0; i < MaxBackBufferCount; i++)
    {
        std::shared_ptr<VulkanTexture> BackBuffer = std::make_shared<VulkanTexture>();
        BackBuffer->SetImage(BackBufferImages[i], SurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, false);

        BackBufferTextures.emplace_back(BackBuffer);
    }
}
void VulkanSwapChain::DestroyBackBufferImageViews()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    for (UINT32 i = 0; i < BackBufferTextures.size(); i++)
    {
        BackBufferTextures[i]->Destroy();
    }
    BackBufferTextures.clear();
}
void VulkanSwapChain::CreateDepthStencilBufferImageViews()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkPhysicalDevice PhysicalDevice = RHI->GetPhysicalDevice();

    // Find DepthStencilFormat
    {
        const std::vector<VkFormat> Candidates = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
        VkImageTiling Tiling = VK_IMAGE_TILING_OPTIMAL;
        VkFormatFeatureFlags Features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

        for (const VkFormat Format : Candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(PhysicalDevice, Format, &props);

            if ((Tiling == VK_IMAGE_TILING_LINEAR) && ((props.linearTilingFeatures & Features) == Features))
            {
                DepthStencilFormat = Format;
                break;
            }
            else if ((Tiling == VK_IMAGE_TILING_OPTIMAL) && ((props.optimalTilingFeatures & Features) == Features))
            {
                DepthStencilFormat = Format;
                break;
            }
        }

        ASSERT(DepthStencilFormat != VK_FORMAT_UNDEFINED);
    }

    UINT32 Width = 0, Height = 0;
    APPLICATION->GetWindow()->GetExtend(Width, Height);

    for (UINT32 i = 0; i < MaxBackBufferCount; i++)
    {
        std::shared_ptr<VulkanTexture> DepthStencil = std::make_shared<VulkanTexture>();
        DepthStencil->CreateImage(Width,
                                  Height,
                                  1,
                                  1,
                                  DepthStencilFormat,
                                  VK_IMAGE_TILING_OPTIMAL,
                                  VK_IMAGE_VIEW_TYPE_2D,
                                  VK_IMAGE_ASPECT_DEPTH_BIT,
                                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        DepthStencilTextures.emplace_back(DepthStencil);
    }
}
void VulkanSwapChain::DestroyDepthStencilBufferImageViews()
{
    for (UINT32 i = 0; i < DepthStencilTextures.size(); i++)
    {
        DepthStencilTextures[i]->Destroy();
    }
    DepthStencilTextures.clear();
}

void VulkanSwapChain::QuerySwapChainInfos(VkSurfaceCapabilitiesKHR& Capabilities, std::vector<VkSurfaceFormatKHR>& Formats, std::vector<VkPresentModeKHR>& PresentModes) const
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkPhysicalDevice PhysicalDevice = RHI->GetPhysicalDevice();
    VkSurfaceKHR Surface = RHI->GetVulkanSurface();

    // Capability
    ASSERT_VK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &Capabilities));

    // Format
    UINT32 FormatCount = 0;
    ASSERT_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &FormatCount, nullptr));

    Formats.resize(FormatCount);
    ASSERT_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &FormatCount, Formats.data()));

    // PresentMOde
    UINT32 PresentModeCount = 0;
    ASSERT_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, nullptr));

    PresentModes.resize(PresentModeCount);
    ASSERT_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, PresentModes.data()));
}
void VulkanSwapChain::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& Formats, VkSurfaceFormatKHR& OutputFormat)
{
    for (const auto& Format : Formats)
    {
        if ((Format.format == VK_FORMAT_R8G8B8A8_SRGB) &&
            (Format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
        {
            OutputFormat = Format;
            return;
        }
    }

    assert(false);
    OutputFormat = Formats[0];
}
void VulkanSwapChain::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& PresentModes, VkPresentModeKHR& OutputPresentMode)
{
    for (const auto& PresentMode : PresentModes)
    {
        if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            OutputPresentMode = PresentMode;
            return;
        }
    }

    assert(false);
    OutputPresentMode = VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D VulkanSwapChain::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
{
    if (Capabilities.currentExtent.width != UINT32_MAX)
    {
        return Capabilities.currentExtent;
    }
    else
    {
        UINT32 Width = 0, Height = 0;
        APPLICATION->GetWindow()->GetExtend(Width, Height);

        VkExtent2D Extent = {static_cast<UINT32>(Width), static_cast<UINT32>(Height)};

        Extent.width = std::clamp(Extent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
        Extent.height = std::clamp(Extent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

        return Extent;
    }
}

void VulkanSwapChain::CreateSemaphores()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    VkSemaphoreCreateInfo SemaphoreCreateInfo;
    ZeroVulkanStruct(SemaphoreCreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);

    ImageAcquireSemaphores.resize(MaxBackBufferCount);
    for (UINT32 i = 0; i < MaxBackBufferCount; i++)
    {
        ASSERT_VK_RESULT(vkCreateSemaphore(Device, &SemaphoreCreateInfo, nullptr, &ImageAcquireSemaphores[i]));
    }

    ASSERT_VK_RESULT(vkCreateSemaphore(Device, &SemaphoreCreateInfo, nullptr, &PresentSignalSemaphore));
}
void VulkanSwapChain::DestroySemaphores()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    if (PresentSignalSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(Device, PresentSignalSemaphore, nullptr);
        PresentSignalSemaphore = VK_NULL_HANDLE;
    }

    for (auto Semaphore : ImageAcquireSemaphores)
    {
        vkDestroySemaphore(Device, Semaphore, nullptr);
    }
    ImageAcquireSemaphores.clear();
}
void VulkanSwapChain::CreateFences()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    Fences.resize(MaxBackBufferCount);

    VkFenceCreateInfo FenceCreateInfo;
    ZeroVulkanStruct(FenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
    FenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (UINT32 i = 0; i < MaxBackBufferCount; i++)
    {
        ASSERT_VK_RESULT(vkCreateFence(Device, &FenceCreateInfo, nullptr, &Fences[i]));
    }
}
void VulkanSwapChain::DestroyFences()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    if (RenderFence != nullptr)
    {
        vkResetFences(Device, 1, &RenderFence);
        RenderFence = nullptr;
    }

    for (auto Fence : Fences)
    {
        vkDestroyFence(Device, Fence, nullptr);
    }
    Fences.clear();
}
