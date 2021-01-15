#pragma once

#include <glad.h>
#include <glm.hpp>
#include <matrix_transform.hpp>

#include <Model.h>
#include <vector>

namespace Ship
{
    enum class Ship_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Ship
    {
    public:
        Ship();
        ~Ship();

        void bind() const;
        void unBind() const;

        size_t getIndicesSize() const;
        glm::mat4 move(Ship_Movement movement, float deltaTime);

        Model* getModel();

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

        unsigned int m_shipVAO;
        unsigned int m_shipVBO;
        unsigned int m_shipEBO;

        const float m_movementSpeed = 2.5f;

        Model* m_shipModel;
    };

}