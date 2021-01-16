#pragma once

#include <glad.h>
#include <glm.hpp>
#include <matrix_transform.hpp>

#include <Model.h>
#include <Shader.h>

namespace Island
{
    class Island
    {
    public:
        Island(std::string& model, glm::vec3 origin);
        ~Island();

        Model& getModel();

        glm::vec3 getPosition();

        void render(Shader& shader);

    private:
        Model m_islandModel;
        glm::vec3 m_position;
        glm::mat4 m_islandModelMatrix;
    };
}