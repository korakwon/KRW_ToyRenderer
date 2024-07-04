#include "VulkanRenderTargetForward.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/Texture/VulkanTexture.h"

#include "Core/Engine.h"
#include "Application/Application.h"
#include "Platform/BaseWindow.h"
#include "Scene/World/World.h"

using RHI::VulkanRenderTarget;
using RHI::VulkanRenderTargetForward;

// *************************** RenderTarget ********************************
VulkanRenderTargetForward::VulkanRenderTargetForward()
{
}
VulkanRenderTargetForward::~VulkanRenderTargetForward()
{
    Destroy();
}

void VulkanRenderTargetForward::Update(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer)
{
    Begin(TargetCommandBuffer);
    {
        // todo : 바꿔주기
        Core::GEngine->GetWorld()->RenderGameObjects(ERenderPassType::Main);
    }
    End();
}

void VulkanRenderTargetForward::CreateAttachmentTexture()
{
}
void VulkanRenderTargetForward::CreateRenderPass()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    std::vector<VkAttachmentDescription> AttachmentDescriptions;
    VkAttachmentReference SwapChainAttachmentReference = {};
    VkAttachmentReference DepthStencilAttachmentReference = {};
    {
        // AttachmentDescription
        {
            VkAttachmentDescription SwapChainAttachmentDescription = {};
            {
                SwapChainAttachmentDescription.format = RHI->GetVulkanSwapChain()->GetSurfaceFormat().format;
                SwapChainAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                SwapChainAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                SwapChainAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                SwapChainAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                SwapChainAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                SwapChainAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                SwapChainAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            }
            VkAttachmentDescription DepthStencilAttachmentDescription = {};
            {
                DepthStencilAttachmentDescription.format = RHI->GetVulkanSwapChain()->GetDepthStencilFormat();
                DepthStencilAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                DepthStencilAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                DepthStencilAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                DepthStencilAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                DepthStencilAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                DepthStencilAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                DepthStencilAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }
                
            AttachmentDescriptions.emplace_back(SwapChainAttachmentDescription);
            AttachmentDescriptions.emplace_back(DepthStencilAttachmentDescription);
        }

        // AttachmentReference
        {
            SwapChainAttachmentReference = {};
            {
                SwapChainAttachmentReference.attachment = 0;
                SwapChainAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            
            DepthStencilAttachmentReference = {};
            {
                DepthStencilAttachmentReference.attachment = 1;
                DepthStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }
        }
        
        // ClearValue
        ClearValues.clear();
        {
            VkClearValue SwapChainClearValue = {};
            {
                SwapChainClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            }
            VkClearValue DepthStencilClearValue = {};
            {
                DepthStencilClearValue.depthStencil = {1.0f, 0};
            }
                
            ClearValues.emplace_back(SwapChainClearValue);
            ClearValues.emplace_back(DepthStencilClearValue);
        }
    }

    std::vector<VkSubpassDescription> SubpassDescriptions;
    {
        VkSubpassDescription SwapChainSubpassDescription = {};
        {
            SwapChainSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            SwapChainSubpassDescription.colorAttachmentCount = 1;
            SwapChainSubpassDescription.pColorAttachments = &SwapChainAttachmentReference;
            SwapChainSubpassDescription.pDepthStencilAttachment = &DepthStencilAttachmentReference;
        }
            
        SubpassDescriptions.emplace_back(SwapChainSubpassDescription);
    }

    std::vector<VkSubpassDependency> SubpassDependencies;
    {
        VkSubpassDependency Dependency1 = {};
        {
            Dependency1.srcSubpass = VK_SUBPASS_EXTERNAL;
            Dependency1.dstSubpass = 0;
            Dependency1.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            Dependency1.srcAccessMask = 0;
            Dependency1.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            Dependency1.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        }

        SubpassDependencies.emplace_back(Dependency1);
    }

    VkRenderPassCreateInfo RenderPassCreateInfo;
    {
        ZeroVulkanStruct(RenderPassCreateInfo, VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO);
        RenderPassCreateInfo.attachmentCount = static_cast<UINT32>(AttachmentDescriptions.size());
        RenderPassCreateInfo.pAttachments = AttachmentDescriptions.data();
        RenderPassCreateInfo.subpassCount = static_cast<UINT32>(SubpassDescriptions.size());
        RenderPassCreateInfo.pSubpasses = SubpassDescriptions.data();
        RenderPassCreateInfo.dependencyCount = static_cast<UINT32>(SubpassDependencies.size());
        RenderPassCreateInfo.pDependencies = SubpassDependencies.data();
    }

    ASSERT_VK_RESULT(vkCreateRenderPass(Device, &RenderPassCreateInfo, nullptr, &RenderPass));
        
    CurrentSubpassIndex = 0;
    MaxSubpassCount = static_cast<UINT32>(SubpassDescriptions.size());
}
void VulkanRenderTargetForward::CreateFrameBuffer()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    UINT32 RenderWidth = 0, RenderHeight = 0;
    APPLICATION->GetWindow()->GetExtend(RenderWidth, RenderHeight);

    // RenderPassBeginInfo
    {
        ZeroVulkanStruct(RenderPassBeginInfo, VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO);
        RenderPassBeginInfo.renderPass = RenderPass;
        RenderPassBeginInfo.renderArea.offset = {0, 0};
        RenderPassBeginInfo.renderArea.extent = {RenderWidth, RenderHeight};
        RenderPassBeginInfo.clearValueCount = static_cast<UINT32>(ClearValues.size());
        RenderPassBeginInfo.pClearValues = ClearValues.data();
    }

    // FrameBuffer
    {
        FrameBuffers.resize(RHI->GetVulkanSwapChain()->GetBackBufferCount());

        for (UINT32 i = 0; i < FrameBuffers.size(); i++)
        {
            std::array<VkImageView, 2> Attachments = {
                RHI->GetVulkanSwapChain()->GetBackBufferTexture(i)->GetImageView(),
                RHI->GetVulkanSwapChain()->GetDepthStencilTexture(i)->GetImageView()
            };

            VkFramebufferCreateInfo FrameBufferInfo;
            {
                ZeroVulkanStruct(FrameBufferInfo, VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO);
                FrameBufferInfo.renderPass = RenderPass;
                FrameBufferInfo.attachmentCount = static_cast<UINT32>(Attachments.size());
                FrameBufferInfo.pAttachments = Attachments.data();
                FrameBufferInfo.width = RenderWidth;
                FrameBufferInfo.height = RenderHeight;
                FrameBufferInfo.layers = 1;
            }

            ASSERT_VK_RESULT(vkCreateFramebuffer(Device, &FrameBufferInfo, nullptr, &FrameBuffers[i]));
        }
    }
}
