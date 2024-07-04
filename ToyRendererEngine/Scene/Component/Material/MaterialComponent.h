#pragma once
#include "pch.h"
#include "Scene/Component/Component.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanMaterial;
    class VulkanUniformBuffer;
    class VulkanTexture;
}

// ***************************** Scene ******************************
namespace Scene
{
    class MaterialComponent : public Component
    {
        DISABLE_COPY_CLASS(MaterialComponent)

    public:
        MaterialComponent() = delete;
        MaterialComponent(const std::string& Name);
        virtual ~MaterialComponent() override = default;

    public:
        virtual void Destroy() override;

    public:
        void AddUniformBuffer(const std::string& Name, UINT32 BufferSize);
        void UpdateUniformBuffer(const std::string& Name, const void* Buffer, const UINT32& BufferSize);
        
    public:
        void AddTexture(const std::string& Name, const std::string& TexturePath, const UINT32& MipLevel = 1, const ETextureFormatType& Format = ETextureFormatType::R8G8B8A8);
        void AddCubeTexture(const std::string& Name, const std::array<std::string, 6>& TexturePath, const UINT32& MipLevel = 1, const ETextureFormatType& Format = ETextureFormatType::R8G8B8A8);
        
    public:
        const std::shared_ptr<RHI::VulkanMaterial>& GetMaterial();
        void SetMaterial(const std::shared_ptr<RHI::VulkanMaterial>& TargetMaterial);

    protected:
        std::unordered_map<std::string, std::shared_ptr<RHI::VulkanUniformBuffer>> UniformBuffers;

    protected:
        std::unordered_map<std::string, std::shared_ptr<RHI::VulkanTexture>> TextureBuffers;
        
    protected:
        std::shared_ptr<RHI::VulkanMaterial> Material;
    };
}
