#pragma once
#include <glm/glm.hpp>
#include <array>

#define KEY_COUNT 512
#define MOUSE_BUTTON_COUNT 3
enum class KeyState : unsigned char
{
    Release,
    Press
};
class GLFWwindow;
class Input
{
public:
    Input(Input&&) = delete;
    Input& operator=(Input&&) = delete;
    Input(const Input&) = delete;
    Input &operator=(const Input &) = delete;
    static Input &GetInstance()
    {
        static Input instance;
        return instance;
    }
    glm::vec2 GetMouseCurrentPosition();
    glm::vec2 GetMouseDeltaPosition();
    [[nodiscard]] bool IsMouseButtonDown(int button) const;
    [[nodiscard]] bool IsMouseButtonPressed(int button) const;
    [[nodiscard]] bool IsMouseButtonUp(int button) const;
    [[nodiscard]] bool IsKeyDown(int key) const;
    [[nodiscard]] bool IsKeyPressed(int key) const;
    [[nodiscard]] bool IsKeyUp(int key) const;
    [[nodiscard]] double GetDeltaScroll() const;
    void UpdateMousePosition();
    void UpdateKey();
    void UpdateScroll();

    static void MouseButtonCallback(GLFWwindow *window,int button,int action,int mods);
    static void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void ScrollCallback(GLFWwindow* window,double xOffset,double yOffset);

private:
    Input() = default;
    ~Input() = default;
    bool isMouseFirstMove = true;
    glm::vec2 lastMousePosition{0.0f, 0.0f};
    glm::vec2 currentMousePosition{0.0f, 0.0f};
    glm::vec2 deltaMousePosition{0.0f, 0.0f};

    std::array<bool,KEY_COUNT> m_currentKeys = {false};
    std::array<bool,KEY_COUNT> m_previousKeys = {false};

    std::array<int,MOUSE_BUTTON_COUNT> mouseButtonState = {0};

    double lastScrollPos{0.0};
    double currentScrollPos{0.0};
    double deltaScroll{0.0};

    void ProcessMousePosition(double xpos, double ypos);
    void ProcessMouseButton(int button, int action, int mods);
    void ProcessKeyboard(int key, int scancode, int action, int mods);
    void ProcessScoll(double yOffset);
};