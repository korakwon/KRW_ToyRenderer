#pragma once
#include "pch.h"

#define MAX_INPUT_KEY 255

// ********************** Forward Declarations **********************
namespace Core
{
    class InputManager;
}

// **************************** Core ********************************
namespace Core
{
    class InputKeyboard : public ICoreExecute
    {
        DECLARE_PRIVATE_CLASS(InputKeyboard)

    public:
        virtual ~InputKeyboard() final;
        
    public:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;
        
    public:
        bool IsKeyDown(const DWORD& Key) const { return KeyMap[Key] == static_cast<byte>(EKeyStateType::Down); }
        bool IsKeyUp(const DWORD& Key) const { return KeyMap[Key] == static_cast<byte>(EKeyStateType::Up); }
        bool IsKeyPress(const DWORD& Key) const { return KeyMap[Key] == static_cast<byte>(EKeyStateType::Press); }

    public:
        void ClearState();
        
    private:
        byte KeyState[MAX_INPUT_KEY];
        byte KeyOldState[MAX_INPUT_KEY];
        byte KeyMap[MAX_INPUT_KEY];
        
    private:
        friend Core::InputManager;
    };
}
