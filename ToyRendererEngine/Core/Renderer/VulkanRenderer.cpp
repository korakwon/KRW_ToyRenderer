#include "VulkanRenderer.h"

#include "VulkanViewport.h"
#include "Core/Engine.h"
#include "Core/ImGui/ImGuiManager.h"
#include "Core/Renderer/VulkanViewport.h"

#include "RHI/Vulkan/VulkanCommon.h"
#include "RHI/Vulkan/VulkanMaterial.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanSwapChain.h"
#include "RHI/Vulkan/Buffer/VulkanCommandBuffer.h"
#include "RHI/Vulkan/RenderTarget/VulkanRenderTarget.h"
#include "RHI/Vulkan/RenderTarget/VulkanRenderTargetForward.h"
#include "RHI/Vulkan/RenderTarget/VulkanRenderTargetDeferred.h"

#include "Scene/World/World.h"

using Core::VulkanRenderer;
using RHI::VulkanRHI;
using RHI::VulkanRenderTarget;

// ***************************** Renderer *********************************
VulkanRenderer::VulkanRenderer()
{
}
VulkanRenderer::~VulkanRenderer()
{
    Destroy();
}

void VulkanRenderer::Initialize()
{
    CreateVulkanRenderTarget();
}
void VulkanRenderer::Update(const float& DeltaTime)
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    
    if(IsResize == true)
    {
        if(RHI->IsResized() == false)
        {
            Initialize();
            IsResize = false;
        }
        return;
    }
    
    std::shared_ptr<RHI::VulkanRenderTarget> RenderTarget = RHI->GetRenderTarget();
    
    RHI->BeginRenderFrame();
    {
        std::shared_ptr<RHI::VulkanCommandBuffer> CommandBuffer = RHI->GetActivateCommandBuffer();
        
        RenderTarget->Update(CommandBuffer);
    }
    RHI->EndRenderFrame();
}
void VulkanRenderer::Destroy()
{
}

void VulkanRenderer::OnResize(const UINT32& Width, const UINT32& Height)
{
    IsResize = true;
}

void VulkanRenderer::CreateVulkanRenderTarget()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();

    std::shared_ptr<VulkanRenderTarget> RenderTarget;

    // todo : RenderTarget Setting 추가하자
    //if(RenderTargetType == ERenderTargetType::ForwardRenderTarget)
    //{
    //    RenderTarget.reset(new RHI::VulkanRenderTargetForward());
    //}
    //else if(RenderTargetType == ERenderTargetType::DeferredRenderTarget)
    {
        RenderTarget.reset(new RHI::VulkanRenderTargetDeferred());
    }
    
    ASSERT_NOT_NULL(RenderTarget);
    RHI->SetRenderTarget(RenderTarget);
}
