#include "InputMouse.h"
#include "Application/Application.h"
#include "Platform/BaseWindow.h"

using Core::InputMouse;

InputMouse::InputMouse()
{
	ClearState();
}
InputMouse::~InputMouse()
{
	Destroy();
}

void InputMouse::Initialize()
{
	ClearState();
}
void InputMouse::Update(const float& DeltaTime)
{
	// Position & Direction
	{
		// Position
		{
			UINT32 Width = 0, Height = 0;
			APPLICATION->GetWindow()->GetExtend(Width, Height);
			LONG LWidth = static_cast<LONG>(Width), LHeight = static_cast<LONG>(Height);
		
			POINT CurrentMousePosition;
			GetCursorPos(&CurrentMousePosition);
			ScreenToClient(Platform::G_HWND, &CurrentMousePosition);
			
			CurrentMousePosition.x = (CurrentMousePosition.x < 0) ? 0 : CurrentMousePosition.x;
			CurrentMousePosition.y = (CurrentMousePosition.y < 0) ? 0 : CurrentMousePosition.y;
		
			CurrentMousePosition.x = (CurrentMousePosition.x > LWidth) ? LWidth : CurrentMousePosition.x;
			CurrentMousePosition.y = (CurrentMousePosition.y > LHeight) ? LHeight : CurrentMousePosition.y;

			PrevMousePosition = MousePosition;
			MousePosition.x = static_cast<float>(CurrentMousePosition.x);
			MousePosition.y = static_cast<float>(CurrentMousePosition.y);
		}

		// Direction
		{
			static float MOUSE_DIRECTION_SENSITIVITY = 0.1f;
			
			Vector2 MouseDirection = MousePosition - PrevMousePosition;
			MouseMoveDirection = EMouseDirection::None;
		
			if(MouseDirection.x > MOUSE_DIRECTION_SENSITIVITY)
			{
				MouseMoveDirection = static_cast<EMouseDirection>(static_cast<int>(MouseMoveDirection) | static_cast<int>(EMouseDirection::Right));
			}
			else if(MouseDirection.x < -MOUSE_DIRECTION_SENSITIVITY)
			{
				MouseMoveDirection = static_cast<EMouseDirection>(static_cast<int>(MouseMoveDirection) | static_cast<int>(EMouseDirection::Left));
			}
		
			if(MouseDirection.y > MOUSE_DIRECTION_SENSITIVITY)
			{
				MouseMoveDirection = static_cast<EMouseDirection>(static_cast<int>(MouseMoveDirection) | static_cast<int>(EMouseDirection::Down));
			}
			else if(MouseDirection.y < -MOUSE_DIRECTION_SENSITIVITY)
			{
				MouseMoveDirection = static_cast<EMouseDirection>(static_cast<int>(MouseMoveDirection) | static_cast<int>(EMouseDirection::Up));
			}
		}
	}

	// Wheel
	{
		if(IsMouseWheelMoving == true)
		{
			if(IsMouseWheelMovingReset == true)
			{
				MouseWheelDelta = 0.0f;
				IsMouseWheelMoving = false;
				IsMouseWheelMovingReset = false;
			}
			else
			{
				IsMouseWheelMovingReset = true;
			}
		}
	}

	// Button
	{
		memcpy(KeyOldState, KeyState, sizeof(byte) * MAX_INPUT_MOUSE);
		ZeroMemory(KeyState, sizeof(byte) * MAX_INPUT_MOUSE);
		ZeroMemory(KeyMap, sizeof(byte) * MAX_INPUT_MOUSE);

		KeyState[GetMouseButtonIndex(VK_LBUTTON)] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
		KeyState[GetMouseButtonIndex(VK_RBUTTON)] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;
		KeyState[GetMouseButtonIndex(VK_MBUTTON)] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? 1 : 0;

		for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
		{
			int OldState = KeyOldState[i];
			int State = KeyState[i];

			if ((OldState == 0) && (State == 1))
			{
				KeyMap[i] = static_cast<byte>(EKeyStateType::Down);
			}
			else if ((OldState == 1) && (State == 0))
			{
				KeyMap[i] = static_cast<byte>(EKeyStateType::Up);
			}
			else if ((OldState == 1) && (State == 1))
			{
				KeyMap[i] = static_cast<byte>(EKeyStateType::Press);
			}
			else
			{
				KeyMap[i] = static_cast<byte>(EKeyStateType::None);
			}
		}
	}
}
void InputMouse::Destroy()
{
}

bool InputMouse::IsButtonDown(const DWORD& Button)
{
	int Index = GetMouseButtonIndex(Button);
	if(Index == -1)
	{
		return false;
	}
		
	if (KeyMap[Index] == static_cast<byte>(EKeyStateType::Down))
	{
		return true;
	}
	return false;
}
bool InputMouse::IsButtonUp(const DWORD& Button)
{
	int Index = GetMouseButtonIndex(Button);
	if(Index == -1)
	{
		return false;
	}
		
	if (KeyMap[Index] == static_cast<byte>(EKeyStateType::Up))
	{
		return true;
	}
	return false;
}
bool InputMouse::IsButtonPress(const DWORD& Button)
{
	int Index = GetMouseButtonIndex(Button);
	if(Index == -1)
	{
		return false;
	}
		
	if (KeyMap[Index] == static_cast<byte>(EKeyStateType::Press))
	{
		return true;
	}
	return false;
}

void InputMouse::ClearState()
{
	MousePosition = Vector2(0.0f);
	PrevMousePosition = Vector2(0.0f);
	MouseMoveDirection = EMouseDirection::None;
	
	MouseWheelDelta = 0.0f;
	IsMouseWheelMoving = false;
	IsMouseWheelMovingReset = false;
	
	ZeroMemory(KeyState, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(KeyOldState, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(KeyMap, sizeof(byte) * MAX_INPUT_MOUSE);
}

int InputMouse::GetMouseButtonIndex(const DWORD& Button)
{
	int Index = -1;
	
	if(Button == VK_LBUTTON)
	{
		Index = 0;
	}
	else if(Button == VK_RBUTTON)
	{
		Index = 1;
	}
	else if(Button == VK_MBUTTON)
	{
		Index = 2;
	}
	
	return Index;
}

void InputMouse::OnWheelEvent(const float& Delta)
{
	MouseWheelDelta = -1.0f * Delta;
	IsMouseWheelMoving = true;
	IsMouseWheelMovingReset = false;
}

