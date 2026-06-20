#pragma once
#include <GLFW/glfw3.h>
#include <string>
class Window
{
public:
    Window(unsigned width, unsigned height, const std::string &title);
    ~Window();
    GLFWwindow* GetNativeWindow() const;
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
    void ResizeWindow(unsigned width, unsigned height);
private:
    GLFWwindow *nativeWindow;
    GLFWmonitor *monitor;
    const GLFWvidmode *mode;

    unsigned width;
    unsigned height;

    bool isFullscreen = false;

    void Center();
};