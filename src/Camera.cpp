#include "Camera.h"

// constructor with vectors
Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f) ) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(m_speed)
{
    m_position = position;
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(m_speed)
{
    m_position = glm::vec3(posX, posY, posZ);
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        m_position += m_front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == Camera_Movement::LEFT)
        m_position -= m_right * velocity;
    if (direction == Camera_Movement::RIGHT)
        m_position += m_right * velocity;
}