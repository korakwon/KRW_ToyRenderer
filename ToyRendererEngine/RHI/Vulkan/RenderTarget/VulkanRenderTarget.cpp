#include "VulkanRenderTarget.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanSwapChain.h"
#include "RHI/Vulkan/Buffer/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Texture/VulkanTexture.h"

#include "Application/Application.h"
#include "Platform/BaseWindow.h"
#include "Core/Engine.h"

using RHI::VulkanRHI;
using RHI::VulkanCommandBuffer;
using RHI::VulkanRenderTarget;

// *************************** RenderTarget ********************************
VulkanRenderTarget::VulkanRenderTarget()
{
}
VulkanRenderTarget::~VulkanRenderTarget()
{
    Destroy();
}

void VulkanRenderTarget::Create()
{
    CreateAttachmentTexture();
    CreateRenderPass();
    CreateFrameBuffer();
}
void VulkanRenderTarget::Destroy()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    for (auto& Framebuffer : FrameBuffers)
    {
        vkDestroyFramebuffer(Device, Framebuffer, nullptr);
    }
    FrameBuffers.clear();

    if (RenderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(Device, RenderPass, nullptr);
        RenderPass = VK_NULL_HANDLE;
    }

    CommandBuffer = nullptr;
}

void VulkanRenderTarget::Begin(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer)
{
    ASSERT_NOT_NULL(TargetCommandBuffer);
    CommandBuffer = TargetCommandBuffer;

    VulkanRHI* RHI = Core::GEngine->GetRHI();
    UINT32 BackBufferIndex = RHI->GetVulkanSwapChain()->GetCurrentBackBufferFrameIndex();

    RenderPassBeginInfo.framebuffer = FrameBuffers[BackBufferIndex];

    vkCmdBeginRenderPass(CommandBuffer->GetBuffer(), &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}
bool VulkanRenderTarget::NextSubpass()
{
    ASSERT_NOT_NULL(CommandBuffer);

    bool Result = false;
    if(++CurrentSubpassIndex < MaxSubpassCount)
    {
        vkCmdNextSubpass(CommandBuffer->GetBuffer(), VK_SUBPASS_CONTENTS_INLINE);
        Result = true;  
    }
    return Result;
}
void VulkanRenderTarget::End()
{
    ASSERT_NOT_NULL(CommandBuffer);

    vkCmdEndRenderPass(CommandBuffer->GetBuffer());

    CommandBuffer = nullptr;
    
    CurrentSubpassIndex = 0;
}
