#pragma once

#include <glad.h>
#include <glm.hpp>
#include <matrix_transform.hpp>

#include <Model.h>
#include <Shader.h>

namespace GameObject {   

    enum class Ship_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        SPEED_UP,
        SPEED_DOWN
    };

    class Island {
     public:
        Island(std::string& model, glm::vec3 origin) : m_islandModel(model), m_position(origin) {
            m_islandModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_islandModelMatrix = glm::scale(m_islandModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        }

        ~Island() {}

        Model& getModel() {
            return m_islandModel;
        }

        glm::vec3 getPosition() {
            return m_position;
        }

        void render(Shader& shader) {
            shader.setMat4("model", m_islandModelMatrix);
            m_islandModel.Draw(shader);
        }

     private:
        Model m_islandModel;
        glm::vec3 m_position;
        glm::mat4 m_islandModelMatrix;
    };

    class Bug {
     public:
        Bug(std::string& model, glm::vec3 origin, glm::vec3 seagullPosition) : m_bugModel(model), m_position(origin) {
            m_bugModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_bugModelMatrix = glm::rotate(m_bugModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_bugModelMatrix = glm::scale(m_bugModelMatrix, glm::vec3(0.0003f, 0.0003f, 0.0003f));

            m_seagullOffsets = m_position - seagullPosition;
        }

        ~Bug() {}

        Model& getModel() {
            return m_bugModel;
        }

        glm::vec3 getPosition() {
            return m_position;
        }

        void render(glm::vec3 seagullPosition, Shader& shader) {
            m_position = m_seagullOffsets + seagullPosition;
            m_bugModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_bugModelMatrix = glm::rotate(m_bugModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_bugModelMatrix = glm::scale(m_bugModelMatrix, glm::vec3(0.0003f, 0.0003f, 0.0003f));
            shader.setMat4("model", m_bugModelMatrix);
            m_bugModel.Draw(shader);
        }

     private:
        Model m_bugModel;
        glm::vec3 m_position;
        glm::mat4 m_bugModelMatrix;
        bool print = true;
        // offsets from the seagull
        glm::vec3 m_seagullOffsets;

        friend class Seagull;
    };

    class Seagull {
     public:
        Seagull(std::string& model, glm::vec3 origin, glm::vec3 shipPosition) : m_seagullModel(model), m_position(origin) {
            m_seagullModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_seagullModelMatrix = glm::rotate(m_seagullModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_seagullModelMatrix = glm::scale(m_seagullModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));

            m_shipOffsets = m_position - shipPosition;
            m_radius = glm::sqrt((m_position.x - shipPosition.x) * (m_position.x - shipPosition.x) + (m_position.z - shipPosition.z) * (m_position.z - shipPosition.z));
        }

        ~Seagull() {}

        Model& getModel() {
            return m_seagullModel;
        }

        glm::vec3 getPosition() {
            return m_position;
        }

        void render(glm::vec3 shipPosition, Shader& shader) {
            m_seagullModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_seagullModelMatrix = glm::rotate(m_seagullModelMatrix, glm::radians(m_angle), glm::vec3(0.0f, 1.0f, 0.0f));
            m_seagullModelMatrix = glm::scale(m_seagullModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
            shader.setMat4("model", m_seagullModelMatrix);
            m_seagullModel.Draw(shader);
        }

        void populate(std::string& bugModel) {
            m_bugs.emplace_back(bugModel, glm::vec3(m_position.x + 0.2f, m_position.y, m_position.z), m_position);
            m_bugs.emplace_back(bugModel, glm::vec3(m_position.x - 0.2f, m_position.y, m_position.z), m_position);
        }

        std::vector<Bug>& getBugs() {
            return m_bugs;
        }

     private:
         float m_angle = 180.0f;
         float m_radius;
         Model m_seagullModel;
         glm::vec3 m_position;
         glm::mat4 m_seagullModelMatrix;

         // offsets from the ship
         glm::vec3 m_shipOffsets;
         std::vector<Bug> m_bugs;

         friend class Ship;
    };

    class Ship {
     public:
        Ship(std::string& shipModel, std::string& seagullModel, glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f)) : 
            m_shipModel(shipModel), 
            m_position(origin), 
            m_front(glm::vec3(0.0f, 0.0f, -1.0f)) {
            m_shipModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_shipModelMatrix = glm::rotate(m_shipModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_shipModelMatrix = glm::scale(m_shipModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
        }

        ~Ship() {}

        Model& getModel() {
            return m_shipModel;
        }

        glm::vec3 getPosition() {
            return m_position;
        }

        void move(Ship_Movement movement, float deltaTime) {
            if (movement == Ship_Movement::SPEED_UP) {
                m_movementSpeed += 0.05f;
                if (m_movementSpeed > 10.0f)
                    m_movementSpeed = 10.0f;
            } else if (movement == Ship_Movement::SPEED_DOWN) {
                m_movementSpeed -= 0.05f;
                if (m_movementSpeed < 1.0f)
                    m_movementSpeed = 1.0f;
            }

            float velocity = m_movementSpeed * deltaTime;

            if (movement == Ship_Movement::FORWARD) {
                m_position += m_front * velocity;
                for (auto& seagull : m_seagulls)
                    seagull.m_position += m_front * velocity;
            } else if (movement == Ship_Movement::BACKWARD) {
                m_position -= m_front * velocity;
                for (auto& seagull : m_seagulls)
                    seagull.m_position -= m_front * velocity;
            } else if (movement == Ship_Movement::LEFT) {
                turn(Ship_Movement::LEFT);
            } else if (movement == Ship_Movement::RIGHT) {
                turn(Ship_Movement::RIGHT);
            }
        }

        void turn(Ship_Movement turn) {
            if (turn == Ship_Movement::LEFT)
                m_angle += 0.5f;
            else if(turn == Ship_Movement::RIGHT)
                m_angle -= 0.5f;
            m_front.x = glm::sin(glm::radians(m_angle));
            m_front.z = glm::cos(glm::radians(m_angle));

            for (auto& seagull : m_seagulls) {
                if (seagull.m_shipOffsets.x > 0) {
                    seagull.m_position.x = m_position.x - glm::cos(glm::radians(m_angle)) * seagull.m_radius;
                    seagull.m_position.z = m_position.z + glm::sin(glm::radians(m_angle)) * seagull.m_radius;
                } else if (seagull.m_shipOffsets.x < 0) {
                    seagull.m_position.x = m_position.x + glm::cos(glm::radians(m_angle)) * seagull.m_radius;
                    seagull.m_position.z = m_position.z - glm::sin(glm::radians(m_angle)) * seagull.m_radius;
                }
                if (turn == Ship_Movement::LEFT)
                    seagull.m_angle += 0.5f;
                else if (turn == Ship_Movement::RIGHT)
                    seagull.m_angle -= 0.5f;
            }
        }

        void render(Shader& shader) {
            m_shipModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_shipModelMatrix = glm::rotate(m_shipModelMatrix, glm::radians(m_angle), glm::vec3(0.0f, 1.0f, 0.0f));
            m_shipModelMatrix = glm::scale(m_shipModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
            shader.setMat4("model", m_shipModelMatrix);
            m_shipModel.Draw(shader);
        }

        void populate(std::string& seagullModel) {
            m_seagulls.emplace_back(seagullModel, glm::vec3(m_position.x + 1.0f, m_position.y + 1.0f, m_position.z), m_position);
            m_seagulls.emplace_back(seagullModel, glm::vec3(m_position.x - 1.0f, m_position.y + 1.0f, m_position.z), m_position);
        }

        glm::vec3 getFront() {
            return m_front;
        }

        float getAngle() {
            return m_angle;
        }

        std::vector<Seagull>& getSeagulls() {
            return m_seagulls;
        }

     private:
        float m_movementSpeed = 2.5f;
        float m_angle = 180.0f;

        Model m_shipModel;

        glm::vec3 m_position;
        glm::mat4 m_shipModelMatrix;
        glm::vec3 m_front;

        std::vector<Seagull> m_seagulls;
    };
}