#pragma once
#include "pch.h"
#include "Util/Math.h"

// ********************** Forward Declarations **********************
namespace Scene
{
    class Component;
    class StaticMesh;
    class TransformComponent;
}
namespace RHI
{
    class VulkanPipeline;
    class VulkanUniformBuffer;
    class VulkanTexture;
    class VulkanMaterial;
}

// ***************************** Scene ******************************
namespace Scene
{
    class GameObject : public IObjectExecute
    {
    public:
        GameObject();
        virtual ~GameObject() override;

    public:
        virtual void Initialize() override;
        virtual void Update(const float& DeltaTime) override;
        virtual void Render() override;
        virtual void Destroy() override;

    public:
        TransformComponent* GetTransformComponent();
        Scene::Component* GetComponent(const std::string& ComponentName);

    public:
        bool AddComponent(const std::shared_ptr<Scene::Component>& Component);
        
    public:
        bool IsRenderTarget(const ERenderPassType& Type);

    protected:
        std::shared_ptr<TransformComponent> TransformComponent;
        std::unordered_map<std::string, std::shared_ptr<Scene::Component>> Components;
        
    protected:
        ERenderPassType RenderPassType = ERenderPassType::Shadow | ERenderPassType::Main;
    };
}