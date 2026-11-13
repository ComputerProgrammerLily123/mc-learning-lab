#include "input.h"
#include <GLFW/glfw3.h>

void Input::MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    Input::GetInstance().ProcessMousePosition(xpos, ypos);
}
void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    Input::GetInstance().ProcessMouseButton(button, action, mods);
}
glm::vec2 Input::GetMouseCurrentPosition()
{
    return currentMousePosition;
}
glm::vec2 Input::GetMouseDeltaPosition()
{
    return currentMousePosition - lastMousePosition;
}
void Input::ProcessMousePosition(double xposIn, double yposIn)
{
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);
    currentMousePosition = {xpos, ypos};
    if (isMouseFirstMove)
    {
        lastMousePosition = currentMousePosition;
        isMouseFirstMove = false;
    }
}
void Input::ProcessMouseButton(int button, int action, int mods)
{
    if (button < 0 || button >= currentMouseButtons.size()) return;

    currentMouseButtons[button] = action == GLFW_PRESS;
}
void Input::Update()
{
    previousMouseButtons = currentMouseButtons;
    m_previousKeys = m_currentKeys;
    lastMousePosition = currentMousePosition;
    deltaScroll = 0.0;
}
void Input::KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Input::GetInstance().ProcessKeyboard(key, scancode, action, mods);
}
bool Input::IsMouseButtonDown(int button) const
{
    if (button < 0 || button >= currentMouseButtons.size()) return false;

    return currentMouseButtons[button] && !previousMouseButtons[button];
}

bool Input::IsMouseButtonPressed(int button) const
{
    if (button < 0 || button >= currentMouseButtons.size()) return false;

    return currentMouseButtons[button];
}

bool Input::IsMouseButtonUp(int button) const
{
    if (button < 0 || button >= currentMouseButtons.size()) return false;

    return !currentMouseButtons[button] && previousMouseButtons[button];
}
bool Input::IsKeyDown(int key) const
{
    if (key < 0 || key >= KEY_COUNT)
        return false;
    return m_currentKeys[key] && !m_previousKeys[key];
}
bool Input::IsKeyPressed(int key) const
{
    if (key < 0 || key >= KEY_COUNT)
        return false;
    return m_currentKeys[key];
}
bool Input::IsKeyUp(int key) const
{
    if (key < 0 || key >= KEY_COUNT)
        return false;
    return !m_currentKeys[key] && m_previousKeys[key];
}
void Input::ProcessKeyboard(int key, int scancode, int action, int mods)
{
    if (key >= 0 && key < KEY_COUNT)
    {
        m_currentKeys[key] = (action != GLFW_RELEASE);
    }
}
void Input::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    Input::GetInstance().ProcessScoll(yOffset);
}
void Input::ProcessScoll(double yOffset)
{
    deltaScroll = yOffset;
}
double Input::GetDeltaScroll() const
{
    return deltaScroll;
}