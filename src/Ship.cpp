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

// TODO: Increase/decrease ship speed with arrow keys
void Ship::Ship::move(Ship_Movement movement, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

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