#pragma once

#include <glad.h>
#include <glm.hpp>
#include <matrix_transform.hpp>

#include <Model.h>
#include <Shader.h>

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
        Ship(std::string& model, glm::vec3 origin = glm::vec3(0.0f, -1.0f, 0.0f));
        ~Ship();

        void move(Ship_Movement movement, float deltaTime);

        glm::vec3 getPosition();

        Model& getModel();

        void render(Shader& shader);

    private:

        float m_movementSpeed = 2.5f;

        Model m_shipModel;

        glm::vec3 m_position;
        glm::mat4 m_shipModelMatrix;
    };
}