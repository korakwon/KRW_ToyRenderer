#include "GameObject.h"

#include "Component/Component.h"
#include "Component/Transform/TransformComponent.h"

using Scene::GameObject;
//using Scene::Component;

// ***************************** Scene ******************************
GameObject::GameObject()
{
    TransformComponent = std::make_shared<Scene::TransformComponent>();
    TransformComponent->ParentGameObject = this;
}
GameObject::~GameObject()
{
    Destroy();
}

void GameObject::Initialize()
{
    Super::Initialize();
    
    if(TransformComponent != nullptr)
    {
        TransformComponent->Initialize();
    }

    for(auto& Component : Components)
    {
        Component.second->Initialize();
    }
}
void GameObject::Update(const float& DeltaTime)
{
    Super::Update(DeltaTime);
    
    if(TransformComponent != nullptr)
    {
        TransformComponent->Update(DeltaTime);
    }

    for(auto& Component : Components)
    {
        Component.second->Update(DeltaTime);
    }
}
void GameObject::Render()
{
    Super::Render();
    
    if(TransformComponent != nullptr)
    {
        TransformComponent->Render();
    }

    for(auto& Component : Components)
    {
        Component.second->Render();
    }
}
void GameObject::Destroy()
{
    if(TransformComponent != nullptr)
    {
        TransformComponent->Destroy();
        TransformComponent = nullptr;
    }

    for(auto& Component : Components)
    {
        Component.second->Destroy();
    }
    Components.clear();
    
    Super::Destroy();
}

Scene::TransformComponent* GameObject::GetTransformComponent()
{
    if(TransformComponent != nullptr)
    {
       return TransformComponent.get();
    }
    return nullptr;
}
Scene::Component* GameObject::GetComponent(const std::string& ComponentName)
{
    Scene::Component* ResultComponent = nullptr;

    if (Components.find(ComponentName) == Components.end())
    {
        // ASSERT(false);
    }
    else
    {
        ResultComponent = Components[ComponentName].get();
    }

    return ResultComponent;
}

bool GameObject::AddComponent(const std::shared_ptr<Scene::Component>& Component)
{
    bool Result = false;
    if(Component == nullptr)
    {
        return Result;
    }

    std::string ComponentName = Component->GetName();
    if(ComponentName.empty())
    {
        return Result;
    }
    
    if (Components.find(ComponentName) == Components.end())
    {
        Components[ComponentName] = Component;
        Component->ParentGameObject = this;

        Result = true;
    }
    
    return Result;
}

bool GameObject::IsRenderTarget(const ERenderPassType& Type)
{
    return (static_cast<UINT32>(RenderPassType) & static_cast<UINT32>(Type)) == static_cast<UINT32>(Type);
}
