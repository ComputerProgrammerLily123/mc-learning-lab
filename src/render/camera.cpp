#include "camera.h"

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