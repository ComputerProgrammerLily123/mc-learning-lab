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
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    currentMousePosition = {xpos, ypos};
    if (isMouseFirstMove)
    {
        lastMousePosition = currentMousePosition;
        isMouseFirstMove = false;
    }
}
void Input::ProcessMouseButton(int button, int action, int mods)
{
    mouseButtonState[button] = action;
}
void Input::KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Input::GetInstance().ProcessKeyboard(key, scancode, action, mods);
}
void Input::UpdateMousePosition()
{
    deltaMousePosition = currentMousePosition - lastMousePosition;
    lastMousePosition = currentMousePosition;
}
bool Input::IsMouseButtonDown(int button) const
{
    return mouseButtonState[button] == GLFW_PRESS;
}
bool Input::IsMouseButtonPressed(int button) const
{
    return mouseButtonState[button] == GLFW_REPEAT;
}
bool Input::IsMouseButtonUp(int button) const
{
    return mouseButtonState[button] == GLFW_RELEASE;
}
void Input::UpdateKey()
{
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        m_previousKeys[i] = m_currentKeys[i];
    }
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
void Input::UpdateScroll(){
    deltaScroll = 0;
}