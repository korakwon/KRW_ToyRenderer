#include "InputManager.h"
#include "Platform/BaseWindow.h"

using Core::InputManager;

IMPLEMENT_SINGLETON(InputManager)

InputManager::InputManager()
{
}
InputManager::~InputManager()
{
    Destroy();
}

void InputManager::Initialize()
{
    Mouse.Initialize();
    Keyboard.Initialize();
}
void InputManager::Update(const float& DeltaTime)
{
    // Only Focus in Renderer
    if(GetActiveWindow() == Platform::G_HWND)
    {
        Mouse.Update(DeltaTime);
        Keyboard.Update(DeltaTime);
    }
    else
    {
        Mouse.ClearState();
        Keyboard.ClearState();
    }
}
void InputManager::Destroy()
{
    Mouse.Destroy();
    Keyboard.Destroy();

    DESTROY_SINGLETON()
}

Core::InputMouse& InputManager::GetMouse()
{
    return Mouse;
}
Core::InputKeyboard& InputManager::GetKeyboard()
{
    return Keyboard;
}

LRESULT InputManager::MouseWheelWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_MOUSEWHEEL:
        {
            float WheelDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA); 
            Mouse.OnWheelEvent(WheelDelta);
            return 0;
        }
    default:
        break;
    }

    return 0;
}
