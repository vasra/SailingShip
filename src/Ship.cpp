#include "Ship.h"

Ship::Ship()
{
    m_shipModel = new Model( "C:\\Users\\billaros\\source\\repos\\SailingShip\\textures\\galleon-16th-century-ship\\GALEON.obj" );
    //glGenVertexArrays(1, &m_shipVAO);
    //glGenBuffers(1, &m_shipVBO);
    //glGenBuffers(1, &m_shipEBO);

    //glBindVertexArray(m_shipVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, m_shipVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(m_shipVertices), m_shipVertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_shipEBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_shipIndices), m_shipIndices, GL_STATIC_DRAW);

    //// ship position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// ship texture attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
}

Ship::~Ship()
{
    delete m_shipModel;
}

void Ship::bind() const
{
    glBindVertexArray(m_shipVAO);
}

void Ship::unBind() const
{
    glBindVertexArray(0);
}

size_t Ship::getIndicesSize() const
{
    return sizeof(m_shipIndices);
}

Model* Ship::getModel()
{
    return m_shipModel;
}

glm::mat4 Ship::move(Ship_Movement movement, float dx, float dy, float dz)
{
    glm::mat4 model = glm::mat4(1.0f);

    if (movement == Ship_Movement::FORWARD) {
        model = glm::translate(model, glm::vec3(dx, dy, dz));
    }

    return model;
}