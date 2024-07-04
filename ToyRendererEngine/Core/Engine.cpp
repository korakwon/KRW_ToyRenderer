#include "Engine.h"
#include "Application/Application.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "Core/Renderer/VulkanRenderer.h"
#include "Core/ImGui/ImGuiManager.h"
#include "Scene/World/World.h"

using Core::Engine;

// ************************* Global Value ***************************
Engine* Core::GEngine = nullptr;

// ***************************** Engine *****************************
Engine::Engine()
{
}
Engine::~Engine()
{
    Destroy();
}

RHI::VulkanRHI* Engine::GetRHI() const
{
    ASSERT_NOT_NULL(RHI);
    return RHI.get();
}

void Engine::Initialize()
{
    CreateVulkanRHI();
    CreateVulkanRenderer();
    IMGUI_MANAGER->Initialize();
}
void Engine::Update(const float& DeltaTime)
{
    // Init World
    {
        if(World != nullptr)
        {
            World->Initialize();
        }
    }
    
    // Update
    {
        ASSERT_NOT_NULL(RHI);
        RHI->Update(DeltaTime);

        if(World != nullptr)
        {
            IMGUI_MANAGER->StartFrame();

            World->Update(DeltaTime);
            
            IMGUI_MANAGER->EndFrame();
            IMGUI_MANAGER->Update(DeltaTime);
        }
    }

    // Rendering
    {
        if(World != nullptr)
        {
            World->Render();
        }
        
        ASSERT_NOT_NULL(Renderer);
        Renderer->Update(DeltaTime);
    }
}
void Engine::Destroy()
{
    // Wait RHI Destroy - Device & Event 
    if(RHI != nullptr)
    {
        RHI->WaitDestroy();
    }

    DestroyWorld();

    // ImGui Manager
    IMGUI_MANAGER->Destroy();
    
    DestroyVulkanRenderer();
    DestroyVulkanRHI();
}

void Engine::OnResize(const UINT32& Width, const UINT32& Height)
{
    ASSERT_NOT_NULL(RHI);

    RHI->OnResize(Width, Height);
    Renderer->OnResize(Width, Height);
    IMGUI_MANAGER->OnResize(Width, Height);
}

void Engine::InitWorld(const std::shared_ptr<Scene::World>& TargetWorld)
{
    DestroyWorld();
    
    World = TargetWorld;
}
void Engine::DestroyWorld()
{
    if(World != nullptr)
    {
        World->Destroy();
        World.reset();
        World = nullptr;
    }
}

void Engine::CreateVulkanRHI()
{
    RHI.reset(new RHI::VulkanRHI());
    ASSERT_NOT_NULL(RHI);
    RHI->Initialize();
}
void Engine::CreateVulkanRenderer()
{
    Renderer.reset(new Core::VulkanRenderer());
    ASSERT_NOT_NULL(Renderer);
    Renderer->Initialize();
}

void Engine::DestroyVulkanRHI()
{
    if(RHI != nullptr)
    {
        RHI->Destroy();
        RHI.reset();
        RHI = nullptr;
    }
}
void Engine::DestroyVulkanRenderer()
{
    if(Renderer != nullptr)
    {
        Renderer->Destroy();
        Renderer.reset();
        Renderer = nullptr;
    }
}
