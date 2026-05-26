#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;

    unsigned screenWidth;
    unsigned screenHeight;

    float mouseSensitivity = 0.1f;
    float zoom = 60.0f;

    Camera(unsigned screenWidth, unsigned screenHeight);

    glm::mat4 GetPerspectiveMatrix();
    glm::mat4 GetViewMatrix();

    void ProcessMouseMovement(float xoffset, float yoffset);
    void ResizeScreen(unsigned width, unsigned height);
    void SetPosition(glm::vec3 position);
    void SetPosition(float x, float y, float z);
    void SetYaw(float offset);
    void SetZoom(float zoom);

private:
    void UpdateCameraVectors();
};
#endif