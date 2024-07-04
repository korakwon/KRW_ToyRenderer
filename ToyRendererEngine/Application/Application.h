#pragma once
#include "pch.h"

// ****************************** Define ****************************
#define APPLICATION App::Application::GetInstance()

// ********************** Forward Declarations **********************
namespace Platform
{
    class BaseWindow;
}
namespace Scene
{
    class World;
}

// ****************************** App *******************************
namespace App
{
    class Application : public ICoreExecute
    {
        DECLARE_SINGLETON(Application)
        
    public:
        virtual ~Application() override;

    public:
        void Create(std::wstring AppTitle, const int& AppWidth, const int& AppHeight);
        void InitWorld(const std::shared_ptr<Scene::World>& World);
        void Run();

    public:
        void OnResize(const UINT32& Width, const UINT32& Height);

    public:
        Platform::BaseWindow* GetWindow() const { return Window.get(); }

    public:
        static void OnRequestExit()
        {
            IS_REQUEST_EXIT = true;
        }
        
    private:
        virtual void Initialize() override;
        virtual void Update(const float& DeltaTime) override;
        virtual void Destroy() override;

    private:
        std::shared_ptr<Platform::BaseWindow> Window = nullptr;

    private:
        bool IsCreatedApplication = false;
        bool IsInitializedApplication = false;
        
    private:
        static bool IS_REQUEST_EXIT;
    };

    LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
}
