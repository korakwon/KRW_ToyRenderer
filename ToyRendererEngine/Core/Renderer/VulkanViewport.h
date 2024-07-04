#pragma once
#include "pch.h"

// ********************** Forward Declarations **********************
namespace Core
{
    class VulkanRenderer;
}
namespace RHI
{
    class VulkanStaticMesh;
    class VulkanMaterial;
    class VulkanTexture;
}

// ***************************** Core *******************************
namespace Core
{
    class VulkanViewport : public ICoreExecute
    {
        DISABLE_COPY_CLASS(VulkanViewport)

    public:
        VulkanViewport();
        virtual ~VulkanViewport() final;

    public:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;

    public:
        void WriteGBuffer(const std::string& BufferName, const std::shared_ptr<RHI::VulkanTexture>& Texture);

    private:
        void CreateMaterial();
        void DestroyMaterial();
        
    private:
        void CreateStaticMeshPlane();
        void DestroyStaticMeshPlane();
        
    private:
        std::shared_ptr<RHI::VulkanMaterial> Material;
        std::shared_ptr<RHI::VulkanStaticMesh> StaticMeshPlane;
    };
}
