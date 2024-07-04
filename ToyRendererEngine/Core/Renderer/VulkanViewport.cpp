#include "VulkanViewport.h"

#include "Util/Math.h"
#include "Util/VertexIndexLayout.h"
#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanShader.h"
#include "RHI/Vulkan/VulkanPipeline.h"
#include "RHI/Vulkan/VulkanPipelineInfo.h"
#include "RHI/Vulkan/VulkanMaterial.h"
#include "RHI/Vulkan/Texture/VulkanTexture.h"
#include "RHI/Vulkan/RenderTarget/VulkanRenderTargetDeferred.h"
#include "RHI/Vulkan/Buffer/VulkanStaticMesh.h"

using Core::VulkanViewport;

VulkanViewport::VulkanViewport()
{
}
VulkanViewport::~VulkanViewport()
{
    Destroy();
}

void VulkanViewport::Initialize()
{
    CreateMaterial();
    CreateStaticMeshPlane();
}
void VulkanViewport::Update(const float& DeltaTime)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();

    std::shared_ptr<RHI::VulkanCommandBuffer> CommandBuffer = RHI->GetActivateCommandBuffer();

    Material->BindFrame(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
    {
        StaticMeshPlane->BindFrame(CommandBuffer);

        Material->DrawFrame(CommandBuffer);
        StaticMeshPlane->DrawFrame(CommandBuffer);
    }
    Material->EndFrame();
}
void VulkanViewport::Destroy()
{
    DestroyMaterial();
    DestroyStaticMeshPlane();
}

void VulkanViewport::WriteGBuffer(const std::string& BufferName, const std::shared_ptr<RHI::VulkanTexture>& Texture)
{
    ASSERT_NOT_NULL(Texture);

    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();
    std::shared_ptr<RHI::VulkanRenderTarget> RenderTarget = RHI->GetRenderTarget();

    Material->WriteTexture(BufferName, Texture.get());
}

void VulkanViewport::CreateMaterial()
{
    static std::string vet = "../_Assets/_Shaders/_Common/Quad_Vertex.spv";
    static std::string fag = "../_Assets/_Shaders/_Common/Quad_Pixel.spv";

    std::shared_ptr<RHI::VulkanShader> Shader = RHI::CreateShader(vet.c_str(), fag.c_str());
    {
        Shader->AddDescriptorSetLayoutBinding("GBufferA", VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
        Shader->AddDescriptorSetLayoutBinding("GBufferB", VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
        Shader->AddDescriptorSetLayoutBinding("GBufferC", VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
        Shader->AddDescriptorSetLayoutBinding("GBufferD", VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    Shader->AllocateDescriptorSets();

    RHI::VulkanPipelineInfo PipelineInfo = RHI::VulkanPipelineInfoProxy<ERasterizationInfo_CullModeType::None,
                                                                        ERasterizationInfo_FrontFaceModeType::CCW,
                                                                        false,
                                                                        1>::Create();
    {
        PipelineInfo.SetVertexCreateInfo(VertexTexture::GetVkPipelineVertexInputStateCreateInfo());
        PipelineInfo.SetSubpassCount(1);
    }

    Material = std::make_shared<RHI::VulkanMaterial>(PipelineInfo, Shader);
}
void VulkanViewport::DestroyMaterial()
{
    if (Material != nullptr)
    {
        Material.reset();
        Material = nullptr;
    }
}

void VulkanViewport::CreateStaticMeshPlane()
{
    static std::vector<VertexTexture> Vertices =
    {
        {{-1.0f, +1.0f, +0.0f}, {+0.0f, +0.0f}},
        {{+1.0f, +1.0f, +0.0f}, {+1.0f, +0.0f}},
        {{+1.0f, -1.0f, +0.0f}, {+1.0f, +1.0f}},
        {{-1.0f, -1.0f, +0.0f}, {+0.0f, +1.0f}},
    };

    std::vector<INDEX32> Indices =
    {
        0, 1, 2, 0, 2, 3
    };

    StaticMeshPlane = std::make_shared<RHI::VulkanStaticMesh>(
        Vertices.data(), static_cast<UINT32>(Vertices.size()), static_cast<UINT32>(sizeof(VertexTexture)),
        Indices.data(), static_cast<UINT32>(Indices.size()), static_cast<UINT32>(sizeof(INDEX32)));
}
void VulkanViewport::DestroyStaticMeshPlane()
{
    if (StaticMeshPlane != nullptr)
    {
        StaticMeshPlane.reset();
        StaticMeshPlane = nullptr;
    }
}
