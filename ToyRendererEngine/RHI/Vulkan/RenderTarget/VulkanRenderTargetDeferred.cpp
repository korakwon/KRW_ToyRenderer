#include "VulkanRenderTargetDeferred.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/Texture/VulkanTexture.h"

#include "Core/Engine.h"
#include "Core/Renderer/VulkanViewport.h"
#include "Application/Application.h"
#include "Platform/BaseWindow.h"
#include "Scene/World/World.h"

#include "Core/ImGui/ImGuiManager.h"

using RHI::VulkanRenderTarget;
using RHI::VulkanRenderTargetDeferred;

// *************************** RenderTarget ********************************
VulkanRenderTargetDeferred::VulkanRenderTargetDeferred()
{
}
VulkanRenderTargetDeferred::~VulkanRenderTargetDeferred()
{
    Destroy();
}

void VulkanRenderTargetDeferred::Create()
{
    Super::Create();
    CreateQuadViewport();
}
void VulkanRenderTargetDeferred::Destroy()
{
    DestroyQuadViewport();

    for (auto& ColorAttachmentTexture : GBufferAAttachmentTextures)
    {
        ColorAttachmentTexture->Destroy();
    }
    GBufferAAttachmentTextures.clear();

    for (auto& NormalAttachmentTexture : GBufferBAttachmentTextures)
    {
        NormalAttachmentTexture->Destroy();
    }
    GBufferBAttachmentTextures.clear();

    for (auto& PositionAttachmentTexture : GBufferCAttachmentTextures)
    {
        PositionAttachmentTexture->Destroy();
    }
    GBufferCAttachmentTextures.clear();

    Super::Destroy();
}

void VulkanRenderTargetDeferred::Update(const std::shared_ptr<VulkanCommandBuffer>& TargetCommandBuffer)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();

    Begin(TargetCommandBuffer);
    {
        UINT32 BackBufferIndex = RHI->GetVulkanSwapChain()->GetCurrentBackBufferFrameIndex();

        // Subpass 1
        {
            // todo : Shadow 추가
            Core::GEngine->GetWorld()->RenderGameObjects(ERenderPassType::Main);
        }

        // Subpass 2
        if (NextSubpass() == true)
        {
            QuadViewport->WriteGBuffer("GBufferA", GBufferAAttachmentTextures[BackBufferIndex]);
            QuadViewport->WriteGBuffer("GBufferB", GBufferBAttachmentTextures[BackBufferIndex]);
            QuadViewport->WriteGBuffer("GBufferC", GBufferCAttachmentTextures[BackBufferIndex]);
            QuadViewport->WriteGBuffer("GBufferD", RHI->GetVulkanSwapChain()->GetDepthStencilTexture(BackBufferIndex));
            QuadViewport->Update(0.0f);
        }

        // ImGui
        IMGUI_MANAGER->Render();
    }
    End();
}

void VulkanRenderTargetDeferred::CreateAttachmentTexture()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    UINT32 RenderWidth = 0, RenderHeight = 0;
    APPLICATION->GetWindow()->GetExtend(RenderWidth, RenderHeight);

    UINT32 BackBufferCount = RHI->GetVulkanSwapChain()->GetBackBufferCount();
    VkFormat BackBufferFormat = RHI->GetVulkanSwapChain()->GetSurfaceFormat().format;

    for (UINT32 i = 0; i < BackBufferCount; i++)
    {
        // GBufferA, ColorAttachmentTexture
        {
            std::shared_ptr<VulkanTexture> ColorAttachmentTexture = std::make_shared<VulkanTexture>();
            ColorAttachmentTexture->CreateImage(RenderWidth,
                                                RenderHeight,
                                                1,
                                                1,
                                                BackBufferFormat,
                                                VK_IMAGE_TILING_OPTIMAL,
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                VK_IMAGE_ASPECT_COLOR_BIT,
                                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            GBufferAAttachmentTextures.emplace_back(ColorAttachmentTexture);
        }

        // GBufferB, NormalAttachmentTexture
        {
            std::shared_ptr<VulkanTexture> NormalAttachmentTexture = std::make_shared<VulkanTexture>();
            NormalAttachmentTexture->CreateImage(RenderWidth,
                                                 RenderHeight,
                                                 1,
                                                 1,
                                                 BackBufferFormat,
                                                 VK_IMAGE_TILING_OPTIMAL,
                                                 VK_IMAGE_VIEW_TYPE_2D,
                                                 VK_IMAGE_ASPECT_COLOR_BIT,
                                                 VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            GBufferBAttachmentTextures.emplace_back(NormalAttachmentTexture);
        }

        // GBufferC, PositionAttachmentTexture
        {
            std::shared_ptr<VulkanTexture> PositionAttachmentTexture = std::make_shared<VulkanTexture>();
            PositionAttachmentTexture->CreateImage(RenderWidth,
                                                   RenderHeight,
                                                   1,
                                                   1,
                                                   BackBufferFormat,
                                                   VK_IMAGE_TILING_OPTIMAL,
                                                   VK_IMAGE_VIEW_TYPE_2D,
                                                   VK_IMAGE_ASPECT_COLOR_BIT,
                                                   VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            GBufferCAttachmentTextures.emplace_back(PositionAttachmentTexture);
        }
    }
}
void VulkanRenderTargetDeferred::CreateRenderPass()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    std::vector<VkAttachmentDescription> AttachmentDescriptions;
    VkAttachmentReference SwapChainAttachmentReference;
    VkAttachmentReference DepthStencilAttachmentReference;
    std::vector<VkAttachmentReference> ColorAttachmentReferences;
    std::vector<VkAttachmentReference> InputAttachmentReferences;
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
            VkAttachmentDescription GBufferAAttachmentDescription = {}; // ColorAttachmentDescription
            {
                GBufferAAttachmentDescription.format = GBufferAAttachmentTextures[0]->GetFormat();
                GBufferAAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                GBufferAAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                GBufferAAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                GBufferAAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                GBufferAAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                GBufferAAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                GBufferAAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            VkAttachmentDescription GBufferBAttachmentDescription = {}; // NormalAttachmentDescription
            {
                GBufferBAttachmentDescription.format = GBufferBAttachmentTextures[0]->GetFormat();
                GBufferBAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                GBufferBAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                GBufferBAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                GBufferBAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                GBufferBAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                GBufferBAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                GBufferBAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            VkAttachmentDescription GBufferCAttachmentDescription = {}; // PositionAttachmentDescription
            {
                GBufferCAttachmentDescription.format = GBufferCAttachmentTextures[0]->GetFormat();
                GBufferCAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                GBufferCAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                GBufferCAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                GBufferCAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                GBufferCAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                GBufferCAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                GBufferCAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            VkAttachmentDescription DepthStencilAttachmentDescription = {};
            {
                DepthStencilAttachmentDescription.format = RHI->GetVulkanSwapChain()->GetDepthStencilFormat();
                DepthStencilAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                DepthStencilAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                DepthStencilAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                DepthStencilAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                DepthStencilAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
                DepthStencilAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                DepthStencilAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }

            AttachmentDescriptions.emplace_back(SwapChainAttachmentDescription);
            AttachmentDescriptions.emplace_back(GBufferAAttachmentDescription);
            AttachmentDescriptions.emplace_back(GBufferBAttachmentDescription);
            AttachmentDescriptions.emplace_back(GBufferCAttachmentDescription);
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
                DepthStencilAttachmentReference.attachment = 4;
                DepthStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }

            ColorAttachmentReferences.clear();
            {
                VkAttachmentReference ColorAttachment_GBufferA = {};
                {
                    ColorAttachment_GBufferA.attachment = 1;
                    ColorAttachment_GBufferA.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                }
                VkAttachmentReference ColorAttachment_GBufferB = {};
                {
                    ColorAttachment_GBufferB.attachment = 2;
                    ColorAttachment_GBufferB.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                }
                VkAttachmentReference ColorAttachment_GBufferC = {};
                {
                    ColorAttachment_GBufferC.attachment = 3;
                    ColorAttachment_GBufferC.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                }

                ColorAttachmentReferences.emplace_back(ColorAttachment_GBufferA);
                ColorAttachmentReferences.emplace_back(ColorAttachment_GBufferB);
                ColorAttachmentReferences.emplace_back(ColorAttachment_GBufferC);
            }

            InputAttachmentReferences.clear();
            {
                VkAttachmentReference InputAttachment_Color = {};
                {
                    InputAttachment_Color.attachment = 1;
                    InputAttachment_Color.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                }
                VkAttachmentReference InputAttachment_Normal = {};
                {
                    InputAttachment_Normal.attachment = 2;
                    InputAttachment_Normal.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                }
                VkAttachmentReference InputAttachment_Position = {};
                {
                    InputAttachment_Position.attachment = 3;
                    InputAttachment_Position.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                }
                VkAttachmentReference InputAttachment_DepthStencil = {};
                {
                    InputAttachment_DepthStencil.attachment = 4;
                    InputAttachment_DepthStencil.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                }

                InputAttachmentReferences.emplace_back(InputAttachment_Color);
                InputAttachmentReferences.emplace_back(InputAttachment_Normal);
                InputAttachmentReferences.emplace_back(InputAttachment_Position);
                InputAttachmentReferences.emplace_back(InputAttachment_DepthStencil);
            }
        }

        // ClearValue
        ClearValues.clear();
        {
            VkClearValue SwapChainClearValue = {};
            {
                SwapChainClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            }
            VkClearValue GBufferAColorClearValue = {};
            {
                GBufferAColorClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            }
            VkClearValue GBufferBColorClearValue = {};
            {
                GBufferBColorClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            }
            VkClearValue GBufferCColorClearValue = {};
            {
                GBufferCColorClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            }
            VkClearValue DepthStencilClearValue = {};
            {
                DepthStencilClearValue.depthStencil = {1.0f, 0};
            }

            ClearValues.emplace_back(SwapChainClearValue);
            ClearValues.emplace_back(GBufferAColorClearValue);
            ClearValues.emplace_back(GBufferBColorClearValue);
            ClearValues.emplace_back(GBufferCColorClearValue);
            ClearValues.emplace_back(DepthStencilClearValue);
        }
    }

    std::vector<VkSubpassDescription> SubpassDescriptions;
    {
        VkSubpassDescription ColorSubpassDescription = {};
        {
            ColorSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            ColorSubpassDescription.colorAttachmentCount = static_cast<UINT32>(ColorAttachmentReferences.size());
            ColorSubpassDescription.pColorAttachments = ColorAttachmentReferences.data();
            ColorSubpassDescription.pDepthStencilAttachment = &DepthStencilAttachmentReference;
        }
        VkSubpassDescription SwapChainSubpassDescription = {};
        {
            SwapChainSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            SwapChainSubpassDescription.colorAttachmentCount = 1;
            SwapChainSubpassDescription.pColorAttachments = &SwapChainAttachmentReference;
            SwapChainSubpassDescription.inputAttachmentCount = static_cast<UINT32>(InputAttachmentReferences.size());
            SwapChainSubpassDescription.pInputAttachments = InputAttachmentReferences.data();
        }

        SubpassDescriptions.emplace_back(ColorSubpassDescription);
        SubpassDescriptions.emplace_back(SwapChainSubpassDescription);
    }

    std::vector<VkSubpassDependency> SubpassDependencies;
    {
        VkSubpassDependency Dependency1 = {};
        {
            Dependency1.srcSubpass = VK_SUBPASS_EXTERNAL;
            Dependency1.dstSubpass = 0;
            Dependency1.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            Dependency1.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            Dependency1.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            Dependency1.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            Dependency1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        }
        VkSubpassDependency Dependency2 = {};
        {
            Dependency2.srcSubpass = 0;
            Dependency2.dstSubpass = 1;
            Dependency2.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            Dependency2.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            Dependency2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            Dependency2.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            Dependency2.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        }
        VkSubpassDependency Dependency3 = {};
        {
            Dependency3.srcSubpass = 1;
            Dependency3.dstSubpass = VK_SUBPASS_EXTERNAL;
            Dependency3.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            Dependency3.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            Dependency3.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            Dependency3.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            Dependency3.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        }

        SubpassDependencies.emplace_back(Dependency1);
        SubpassDependencies.emplace_back(Dependency2);
        SubpassDependencies.emplace_back(Dependency3);
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
void VulkanRenderTargetDeferred::CreateFrameBuffer()
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
            std::array<VkImageView, 5> Attachments = {
                RHI->GetVulkanSwapChain()->GetBackBufferTexture(i)->GetImageView(),
                GBufferAAttachmentTextures[i]->GetImageView(),
                GBufferBAttachmentTextures[i]->GetImageView(),
                GBufferCAttachmentTextures[i]->GetImageView(),
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

void VulkanRenderTargetDeferred::CreateQuadViewport()
{
    QuadViewport.reset(new Core::VulkanViewport());
    QuadViewport->Initialize();

    VulkanRHI* RHI = Core::GEngine->GetRHI();
}
void VulkanRenderTargetDeferred::DestroyQuadViewport()
{
    if (QuadViewport != nullptr)
    {
        QuadViewport->Destroy();
        QuadViewport.reset();
        QuadViewport = nullptr;
    }
}
