#include "MaterialComponent.h"
#include "RHI/Vulkan/VulkanMaterial.h"
#include "RHI/Vulkan/Buffer/VulkanUniformBuffer.h"
#include "RHI/Vulkan/Texture/VulkanTexture.h"

using Scene::MaterialComponent;

MaterialComponent::MaterialComponent(const std::string& Name)
    : Component(Name)
{
}

void MaterialComponent::Destroy()
{
    for(const auto& UniformBuffer : UniformBuffers)
    {
        UniformBuffer.second->Destroy();
    }
    UniformBuffers.clear();
    
    for(const auto& TextureBuffer : TextureBuffers)
    {
        TextureBuffer.second->Destroy();
    }
    TextureBuffers.clear();
    
    Super::Destroy();
}

void MaterialComponent::AddUniformBuffer(const std::string& Name, UINT32 BufferSize)
{
    ASSERT_NOT_NULL(Material);
    ASSERT(Name.empty() == false);
    ASSERT(BufferSize > 0);

    if (UniformBuffers.find(Name) == UniformBuffers.end())
    {
        UniformBuffers[Name] = RHI::CreateUniformBuffer(
            BufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        Material->WriteUniformBuffer(Name, UniformBuffers[Name].get());
    }
}
void MaterialComponent::UpdateUniformBuffer(const std::string& Name, const void* Buffer, const UINT32& BufferSize)
{
    ASSERT_NOT_NULL(Material);
    ASSERT_NOT_NULL(Buffer);
    ASSERT(BufferSize > 0);
    
    if (UniformBuffers.find(Name) != UniformBuffers.end())
    {
        UniformBuffers[Name]->UpdateBuffer(Buffer, BufferSize);
    }
}

void MaterialComponent::AddTexture(const std::string& Name, const std::string& TexturePath, const UINT32& MipLevel, const ETextureFormatType& Format)
{
    ASSERT_NOT_NULL(Material);
    ASSERT(Name.empty() == false);
    ASSERT(TexturePath.empty() == false);

    if (TextureBuffers.find(Name) == TextureBuffers.end())
    {
        TextureBuffers[Name] = RHI::Create2DTexture(TexturePath, MipLevel, Format);

        Material->WriteTexture(Name, TextureBuffers[Name].get());
    }
}
void MaterialComponent::AddCubeTexture(const std::string& Name, const std::array<std::string, 6>& TexturePaths, const UINT32& MipLevel, const ETextureFormatType& Format)
{
    ASSERT_NOT_NULL(Material);
    ASSERT(Name.empty() == false);
    ASSERT(TexturePaths.empty() == false);

    if (TextureBuffers.find(Name) == TextureBuffers.end())
    {
        TextureBuffers[Name] = RHI::CreateCubeMapTexture(TexturePaths, MipLevel, Format);

        Material->WriteTexture(Name, TextureBuffers[Name].get());
    }
}

const std::shared_ptr<RHI::VulkanMaterial>& MaterialComponent::GetMaterial()
{
    ASSERT_NOT_NULL(Material);

    return Material;
}
void MaterialComponent::SetMaterial(const std::shared_ptr<RHI::VulkanMaterial>& TargetMaterial)
{
    if (TargetMaterial != nullptr)
    {
        Material = TargetMaterial;
    }
}