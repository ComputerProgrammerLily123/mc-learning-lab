#include "camera.h"
#include <glm/gtc/matrix_access.hpp>

Camera::Camera(unsigned screenWidth, unsigned screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
{
    UpdateCameraVectors();
}
glm::mat4 Camera::GetPerspectiveMatrix()
{
    return glm::perspective(glm::radians(zoom), (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
}
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}
std::array<glm::vec4, 6> Camera::GetViewPlanes()
{
    const auto& vp = GetPerspectiveMatrix() * GetViewMatrix();
    std::array<glm::vec4, 6> planes {};
    planes[0] = glm::row(vp, 3) + glm::row(vp, 0);
    planes[1] = glm::row(vp, 3) - glm::row(vp, 0);
    planes[2] = glm::row(vp, 3) + glm::row(vp, 1);
    planes[3] = glm::row(vp, 3) - glm::row(vp, 1);
    planes[4] = glm::row(vp, 3) + glm::row(vp, 2);
    planes[5] = glm::row(vp, 3) - glm::row(vp, 2);
    for (auto &p : planes)
    {
        float len = glm::length(glm::vec3(p));
        if (len > 0)
            p /= len;
    }
    return planes;
}
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    UpdateCameraVectors();
}
void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(right, front));
}
void Camera::ResizeScreen(unsigned width, unsigned height)
{
    screenWidth = width;
    screenHeight = height;
}
void Camera::SetPosition(glm::vec3 position)
{
    this->position = position;
}
void Camera::SetPosition(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}
void Camera::SetYaw(float offset)
{
    yaw += offset;
    UpdateCameraVectors();
}
void Camera::SetZoom(float zoom)
{
    this->zoom = zoom;
}