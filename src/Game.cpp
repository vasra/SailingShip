/*****************************************************************//**
 * \file   Game.cpp
 * \brief  This file contains the main() method of the game. Everything 
 * is initialized here.
 * \author Vasilis
 * \date   February 2021
 *********************************************************************/
#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include <string_cast.hpp>

#include <Shader.h>
#include <Model.h>
#include <GameObject.h>
#include <C:\Users\billaros\source\repos\SailingShip\header\header\Camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

void processInput(GLFWwindow* window, GameObject::Ship& ship);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Globals

// window settings
const unsigned int windowWidth = 1024;
const unsigned int windowHeight = 768;

// paths to the shaders
std::string vShader{ "shaders\\vShader.txt" };
std::string fShader{ "shaders\\fshader.txt" };

// paths to the 3D models
std::string shipModel{ "textures\\galleon-16th-century-ship\\GALEON.obj" };
std::string islandModel{ "textures\\TropicalIsland_extras\\TropicalIsland.obj" };
std::string seagullModel{ "textures\\3DLowPoly-Seagull\\Seagull.obj" };
std::string bugModel{ "textures\\Dragonfly\\Dragonfly.obj" };

Camera::Camera camera{ glm::vec3(0.0f, 1.0f, 3.0f) };

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// the positions of the islands in the world
std::vector<glm::vec3> islandPositions{
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, -3.0f),
        glm::vec3(-2.0f, 0.0f, 4.0f),
        glm::vec3(2.0f, 0.0f, 4.0f),
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(-5.0f, 0.0f, -6.0f)
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Let's sail!", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader(vShader, fShader);

    // Create the islands
    std::vector<GameObject::Island> islands;
    for(auto& position : islandPositions)
         islands.emplace_back(islandModel, position);

    // Create the ship and generate the seagulls
    GameObject::Ship ship{ shipModel, seagullModel };
    ship.populate(seagullModel);
    std::vector<GameObject::Seagull>& seagulls = ship.getSeagulls();
    
    // For each seagull, generate its bugs
    for (auto& seagull : seagulls)
        seagull.populate(bugModel);

    // Set the projection matrix once outside the main loop, as it will remain the
    // same throughout the execution of the program
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);
    shader.setMat4("projection", projection);
    
    // Light properties
    shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // Material properties
    shader.setFloat("material.shininess", 32.0f);

    // View matrix. It is initialized with the camera position
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        shader.use();
        processInput(window, ship);

        glClearColor(0.0f, 0.1f, 0.858824f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        view = camera.GetViewMatrix(ship);
        
        shader.use();
        shader.setVec3("viewPos", camera.Position);
        shader.setMat4("view", view);
        
        // Render the ship, the islands, the seagulls and the bugs
        ship.render(shader);
        for (auto& island : islands)
            island.render(shader);
        
        for (auto& seagull : seagulls) {
            seagull.render(ship.getPosition(), shader);
            for (auto& bug : seagull.getBugs())
                bug.render(seagull.getPosition(), shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/// <summary>
/// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
/// </summary>
/// <param name="window">Pointer to the OpenGL active window.</param>
/// <param name="shader">Reference to the active shader program.</param>
/// <param name="ship">Reference to the ship.</param>
void processInput(GLFWwindow* window, GameObject::Ship& ship) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        ship.move(GameObject::Ship_Movement::SPEED_UP, deltaTime);
        camera.ProcessKeyboard(Camera::Camera_Movement::SPEED_UP, deltaTime, ship);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        ship.move(GameObject::Ship_Movement::SPEED_DOWN, deltaTime);
        camera.ProcessKeyboard(Camera::Camera_Movement::SPEED_DOWN, deltaTime, ship);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        ship.move(GameObject::Ship_Movement::FORWARD, deltaTime);
        camera.ProcessKeyboard(Camera::Camera_Movement::FORWARD, deltaTime, ship);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        ship.move(GameObject::Ship_Movement::BACKWARD, deltaTime);
        camera.ProcessKeyboard(Camera::Camera_Movement::BACKWARD, deltaTime, ship);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        ship.move(GameObject::Ship_Movement::LEFT, deltaTime);
        camera.ProcessKeyboard(Camera::Camera_Movement::LEFT, deltaTime, ship);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        ship.move(GameObject::Ship_Movement::RIGHT, deltaTime);
        camera.ProcessKeyboard(Camera::Camera_Movement::RIGHT, deltaTime, ship);        
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}