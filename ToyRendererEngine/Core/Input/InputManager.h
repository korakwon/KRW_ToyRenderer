#pragma once
#include "pch.h"
#include "Mouse/InputMouse.h"
#include "Keyboard/InputKeyboard.h"

// ****************************** Define *****************************
#define INPUT_MANAGER Core::InputManager::GetInstance()

// ****************************** Croe *******************************
namespace Core
{
    class InputManager : public ICoreExecute
    {
        DECLARE_SINGLETON(InputManager)
        
    public:
        virtual ~InputManager() final;
        
    public:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;
        
    public:
        InputMouse& GetMouse();
        InputKeyboard& GetKeyboard();

    public:
        LRESULT CALLBACK MouseWheelWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

    private:
        InputMouse Mouse;
        InputKeyboard Keyboard;
    };
}