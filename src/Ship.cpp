#include "Ship.h"

Ship::Ship()
{
    glGenVertexArrays(1, &m_shipVAO);
    glGenBuffers(1, &m_shipVBO);
    glGenBuffers(1, &m_shipEBO);

    glBindVertexArray(m_shipVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_shipVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_shipVertices), m_shipVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_shipEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_shipIndices), m_shipIndices, GL_STATIC_DRAW);

    // ship position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ship texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Ship::~Ship() {}

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