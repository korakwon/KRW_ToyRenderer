#include "InputKeyboard.h"

using Core::InputKeyboard;

InputKeyboard::InputKeyboard()
{
    ClearState();
}
InputKeyboard::~InputKeyboard()
{
    Destroy();
}

void InputKeyboard::Initialize()
{
    ClearState();
}
void InputKeyboard::Update(const float& DeltaTime)
{
    memcpy(KeyOldState, KeyState, sizeof(byte) * MAX_INPUT_KEY);
    ZeroMemory(KeyState, sizeof(byte) * MAX_INPUT_KEY);
    ZeroMemory(KeyMap, sizeof(byte) * MAX_INPUT_KEY);

    GetKeyboardState(KeyState);

    for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
    {
        byte Key = KeyState[i] & 0x80;
        KeyState[i] = Key ? 1 : 0;

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
void InputKeyboard::Destroy()
{
}

void InputKeyboard::ClearState()
{
    ZeroMemory(KeyState, sizeof(byte) * MAX_INPUT_KEY);
    ZeroMemory(KeyOldState, sizeof(byte) * MAX_INPUT_KEY);
    ZeroMemory(KeyMap, sizeof(byte) * MAX_INPUT_KEY);
}
