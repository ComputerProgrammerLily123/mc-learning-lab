#pragma once
#include <GLFW/glfw3.h>
#include <string>
class Window
{
public:
    Window(const Window&) = default;
    ~Window();
    Window(Window&&) = delete;
    Window& operator=(const Window&) = default;
    Window& operator=(Window&&) = delete;
    Window(unsigned width, unsigned height, const std::string& title);
    [[nodiscard]] GLFWwindow* GetNativeWindow() const;
    void SetCursorPosCallback(GLFWcursorposfun func);
    void SetCursorState(bool visibility);
    void SetFrameBufferSizeCallback(GLFWframebuffersizefun func);
    void SetKeyCallback(GLFWkeyfun func);
    void SetScrollCallback(GLFWscrollfun func);
    void SetMouseButtonCallback(GLFWmousebuttonfun func);
    void SetWindowUserPointer(void* pointer);

    bool ShouldClose();
    void SetShouldClose();
    void ToggleFullscreen();
    void ResizeViewport(unsigned width, unsigned height);

    [[nodiscard]] unsigned GetWidth() const;
    [[nodiscard]] unsigned GetHeight() const;
private:
    GLFWwindow *nativeWindow;
    GLFWmonitor *monitor;
    const GLFWvidmode *mode;

    unsigned width;
    unsigned height;

    bool isFullscreen = false;

    void Center();
};