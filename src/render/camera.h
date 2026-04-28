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

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    unsigned screenWidth;
    unsigned screenHeight;

    float mouseSensitivity;
    float zoom;

    Camera(unsigned screenWidth, unsigned screenHeight, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    glm::mat4 GetPerspectiveMatrix();
    glm::mat4 GetViewMatrix();

    void ProcessMouseMovement(float xoffset, float yoffset);
    void ResizeScreen(unsigned width, unsigned height);
    void SetPosition(glm::vec3 position);
    void SetPosition(float x, float y, float z);

private:
    void UpdateCameraVectors();
};
#endif