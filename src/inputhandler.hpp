#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../interfaces/ISimulationCallbacks.hpp"

#include "camera.hpp"
#include "physicsengine.hpp"
#include "object.hpp"
#include "constants.hpp"

class InputHandler {
private:
    Camera& camera;
    PhysicsEngine& physics;
    std::vector<std::shared_ptr<Object>>& objects;
    float& deltaTime;
    bool& running;
    ISimulationCallbacks& callbacks;

public:
    InputHandler(Camera& camera, PhysicsEngine& physics, 
                 std::vector<std::shared_ptr<Object>>& objects, 
                 float& deltaTime, bool& running,
                 ISimulationCallbacks& callbacks)
        : camera(camera), physics(physics), objects(objects), 
          deltaTime(deltaTime), running(running), callbacks(callbacks) {}

    void processInput(GLFWwindow* window) {
        // Process keyboard input for camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_W, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_S, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_A, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_D, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_SPACE, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_LEFT_SHIFT, deltaTime);
            
        // Toggle pause
        static bool kKeyPressed = false;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            if (!kKeyPressed) {
                physics.setPaused(!physics.isPaused());
                kKeyPressed = true;
            }
        } else {
            kKeyPressed = false;
        }
        
        // Quit
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            running = false;
            glfwSetWindowShouldClose(window, true);
        }
        
        // Handle object initialization with arrow keys
        if (!objects.empty() && objects.back()->isInitializing()) {
            auto& obj = objects.back();
            float moveStep = obj->getRadius() * 0.2f;
            bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                if (!shiftPressed) {
                    obj->setPosition(obj->getPosition() + glm::vec3(0.0f, moveStep, 0.0f));
                } else {
                    obj->setPosition(obj->getPosition() + glm::vec3(0.0f, 0.0f, moveStep));
                }
            }
            
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                if (!shiftPressed) {
                    obj->setPosition(obj->getPosition() + glm::vec3(0.0f, -moveStep, 0.0f));
                } else {
                    obj->setPosition(obj->getPosition() + glm::vec3(0.0f, 0.0f, -moveStep));
                }
            }
            
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                obj->setPosition(obj->getPosition() + glm::vec3(moveStep, 0.0f, 0.0f));
            }
            
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                obj->setPosition(obj->getPosition() + glm::vec3(-moveStep, 0.0f, 0.0f));
            }
            
            // Increase mass when right mouse button is held
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                obj->increaseMass(1.0f + 1.0f * deltaTime);
            }
        }
    }

    // Static callback functions
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* callbacks = static_cast<ISimulationCallbacks*>(glfwGetWindowUserPointer(window));
        if (!callbacks) return;
        
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                callbacks->createObject();
            } else if (action == GLFW_RELEASE) {
                callbacks->launchObject();
            }
        }
    }

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* callbacks = static_cast<ISimulationCallbacks*>(glfwGetWindowUserPointer(window));
        if (!callbacks) return;
        
        callbacks->processMouseMovement(xpos, ypos);
    }

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* callbacks = static_cast<ISimulationCallbacks*>(glfwGetWindowUserPointer(window));
        if (!callbacks) return;
        
        callbacks->processScroll(yoffset);
    }
};