#include "Ship.h"

Ship::Ship::Ship()
{
    m_shipModel = new Model( "C:\\Users\\rava\\Desktop\\OpenGL\\SailingShip\\textures\\galleon-16th-century-ship\\GALEON.obj" );
}

Ship::Ship::~Ship()
{
    delete m_shipModel;
}

void Ship::Ship::bind() const
{
    glBindVertexArray(m_shipVAO);
}

void Ship::Ship::unBind() const
{
    glBindVertexArray(0);
}

size_t Ship::Ship::getIndicesSize() const
{
    return sizeof(m_shipIndices);
}

Model* Ship::Ship::getModel()
{
    return m_shipModel;
}

glm::mat4 Ship::Ship::move(Ship_Movement movement, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    glm::mat4 model = glm::mat4(1.0f);

    if (movement == Ship_Movement::FORWARD) {
        model = glm::translate(model, glm::vec3(0, 0, velocity));
    }

    return model;
}