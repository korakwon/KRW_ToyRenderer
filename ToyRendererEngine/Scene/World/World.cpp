#include "World.h"
#include "Scene/GameObject.h"

using Scene::World;

// ***************************** Scene ******************************
World::~World()
{
    Destroy();
}

void World::Initialize()
{
    for(const std::shared_ptr<GameObject>& Object : InitializeGameObjects)
    {
        Object->Initialize();
        
        UpdateGameObjects.emplace_back(Object);
    }
    
    InitializeGameObjects.clear();
}
void World::Update(const float& DeltaTime)
{
    for(const std::shared_ptr<GameObject>& Object : UpdateGameObjects)
    {
        Object->Update(DeltaTime);
    }
}
void World::Render()
{
    ClearCollectedRenderGameObjects();
    
    for(const std::shared_ptr<GameObject>& Object : UpdateGameObjects)
    {
        for(UINT i = 0; i < RENDER_PASS_TYPE_NUM; i++)
        {
            ERenderPassType Type = static_cast<ERenderPassType>(1 << i);
            
            if(Object->IsRenderTarget(Type) == true)
            {
                RenderCollectedGameObjects[Type].emplace_back(Object);
            }
        }
    }
}
void World::Destroy()
{
    ClearCollectedRenderGameObjects();
    
    for(const std::shared_ptr<GameObject>& Object : UpdateGameObjects)
    {
        Object->Destroy();
    }
    UpdateGameObjects.clear();
    
    for(const std::shared_ptr<GameObject>& Object : InitializeGameObjects)
    {
        Object->Destroy();
    }
    InitializeGameObjects.clear();
}

void World::AddGameObject(const std::shared_ptr<GameObject>& GameObject)
{
    InitializeGameObjects.emplace_back(GameObject);
}
void World::SetCameraGameObject(const std::weak_ptr<Scene::CameraGameObject>& GameObject)
{
    if(GameObject.expired() == true)
    {
        return;
    }
    
    CameraGameObject = GameObject;
}
std::weak_ptr<Scene::CameraGameObject> World::GetCameraGameObject()
{
    return CameraGameObject;
}

void World::RenderGameObjects(const ERenderPassType& Type)
{
    if (RenderCollectedGameObjects.find(Type) == RenderCollectedGameObjects.end())
    {
        return;
    }
    
    for(const std::shared_ptr<GameObject>& Object : RenderCollectedGameObjects[Type])
    {
        Object->Render();
    }
}

void World::ClearCollectedRenderGameObjects()
{
    for(auto& CollectedGameObjects : RenderCollectedGameObjects)
    {
        CollectedGameObjects.second.clear();
    }
    RenderCollectedGameObjects.clear();
}
