#ifndef WINDOW_H
#define WINDOW_H

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

    bool ShouldClose();
    void SetShouldClose();
private:
    GLFWwindow *nativeWindow;
    unsigned width;
    unsigned height;
    std::string title;

    void Center(int width,int height);
};
#endif