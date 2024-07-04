#pragma once
#include "pch.h"

// ********************** Forward Declarations **********************
namespace App
{
    class Application;
}

namespace RHI
{
    class VulkanRHI;
}

namespace Core
{
    class VulkanRenderer;
}

namespace Scene
{
    class World;
}

// ****************************** App *******************************
namespace Core
{
    class Engine : public ICoreExecute
    {
        DISABLE_COPY_CLASS(Engine)

    public:
        Engine();
        virtual ~Engine() final;

    public:
        RHI::VulkanRHI* GetRHI() const;

    public:
        const std::shared_ptr<Scene::World>& GetWorld() { return World; }

    private:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;

    private:
        void OnResize(const UINT32& Width, const UINT32& Height);

    private:
        void InitWorld(const std::shared_ptr<Scene::World>& TargetWorld);
        void DestroyWorld();

    private:
        void CreateVulkanRHI();
        void CreateVulkanRenderer();

    private:
        void DestroyVulkanRHI();
        void DestroyVulkanRenderer();

    private:
        std::shared_ptr<Scene::World> World;

    private:
        std::unique_ptr<RHI::VulkanRHI> RHI;
        std::unique_ptr<Core::VulkanRenderer> Renderer;

    private:
        friend App::Application;
    };

    // ************************* Global Value ***************************
    extern Engine* GEngine;
}
