#include "Island.h"

Island::Island()
{
    glGenVertexArrays(1, &m_islandVAO);
    glGenBuffers(1, &m_islandVBO);
    glGenBuffers(1, &m_islandEBO);

    glBindVertexArray(m_islandVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_islandVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_islandVertices), m_islandVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_islandEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_islandIndices), m_islandIndices, GL_STATIC_DRAW);

    // island position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // island texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Island::~Island()
{
}

void Island::bind()
{
    glBindVertexArray(m_islandVAO);
}

void Island::unBind()
{
    glBindVertexArray(0);
}

size_t Island::getIndicesSize()
{
    return sizeof(m_islandIndices);
}