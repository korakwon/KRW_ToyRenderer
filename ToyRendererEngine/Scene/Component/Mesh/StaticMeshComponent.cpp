#include "StaticMeshComponent.h"

#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanMaterial.h"
#include "RHI/Vulkan/Buffer/VulkanStaticMesh.h"

#include "Scene/GameObject.h"
#include "Scene/Component/Material/MaterialComponent.h"

using Scene::StaticMeshComponent;

StaticMeshComponent::StaticMeshComponent(const std::string& Name)
    : Component(Name)
{
}
void StaticMeshComponent::Initialize()
{
    Super::Initialize();
}
void StaticMeshComponent::Update(const float& DeltaTime)
{
    Super::Update(DeltaTime);
}
void StaticMeshComponent::Render()
{
    Super::Render();
    
    Scene::MaterialComponent* MaterialComponent = static_cast<Scene::MaterialComponent*>(ParentGameObject->GetComponent(TargetMaterialName));
    if(MaterialComponent != nullptr)
    {
        RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();
        std::shared_ptr<RHI::VulkanCommandBuffer> CommandBuffer = RHI->GetActivateCommandBuffer();

        std::shared_ptr<RHI::VulkanMaterial> Material = MaterialComponent->GetMaterial();

        if(Material != nullptr)
        {
            Material->BindFrame(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
            {
                StaticMesh->BindFrame(CommandBuffer);
            
                Material->DrawFrame(CommandBuffer);
                StaticMesh->DrawFrame(CommandBuffer);
            }
            Material->EndFrame();
        }
    }
}
void StaticMeshComponent::Destroy()
{
    Super::Destroy();
}

void StaticMeshComponent::SetStaticMesh(const std::shared_ptr<RHI::VulkanStaticMesh>& TargetStaticMesh)
{
    if (TargetStaticMesh != nullptr)
    {
        StaticMesh = TargetStaticMesh;
    }
}
