#include "window.h"
#include <iostream>
Window::Window(unsigned width, unsigned height, const std::string &title): width(width),height(height)
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    nativeWindow = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
    if (nativeWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(nativeWindow);
    glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
    Center();
}
Window::~Window()
{
}
GLFWwindow *Window::GetNativeWindow() const
{
    return nativeWindow;
}
void Window::SetCursorPosCallback(GLFWcursorposfun func)
{
    glfwSetCursorPosCallback(nativeWindow, func);
}
void Window::SetCursorState(bool visibility)
{
    if (visibility)
        glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::SetFrameBufferSizeCallback(GLFWframebuffersizefun func)
{
    glfwSetFramebufferSizeCallback(nativeWindow, func);
}
void Window::SetKeyCallback(GLFWkeyfun func)
{
    glfwSetKeyCallback(nativeWindow, func);
}
void Window::SetScrollCallback(GLFWscrollfun func)
{
    glfwSetScrollCallback(nativeWindow, func);
}
void Window::SetMouseButtonCallback(GLFWmousebuttonfun func)
{
    glfwSetMouseButtonCallback(nativeWindow, func);
}
void Window::SetWindowUserPointer(void* pointer)
{
    glfwSetWindowUserPointer(nativeWindow,pointer);
}
bool Window::ShouldClose()
{
    return glfwWindowShouldClose(nativeWindow);
}
void Window::SetShouldClose()
{
    glfwSetWindowShouldClose(nativeWindow, true);
}
void Window::Center()
{
    glfwSetWindowPos(nativeWindow, static_cast<int>(mode->width - width) / 2, static_cast<int>(mode->height - height) / 2);
}
void Window::ToggleFullscreen()
{
    static int windowedX, windowedY, windowedW, windowedH;

    if (!isFullscreen)
    {
        glfwGetWindowPos(nativeWindow, &windowedX, &windowedY);
        glfwGetWindowSize(nativeWindow, &windowedW, &windowedH);
        glfwSetWindowMonitor(nativeWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(nativeWindow, nullptr, windowedX, windowedY, windowedW, windowedH, 0);
    }
    isFullscreen = !isFullscreen;
}
void Window::ResizeViewport(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;
    glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
}
unsigned Window::GetWidth() const
{
    return width;
}
unsigned Window::GetHeight() const
{
    return height;
}