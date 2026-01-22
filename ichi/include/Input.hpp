#pragma once

#include "KeyCodes.hpp"
#include <glm/glm.hpp>
#include <unordered_map>

struct Input
{
    void RegisterCallbacks();

    void SetKeyState(KeyCode key, KeyState state);
    void SetMouseButtonState(MouseButton button, KeyState state);

    void UpdateKeyStates();

    KeyState GetKeyState(KeyCode key);
    KeyState GetMouseButtonState(MouseButton button);

    glm::vec2 GetMousePosition();

    bool IsKeyDown(KeyCode key);
    bool IsKeyPressed(KeyCode key);
    bool IsMouseButtonDown(MouseButton button);
    bool IsMouseButtonPressed(MouseButton button);

    std::unordered_map<KeyCode, KeyState> keyStates;
    std::unordered_map<MouseButton, KeyState> mouseButtonStates;

    static Input& Get();
};