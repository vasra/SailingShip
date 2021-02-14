/*********************************************************************
 * \file   GameObject.h
 * \brief  All the objects that will be used in the game.
 * This file contains the namespace GameObject. To this namespace, 
 * belong the classes that represent the ship, the islands, the 
 * seagulls and the bugs. This namespace was created to avoid any
 * naming conflicts with the already existing code that is used. Since
 * the classes are small, they will be kept in this header file only.
 * from https://learnopengl.com.
 * \author Vasilis
 * \date   January 2021
 *********************************************************************/
#pragma once

#include <glad.h>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <cmath>
#include <Model.h>
#include <Shader.h>

namespace GameObject {   

    /// <summary>
    /// Enum used to describe the movement of the ship.
    /// </summary>
    enum class Ship_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        SPEED_UP,
        SPEED_DOWN
    };

    /// <summary>
    /// \class Island
    /// The class used to render the Island objects.
    /// </summary>
    class Island {
     public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="model"> The path of the 3D model that will be used.</param>
        /// <param name="origin">The location of the island in the world map.</param>
        Island(std::string& model, glm::vec3 origin) : m_islandModel(model), m_position(origin) {
            m_islandModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_islandModelMatrix = glm::scale(m_islandModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        }

        ~Island() {}

        // Getters

        /// <summary>
        /// Returns the 3D model of the island.
        /// </summary>
        const Model& getModel() const {
            return m_islandModel;
        }

        /// <summary>
        /// Returns the position of the island.
        /// </summary>
        const glm::vec3 getPosition() const {
            return m_position;
        }

        /// <summary>
        /// Renders the island when called in the main loop.
        /// </summary>
        /// <param name="shader">The main shader program.</param>
        void render(Shader& shader) {
            shader.setMat4("model", m_islandModelMatrix);
            m_islandModel.Draw(shader);
        }

     private:
        Model m_islandModel;           ///< The 3D model of the island.
        glm::vec3 m_position;          ///< The position of the island.
        glm::mat4 m_islandModelMatrix; ///< The island's model matrix.    
    };

    /// <summary>
    /// \class Bug
    /// The class used for rendering bugs.
    /// </summary>
    class Bug {
     public:
        /// <summary>
        /// The constructor for the bug.
        /// </summary>
        /// <param name="model">Path to the 3D model.</param>
        /// <param name="origin">The position in the world that the bug will spawn.</param>
        /// <param name="seagullPosition">The position of the seagull that the bug will be following.</param>
        Bug(std::string& model, glm::vec3 origin, glm::vec3 seagullPosition) : m_bugModel(model), m_position(origin) {
            m_bugModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_bugModelMatrix = glm::rotate(m_bugModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_bugModelMatrix = glm::scale(m_bugModelMatrix, glm::vec3(0.0003f, 0.0003f, 0.0003f));

            m_seagullOffsets = m_position - seagullPosition;
            m_radius = glm::sqrt(pow(m_seagullOffsets.x, 2.0f) + pow(m_seagullOffsets.z, 2.0f));
            m_angle = (m_seagullOffsets.x < 0) ? 180.0f : 0.0f;
        }

        ~Bug() {}

        /// <summary>
        /// Renders the bug. The bug will be rotating around its seagull over time, based
        /// on its current position in the x axis (left or right of the seagull).
        /// Scaling the model down to 0.0003 of its size was appropriate, in order to 
        /// look normal.
        /// </summary>
        /// <param name="seagullPosition">The position of the seagull.</param>
        /// <param name="shader">The current shader program.</param>
        void render(glm::vec3 seagullPosition, Shader& shader) {
            m_angle += 0.01f;
            if (m_angle > 360.0f)
                m_angle = 0.0f;

            m_position.x = seagullPosition.x + glm::sin(m_angle) * m_radius;
            m_position.z = seagullPosition.z + glm::cos(m_angle) * m_radius;

            m_bugModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_bugModelMatrix = glm::rotate(m_bugModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_bugModelMatrix = glm::scale(m_bugModelMatrix, glm::vec3(0.0003f, 0.0003f, 0.0003f));

            shader.setMat4("model", m_bugModelMatrix);
            m_bugModel.Draw(shader);
        }

     private:
        Model m_bugModel;           ///< The 3D model of the bug
        glm::vec3 m_position;       ///< The current position of the bug in the world
        glm::mat4 m_bugModelMatrix; ///< The model matrix of the bug
        glm::vec3 m_seagullOffsets; ///< Offsets from the seagull
        float m_radius;             ///< Radius from the seagull
        float m_angle;              ///< The angle towards the seagull
        
        friend class Seagull;
    };

    /// <summary>
    /// \class Seagull
    /// The Seagull object follows the ship around. When the ship turns, the seagulls turn along
    /// with it. Each seagull will have two bugs following them, initialized in the populate()
    /// function.
    /// </summary>
    class Seagull {
     public:
        /// <summary>
        /// The constructor for the seagull.
        /// </summary>
        /// <param name="model">Path to the 3D model.</param>
        /// <param name="origin">The position in the world that the seagull will spawn</param>
        /// <param name="shipPosition">The position of the ship that the seagull will be following.</param>
        Seagull(std::string& model, glm::vec3 origin, glm::vec3 shipPosition) : m_seagullModel(model), m_position(origin) {
            m_seagullModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_seagullModelMatrix = glm::rotate(m_seagullModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_seagullModelMatrix = glm::scale(m_seagullModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));

            m_shipOffsets = m_position - shipPosition;

            m_radius = glm::sqrt(pow(m_shipOffsets.x, 2.0f) + pow(m_shipOffsets.z, 2.0f)); 
        }

        ~Seagull() {}

        /// <summary>
        /// Renders the seagull according to the ship's position.
        /// </summary>
        /// <param name="shipPosition">The position of the ship.</param>
        /// <param name="shader">The current shader program.</param>
        void render(glm::vec3 shipPosition, Shader& shader) {
            m_seagullModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_seagullModelMatrix = glm::rotate(m_seagullModelMatrix, glm::radians(m_angle), glm::vec3(0.0f, 1.0f, 0.0f));
            m_seagullModelMatrix = glm::scale(m_seagullModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
            shader.setMat4("model", m_seagullModelMatrix);
            m_seagullModel.Draw(shader);
        }

        /// <summary>
        /// Initializes the Bug objects that will follow the seagull.
        /// \warning This method is not called in the constructor during the initialization of the Seagull object.
        ///          It must be called on its own after the object creation.
        /// </summary>
        /// <param name="bugModel">The path to the 3D model that will be used for the bugs.</param>
        void populate(std::string& bugModel) {
            m_bugs.emplace_back(bugModel, glm::vec3(m_position.x + 0.2f, m_position.y, m_position.z), m_position);
            m_bugs.emplace_back(bugModel, glm::vec3(m_position.x - 0.2f, m_position.y, m_position.z), m_position);
        }

        // Getters

        /// <summary>
        /// Returns the current position of the seagull.
        /// </summary>
        glm::vec3 getPosition() {
            return m_position;
        }
        
        /// <summary>
        /// Returns a reference to the vector with the bugs.
        /// </summary>
        std::vector<Bug>& getBugs() {
            return m_bugs;
        }
 
     private:
         float m_angle = 180.0f;         ///< Angle of the seagull relative to the y-axis. Used for rotating.
         float m_radius;                 ///< The radius that the seagull will use to circle the ship.
         Model m_seagullModel;           ///< The 3D model of the seagull.
         glm::vec3 m_position;           ///< The current position of the seagull.
         glm::mat4 m_seagullModelMatrix; ///< The model matrix used in the shaders.

         glm::vec3 m_shipOffsets;        ///< offsets from the ship.
         std::vector<Bug> m_bugs;        ///< vector containing the Bug objects.

         friend class Ship;
    };

    /// <summary>
    /// \class Ship
    /// This is the ship class that contains all the information about our ship. Regarding its movement, 
    /// the ship can move forward and backward, and also it can turn (rotate around the y-axis). The 
    /// seagulls following the ship move exactly the same way.
    /// </summary>
    class Ship {
     public:
         /// <summary>
         /// The constructor for the ship.
         /// </summary>
         /// <param name="shipModel">Path to the 3D model.</param>
         /// <param name="seagullModel">Path to the seagull's 3D model. Used in the populate() function.</param>
         /// <param name="origin">The position in the world that the ship will spawn.</param>
        Ship(std::string& shipModel, std::string& seagullModel, glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f)) : 
            m_shipModel(shipModel), 
            m_position(origin), 
            m_front(glm::vec3(0.0f, 0.0f, -1.0f)) {
            m_shipModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_shipModelMatrix = glm::rotate(m_shipModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            m_shipModelMatrix = glm::scale(m_shipModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
        }

        ~Ship() {}

        /// <summary>
        /// Moves the ships according to the button pressed. The ship
        /// either moves forward or backward, or turns left/right around
        /// the y-axis. Also, its speed changes if the Up/Down buttons 
        /// are pressed.
        /// </summary>
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

        /// <summary>
        /// Turns the ship around the y-axis. The seagulls follow accordingly.
        /// </summary>
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

        /// <summary>
        /// Renders the ship.
        /// </summary>
        void render(Shader& shader) {
            m_shipModelMatrix = glm::translate(glm::mat4(1.0f), m_position);
            m_shipModelMatrix = glm::rotate(m_shipModelMatrix, glm::radians(m_angle), glm::vec3(0.0f, 1.0f, 0.0f));
            m_shipModelMatrix = glm::scale(m_shipModelMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
            shader.setMat4("model", m_shipModelMatrix);
            m_shipModel.Draw(shader);
        }

        /// <summary>
        /// Creates the seagulls that will be following the ship. 
        /// In this case, two seagulls will be created, one on the left and one 
        /// on the right of the ship.
        /// </summary>
        void populate(std::string& seagullModel) {
            m_seagulls.emplace_back(seagullModel, glm::vec3(m_position.x + 1.0f, m_position.y + 1.0f, m_position.z), m_position);
            m_seagulls.emplace_back(seagullModel, glm::vec3(m_position.x - 1.0f, m_position.y + 1.0f, m_position.z), m_position);
        }

        // Getters

        /// <summary>
        /// Returns the 3D model of the ship.
        /// </summary>
        Model& getModel() {
            return m_shipModel;
        }

        /// <summary>
        /// Returns the current position of the ship.
        /// </summary>
        glm::vec3 getPosition() {
            return m_position;
        }

        /// <summary>
        /// Returns the front vector of the ship.
        /// </summary>
        glm::vec3 getFront() {
            return m_front;
        }

        /// <summary>
        /// Returns a reference to the vector with the Seagull objects.
        /// </summary>
        std::vector<Seagull>& getSeagulls() {
            return m_seagulls;
        }
        
        /// <summary>
        /// Returns the angle of the ship relative to the y-axis.
        /// </summary>
        float getAngle() {
            return m_angle;
        }

     private:
        float m_movementSpeed = 2.5f;    ///< The movement speed of the ship.
        float m_angle = 180.0f;          ///< The angle of the ship relative to the y-axis

        Model m_shipModel;               ///< The 3D model of the ship.

        glm::vec3 m_position;            ///< The current position of the ship.
        glm::mat4 m_shipModelMatrix;     ///< The ship's model matrix.
        glm::vec3 m_front;               ///< The ship's front vector.

        std::vector<Seagull> m_seagulls; ///< Vector containing all the seagulls following  the ship.
    };
}