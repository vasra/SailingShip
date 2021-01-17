#include "Ship.h"

Ship::Ship::Ship(std::string& model, glm::vec3 origin) : m_shipModel(model), m_position(origin)
{
    m_shipModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
    m_shipModelMatrix = glm::rotate(m_shipModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_shipModelMatrix = glm::scale(m_shipModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
}

Ship::Ship::~Ship() {}

Model& Ship::Ship::getModel()
{
    return m_shipModel;
}

glm::vec3 Ship::Ship::getPosition()
{
    return m_position;
}

void Ship::Ship::move(Ship_Movement movement, float deltaTime)
{
    if (movement == Ship_Movement::SPEED_UP) {
        m_movementSpeed += 0.05f;
        if (m_movementSpeed > 10.0f)
            m_movementSpeed = 10.0f;
    }
    else if (movement == Ship_Movement::SPEED_DOWN) {
        m_movementSpeed -= 0.05f;
        if (m_movementSpeed < 1.0f)
            m_movementSpeed = 1.0f;
    }
    std::cout << "Ship speed " << m_movementSpeed << std::endl;

    float velocity = m_movementSpeed * deltaTime;
    std::cout << "Ship velocity " << velocity << std::endl;

    if (movement == Ship_Movement::FORWARD) {
        m_position -= glm::vec3(0.0f, 0.0f, velocity);
    }
    else if(movement == Ship_Movement::BACKWARD) {
        m_position += glm::vec3(0.0f, 0.0f, velocity);
    }
    else if (movement == Ship_Movement::LEFT) {
        m_position -= glm::vec3(velocity, 0.0f, 0.0f);
    }
    else if (movement == Ship_Movement::RIGHT) {
        m_position += glm::vec3(velocity, 0.0f, 0.0f);
    }
}

void Ship::Ship::render(Shader& shader)
{
    m_shipModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
    m_shipModelMatrix = glm::rotate(m_shipModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_shipModelMatrix = glm::scale(m_shipModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
    shader.setMat4("model", m_shipModelMatrix);
    m_shipModel.Draw(shader);
}