#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>

#define KEY_COUNT 512
#define MOUSE_BUTTON_COUNT 3
enum KeyState
{
    Release,
    Press
};
class GLFWwindow;
class Input
{
public:
    Input(const Input &) = delete;
    Input &operator=(const Input &) = delete;
    static Input &GetInstance()
    {
        static Input instance;
        return instance;
    }
    glm::vec2 GetMouseCurrentPosition();
    glm::vec2 GetMouseDeltaPosition();

    void UpdateMousePosition();
    bool IsMouseButtonDown(int button) const;
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonUp(int button) const;
    void UpdateKey();
    bool IsKeyDown(int key) const;
    bool IsKeyPressed(int key) const;
    bool IsKeyUp(int key) const;

    static void MouseButtonCallback(GLFWwindow *window,int button,int action,int mods);
    static void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    Input() = default;
    ~Input() = default;
    bool isMouseFirstMove = true;
    glm::vec2 lastMousePosition{0.0f, 0.0f};
    glm::vec2 currentMousePosition{0.0f, 0.0f};
    glm::vec2 deltaMousePosition{0.0f, 0.0f};

    bool m_currentKeys[KEY_COUNT] = {false};
    bool m_previousKeys[KEY_COUNT] = {false};

    int mouseButtonState[MOUSE_BUTTON_COUNT] = {0};

    void ProcessMousePosition(double xpos, double ypos);
    void ProcessMouseButton(int button, int action, int mods);
    void ProcessKeyboard(int key, int scancode, int action, int mods);
    void ProcessScoll();
};
#endif