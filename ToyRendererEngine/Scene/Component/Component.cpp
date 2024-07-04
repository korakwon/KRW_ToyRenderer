#include "Component.h"

using Scene::Component;

Component::Component(const std::string& ComponentName)
    : Name(ComponentName)
{
}

void Component::Initialize()
{
    Super::Initialize();
    
    if(ParentGameObject == nullptr)
    {
        ASSERT(false);    
    }
}
void Component::Update(const float& DeltaTime)
{
    Super::Update(DeltaTime);
    
    if(ParentGameObject == nullptr)
    {
        ASSERT(false);    
    }
}
void Component::Render()
{
    Super::Render();
    
    if(ParentGameObject == nullptr)
    {
        ASSERT(false);    
    }
}
void Component::Destroy()
{
    Super::Destroy();
    
    if(ParentGameObject == nullptr)
    {
        ASSERT(false);    
    }
}

