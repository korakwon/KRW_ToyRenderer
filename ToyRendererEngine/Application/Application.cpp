#include "Application.h"

#include "Platform/VulkanWindow/VulkanWindow.h"

#include "Core/Engine.h"
#include "Core/ImGui/ImGuiManager.h"
#include "Core/Time/TimeManager.h"
#include "Core/Input/InputManager.h"

using App::Application;

// ****************************** App *******************************
bool Application::IS_REQUEST_EXIT = false;

IMPLEMENT_SINGLETON(Application)

Application::Application()
{
}
Application::~Application()
{
    Destroy();
}
void Application::Create(std::wstring AppTitle, const int& AppWidth, const int& AppHeight)
{
    // Window
    {
        Window.reset(new Platform::VulkanWindow(AppTitle, AppWidth, AppHeight));
        ASSERT_NOT_NULL(Window);
        Window->Initialize();
    }

    // Engine
    {
        Core::GEngine = new Core::Engine();     
        ASSERT_NOT_NULL(Core::GEngine);
        Core::GEngine->Initialize();
    }

    IsCreatedApplication = true;
}
void Application::InitWorld(const std::shared_ptr<Scene::World>& World)
{
    Core::GEngine->InitWorld(World);
}
void Application::Run()
{
    ASSERT(IsCreatedApplication);
    
    static float DeltaTime = 0.0f;

    Initialize();

    while (IS_REQUEST_EXIT == false)
    {
        // Time Manager
        TIME_MANAGER->Update(DeltaTime);
        DeltaTime = TIME_MANAGER->GetDeltaTime();

        Update(DeltaTime);
    }

    Destroy();
}

void Application::OnResize(const UINT32& Width, const UINT32& Height)
{
    if(IsInitializedApplication == true)
    {
        Window->OnResize(Width, Height);
        Core::GEngine->OnResize(Width, Height);
    }
}

void Application::Initialize()
{
    ASSERT(IsCreatedApplication);
    
    // Time Manager
    TIME_MANAGER->Initialize();

    // Input Manager
    INPUT_MANAGER->Initialize();
    
    IsInitializedApplication = true;
}
void Application::Update(const float& DeltaTime)
{
    assert(IsCreatedApplication);
    assert(IsInitializedApplication);
    
    // Window
    {
        ASSERT_NOT_NULL(Window);
        Window->Update(DeltaTime);
    }

    // Input Manager
    INPUT_MANAGER->Update(DeltaTime);

    // Engine
    {
        ASSERT_NOT_NULL(Core::GEngine);
        Core::GEngine->Update(DeltaTime);
    }
}
void Application::Destroy()
{
    // Input Manager
    INPUT_MANAGER->Destroy();

    // Time Manager
    TIME_MANAGER->Destroy();

    // Engine
    if (Core::GEngine != nullptr)
    {
        Core::GEngine->Destroy();
        Core::GEngine = nullptr;
    }

    // Window
    if (Window != nullptr)
    {
        Window->Destroy();
        Window.reset();
        Window = nullptr;
    }
}

LRESULT App::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    if (IMGUI_MANAGER->WndProc(hWnd, iMessage, wParam, lParam))
    {
        return true;
    }

    switch (iMessage)
    {
    case WM_CREATE:
        {
            return 0;
        }
    case WM_PAINT:
        {
            return 0;
        }
    case WM_SIZE:
        {
            APPLICATION->OnResize(static_cast<UINT32>(LOWORD(lParam)), static_cast<UINT32>(HIWORD(lParam)));
            return 0;
        }
    case WM_CLOSE:
        {
            Application::OnRequestExit();
            return 0;
        }
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    case WM_MOUSEWHEEL:
        {
            return INPUT_MANAGER->MouseWheelWndProc(hWnd, iMessage, wParam, lParam);
        }
    default:
        break;
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
