#include "window.h"
#include <iostream>
Window::Window(unsigned width, unsigned height, const std::string &title)
{
    nativeWindow = glfwCreateWindow(width, height, "Minecraft:Papyrus Edition", NULL, NULL);
    if (nativeWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(nativeWindow);
    glViewport(0, 0, width, height);
    Center(width, height);
    SetFrameBufferSizeCallback([](GLFWwindow *window, int width, int height)
                               { glViewport(0, 0, width, height); });
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
    glfwSetMouseButtonCallback(nativeWindow,func);
}
bool Window::ShouldClose()
{
    return glfwWindowShouldClose(nativeWindow);
}
void Window::SetShouldClose()
{
    glfwSetWindowShouldClose(nativeWindow, true);
}

void Window::Center(int width, int height)
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(this->nativeWindow, (mode->width - width) / 2, (mode->height - height) / 2);
}