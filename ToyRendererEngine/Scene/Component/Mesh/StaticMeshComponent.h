#pragma once
#include "pch.h"
#include "Scene/Component/Component.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanStaticMesh;
}

// ***************************** Scene ******************************
namespace Scene
{
    class StaticMeshComponent : public Component
    {
        DISABLE_COPY_CLASS(StaticMeshComponent)

    public:
        StaticMeshComponent() = delete;
        StaticMeshComponent(const std::string& Name);
        virtual ~StaticMeshComponent() override = default;

    public:
        virtual void Initialize() override;
        virtual void Update(const float& DeltaTime) override;
        virtual void Render() override;
        virtual void Destroy() override;

    public:
        void SetTargetMaterialName(const std::string& MaterialName)
        {
            TargetMaterialName = MaterialName;
        }
        
    public:
        void SetStaticMesh(const std::shared_ptr<RHI::VulkanStaticMesh>& TargetStaticMesh);

    private:
        std::string TargetMaterialName;
        
    private:
        std::shared_ptr<RHI::VulkanStaticMesh> StaticMesh;
    };
}
