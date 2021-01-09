#pragma once

#include <glad.h>
#include <vector>

class Ship
{
public:
    Ship();
    ~Ship();
    
    void bind();
    void unBind();

    size_t getIndicesSize();

private:
    float m_shipVertices[20] = {
        // positions           // texture coordinates
        -0.2f, -0.9f, 0.0f,    0.0f, 0.0f, // bottom left
        -0.2f, -0.1f, 0.0f,    0.0f, 1.0f, // upper left
         0.2f, -0.9f, 0.0f,    1.0f, 0.0f, // bottom right
         0.2f, -0.1f, 0.0f,    1.0f, 1.0f  // upper right
    };

    unsigned int m_shipIndices[6] = {
        0, 1, 2, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int m_shipVAO, m_shipVBO, m_shipEBO;
};

