#pragma once
#include "pch.h"
#include "Util/Math.h"

#define MAX_INPUT_MOUSE 8

// ********************** Forward Declarations **********************
namespace Core
{
    class InputManager;
}

// *************************** Enum Class ***************************
namespace Core
{
    enum class EMouseDirection
    {
        None = 0,
        Right = 1 << 0, Left = 1 << 1, Up = 1 << 2, Down = 1 << 3,
        RightUp = Right | Up, RightDown = Right | Down,
        LeftUp = Left | Up, LeftDown = Left | Down,
    };
}

// **************************** Core ********************************
namespace Core
{
    class InputMouse : public ICoreExecute
    {
        DECLARE_PRIVATE_CLASS(InputMouse)

    public:
        virtual ~InputMouse() final;
        
    public:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;

    public:
        Vector2 GetPosition() const { return MousePosition; }
        EMouseDirection GetMoveDirection() const { return MouseMoveDirection; }

    public:
        float GetWheelDelta() const { return MouseWheelDelta; }
        bool IsWheelMoving() const { return IsMouseWheelMoving; }
        
    public:
        bool IsButtonDown(const DWORD& Button);
        bool IsButtonUp(const DWORD& Button);
        bool IsButtonPress(const DWORD& Button);
        
    public:
        void ClearState();

    private:
        int GetMouseButtonIndex(const DWORD& Button);
        
    private:
        void OnWheelEvent(const float& Delta);
        
    private:
        Vector2 MousePosition;
        Vector2 PrevMousePosition;
        EMouseDirection MouseMoveDirection;

    private:
        float MouseWheelDelta;
        bool IsMouseWheelMoving;
        bool IsMouseWheelMovingReset;
        
    private:
        byte KeyState[MAX_INPUT_MOUSE];
        byte KeyOldState[MAX_INPUT_MOUSE];
        byte KeyMap[MAX_INPUT_MOUSE];

    private:
        friend Core::InputManager;
    };
}
