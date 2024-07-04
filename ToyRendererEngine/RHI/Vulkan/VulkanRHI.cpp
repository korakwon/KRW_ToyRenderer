#include "VulkanRHI.h"

#include "VulkanDebug.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanQueue.h"
#include "RenderTarget/VulkanRenderTarget.h"
#include "Buffer/VulkanCommandBuffer.h"

#include "Platform/BaseWindow.h"

using RHI::VulkanRHI;
using RHI::VulkanDevice;

VulkanRHI::VulkanRHI()
{
}
VulkanRHI::~VulkanRHI()
{
    Destroy();
}

void VulkanRHI::BeginRenderFrame()
{
    SwapChain->BeginPresent();
    
    const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer = GetActivateCommandBuffer();
    CommandBuffer->Begin();
}
void VulkanRHI::EndRenderFrame()
{
    const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer = GetActivateCommandBuffer();
    CommandBuffer->End();
    
    SwapChain->EndPresent(CommandBuffer, GetGraphicsQueue(), GetPresentQueue());
}

void VulkanRHI::BeginSingleTimeCommandSubmit(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const
{
    CommandBuffer->Begin();
}
void VulkanRHI::EndSingleTimeCommandSubmit(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const
{
    CommandBuffer->End();
    
    VkSubmitInfo SubmitInfo;
    {
        ZeroVulkanStruct(SubmitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO);
        SubmitInfo.commandBufferCount = 1;
        SubmitInfo.pCommandBuffers = CommandBuffer->GetBufferPtr();
    }

    ASSERT_VK_RESULT(vkQueueSubmit(GetTransferQueue()->GetHandle(), 1, &SubmitInfo, nullptr));
    ASSERT_VK_RESULT(vkQueueWaitIdle(GetTransferQueue()->GetHandle()));
}

void VulkanRHI::SetRenderTarget(const std::shared_ptr<VulkanRenderTarget>& TargetRenderTarget)
{
    ASSERT_NULL(RenderTarget);
    ASSERT_NOT_NULL(TargetRenderTarget);

    RenderTarget = TargetRenderTarget;
    RenderTarget->Create();
}
std::shared_ptr<RHI::VulkanRenderTarget> VulkanRHI::GetRenderTarget()
{
    ASSERT_NOT_NULL(RenderTarget);
    return RenderTarget;
}

void VulkanRHI::Initialize()
{
    CreateVulkanInstance();
    CreateDebugMessenger();
    CreateVulkanSurface();
    CreateVulkanDevice();
    CreateVulkanSwapChain();
    CreateVulkanCommandPool();
    CreateVulkanCommandBuffers();
}
void VulkanRHI::Update(const float& DeltaTime)
{
}
void VulkanRHI::Destroy()
{
    // Wait
    WaitDestroy();

    // RenderTarget
    DestroyVulkanRenderTarget();

    // CommandBuffers
    DestroyVulkanCommandBuffers();

    // CommandPool
    DestroyVulkanCommandPool();

    // SwapChain
    DestroyVulkanSwapChain();

    // Device
    DestroyVulkanDevice();

    // Surface
    DestroyVulkanSurface();

    // Debug
    DestroyDebugMessenger();

    // Instance
    DestroyVulkanInstance();
}

void VulkanRHI::OnResize(const UINT32& Width, const UINT32& Height)
{
    IsResize = true;
    
    WaitDestroy();
    
    SwapChain->ReCreateSwapChain();
    
    DestroyVulkanRenderTarget();
    IsResize = false;
}

void VulkanRHI::WaitDestroy() const
{
    if (Device != nullptr)
    {
        vkDeviceWaitIdle(GetLogicalDevice());
    }
}

void VulkanRHI::CreateVulkanSwapChain()
{
    SwapChain.reset(new VulkanSwapChain());
    ASSERT_NOT_NULL(SwapChain);
}
void VulkanRHI::CreateVulkanCommandPool()
{
    VkDevice Device = GetLogicalDevice();

    VkCommandPoolCreateInfo PoolInfo;
    ZeroVulkanStruct(PoolInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
    PoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    PoolInfo.queueFamilyIndex = GetGraphicsQueue()->GetFamilyIndex();

    ASSERT_VK_RESULT(vkCreateCommandPool(Device, &PoolInfo, nullptr, &CommandPool));
}
void VulkanRHI::CreateVulkanCommandBuffers()
{
    CommandBuffers.clear();

    for (UINT32 i = 0; i < SwapChain->GetBackBufferCount(); i++)
    {
        CommandBuffers.emplace_back(RHI::CreateCommandBuffer());
    }
}

void VulkanRHI::DestroyVulkanSwapChain()
{
    if (SwapChain != nullptr)
    {
        SwapChain->Destroy();
        SwapChain.reset();
        SwapChain = nullptr;
    }
}
void VulkanRHI::DestroyVulkanCommandPool()
{
    if (CommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(GetLogicalDevice(), CommandPool, nullptr);
        CommandPool = VK_NULL_HANDLE;
    }
}
void VulkanRHI::DestroyVulkanCommandBuffers()
{
    if (CommandBuffers.empty() == false)
    {
        for (auto& CommandBuffer : CommandBuffers)
        {
            CommandBuffer->Destroy();
        }
        CommandBuffers.clear();
    }
}
void VulkanRHI::DestroyVulkanRenderTarget()
{
    if(RenderTarget != nullptr)
    {
        RenderTarget->Destroy();
        RenderTarget.reset();
        RenderTarget = nullptr;
    }
}
