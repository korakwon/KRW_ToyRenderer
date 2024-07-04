#include "VulkanMaterial.h"
#include "VulkanPipeline.h"
#include "VulkanShader.h"
#include "Buffer/VulkanUniformBuffer.h"
#include "Texture/VulkanTexture.h"

#include "Application/Application.h"
#include "Platform/BaseWindow.h"

using RHI::VulkanMaterial;

VulkanMaterial::VulkanMaterial(VulkanPipelineInfo& PipelineInfo, const std::shared_ptr<VulkanShader>& TargetShader)
{
    ASSERT_NOT_NULL(TargetShader);
    
    Shader = TargetShader;

    Pipeline = std::make_shared<RHI::VulkanPipeline>();
    
    // todo : Pipeline Init은 다른곳에서 해야함. 여기서하면 초기값 무조건 고정으로 써야함
    Pipeline->Init(PipelineInfo, Shader);
}
VulkanMaterial::~VulkanMaterial()
{
    Destroy();
}

void VulkanMaterial::Destroy()
{
    if(Pipeline != nullptr)
    {
        Pipeline->Destroy();
        Pipeline.reset();
        Pipeline = nullptr;
    }
    
    if(Shader != nullptr)
    {
        Shader->Destroy();
        Shader.reset();
        Shader = nullptr;
    }
}

void VulkanMaterial::WriteUniformBuffer(const std::string& DescriptorSetLayoutName, VulkanUniformBuffer* UniformBuffer) const
{
    ASSERT_NOT_NULL(Shader);
    ASSERT_NOT_NULL(UniformBuffer);

    Shader->WriteBuffer(DescriptorSetLayoutName, UniformBuffer->GetDescriptorBufferInfo());
}
void VulkanMaterial::WriteTexture(const std::string& DescriptorSetLayoutName, VulkanTexture* Texture) const
{
    ASSERT_NOT_NULL(Shader);
    ASSERT_NOT_NULL(Texture);

    Shader->WriteTexture(DescriptorSetLayoutName, Texture->GetDescriptorImageInfo());
}

void VulkanMaterial::BindFrame(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, VkPipelineBindPoint PipelineBindPoint)
{
    ASSERT_NOT_NULL(Pipeline);
    ASSERT_NOT_NULL(Shader);
    ASSERT(IsActivate == false);
    
    IsActivate = true;
    
    UINT32 Width = 0, Height = 0;
    APPLICATION->GetWindow()->GetExtend(Width, Height);

    VkViewport ViewportInfo;
    {
        ViewportInfo.x = 0;
        ViewportInfo.y = 0;
        ViewportInfo.width = static_cast<float>(Width);
        ViewportInfo.height = static_cast<float>(Height);
        ViewportInfo.minDepth = 0.0f;
        ViewportInfo.maxDepth = 1.0f;
    }
    VkRect2D ScissorInfo;
    {
        ScissorInfo.offset.x = 0;
        ScissorInfo.offset.y = 0;
        ScissorInfo.extent.width = Width;
        ScissorInfo.extent.height = Height;
    }
    UpdateViewportInfo(ViewportInfo);
    UpdateScissorInfo(ScissorInfo);

    Pipeline->Bind(CommandBuffer, PipelineBindPoint);
    Shader->Bind(CommandBuffer, PipelineBindPoint, Pipeline.get());
}
void VulkanMaterial::DrawFrame(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) const
{
    Pipeline->Draw(CommandBuffer);
}
void VulkanMaterial::EndFrame()
{
    IsActivate = false;
}

void VulkanMaterial::UpdateViewportInfo(const VkViewport& ViewportInfo) const
{
    Pipeline->UpdateViewportInfo(ViewportInfo);
}
void VulkanMaterial::UpdateScissorInfo(const VkRect2D& ScissorInfo) const
{
    Pipeline->UpdateScissorInfo(ScissorInfo);
}
