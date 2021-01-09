#pragma once

#include <glad.h>

class Island
{
public:
    Island();
    ~Island();

    void bind();
    void unBind();

    size_t getIndicesSize();

private:
    float m_islandVertices[20] = {
        // positions        // texture coordinates
        0.6f, 0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        0.6f, 1.0f, 0.0f,   0.0f, 1.0f, // upper left
        1.0f, 0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f  // upper right
    };
    unsigned int m_islandIndices[6] = {
        0, 1, 2, //first triangle
        1, 2, 3 // second triangle
    };

    unsigned int m_islandVAO, m_islandVBO, m_islandEBO;
};

