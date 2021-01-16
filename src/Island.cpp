#include "Island.h"

Island::Island::Island(std::string& model, glm::vec3 origin) : m_islandModel(model), m_position(origin)
{
    m_islandModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
    m_islandModelMatrix = glm::scale(m_islandModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
}

Island::Island::~Island() {}

Model& Island::Island::getModel()
{
    return m_islandModel;
}

glm::vec3 Island::Island::getPosition()
{
    return m_position;
}

void Island::Island::render(Shader& shader)
{
    shader.setMat4("model", m_islandModelMatrix);
    m_islandModel.Draw(shader);
}