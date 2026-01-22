#include <rgfw.hpp>

#include "../include/Input.hpp"
#include "../include/KeyCodes.hpp"
#include "../include/Profiler.hpp"

#include <format>

void KeyCallback(RGFW_window* window, RGFW_key key, u8 keyChar, RGFW_keymod keyMod, RGFW_bool repeat, RGFW_bool pressed);
void MouseCallback(RGFW_window* window, RGFW_mouseButton button, RGFW_bool pressed);

Input& Input::Get()
{
    static Input input{};
    return input;
}

void Input::RegisterCallbacks()
{
    RGFW_setKeyCallback(KeyCallback);
    RGFW_setMouseButtonCallback(MouseCallback);
}

void Input::SetKeyState(KeyCode key, KeyState state)
{
    std::printf(std::format("SetKeyState - key: {} state: {}\n", (uint8_t)key, (uint8_t)state).c_str());
    keyStates[key] = state;
}

void Input::SetMouseButtonState(MouseButton button, KeyState state)
{
    std::printf(std::format("SetMouseButtonState - key: {} state: {}\n", (uint8_t)button, (uint8_t)state).c_str());
    mouseButtonStates[button] = state;
}

KeyState Input::GetKeyState(KeyCode code)
{
    return keyStates[code];
}

KeyState Input::GetMouseButtonState(MouseButton button)
{
    return mouseButtonStates[button];
}

glm::vec2 Input::GetMousePosition()
{
    float xpos, ypos;
    RGFW_getMouseVector(&xpos, &ypos);
    return { xpos, ypos };
}

void Input::UpdateKeyStates()
{
    Profiler::Get().BeginMarker("Input::UpdateKeyStates");

    for (auto&& pair : keyStates)
    {
        KeyCode keyCode = pair.first;
        KeyState state = pair.second;

        if (state == KeyState::None || state == KeyState::Held) continue;

        if (state == KeyState::Pressed) state = KeyState::Held;
        if (state == KeyState::Released) state = KeyState::None;

        SetKeyState(keyCode, state);
    }

    for (auto&& pair : mouseButtonStates)
    {
        MouseButton button = pair.first;
        KeyState state = pair.second;

        if (state == KeyState::None || state == KeyState::Held) continue;

        if (state == KeyState::Pressed) state = KeyState::Held;
        if (state == KeyState::Released) state = KeyState::None;

        SetMouseButtonState(button, state);
    }

    Profiler::Get().EndMarker();
}

bool Input::IsKeyDown(KeyCode key)
{
    KeyState state = GetKeyState(key);
    return state == KeyState::Pressed || state == KeyState::Held;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
    KeyState state = GetMouseButtonState(button);
    return state == KeyState::Pressed || state == KeyState::Held;
}

bool Input::IsKeyPressed(KeyCode key)
{
    KeyState state = GetKeyState(key);
    return state == KeyState::Pressed;
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
    KeyState state = GetMouseButtonState(button);
    return state == KeyState::Pressed;
}

static void KeyCallback(RGFW_window* window, RGFW_key key, u8 keyChar, RGFW_keymod keyMod, RGFW_bool repeat, RGFW_bool pressed)
{
    if (repeat & pressed) return;
    Input::Get().SetKeyState((KeyCode)key, pressed ? KeyState::Pressed : KeyState::Released);
}

static void MouseCallback(RGFW_window* window, RGFW_mouseButton button, RGFW_bool pressed)
{
    Input::Get().SetMouseButtonState((MouseButton)button, pressed ? KeyState::Pressed : KeyState::Released);
}
