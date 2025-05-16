#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <memory>

#include "camera.hpp"   
#include "renderer.hpp"
#include "physicsengine.hpp"
#include "object.hpp"
#include "grid.hpp"
#include "inputhandler.hpp"
#include "constants.hpp"
#include "shaders.hpp"
#include "utils.hpp"
#include "../interfaces/ISimulationCallbacks.hpp"

class SimulationApp : public ISimulationCallbacks {
private:
    GLFWwindow* window;
    Camera camera;
    PhysicsEngine physics;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Grid> grid;
    std::vector<std::shared_ptr<Object>> objects;
    std::unique_ptr<InputHandler> inputHandler;
    
    float deltaTime;
    float lastFrame;
    bool running;

public:
    SimulationApp() 
        : window(nullptr),
          camera(400.0f, 300.0f),
          physics(),
          deltaTime(0.0f),
          lastFrame(0.0f),
          running(true) {}

    ~SimulationApp() {
        if (window) {
            glfwTerminate();
        }
    }

    bool initialize() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        
        // Create window
        window = glfwCreateWindow(800, 600, "Gravitational Simulation", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);
        
        // Initialize GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return false;
        }
        
        // Set up viewport
        glViewport(0, 0, 800, 600);
        
        // Create input handler
        inputHandler = std::make_unique<InputHandler>(
            camera, physics, objects, deltaTime, running, *this);
        
        // Set up callbacks
        glfwSetCursorPosCallback(window, InputHandler::mouseCallback);
        glfwSetScrollCallback(window, InputHandler::scrollCallback);
        glfwSetMouseButtonCallback(window, InputHandler::mouseButtonCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // Create renderer
        renderer = std::make_unique<Renderer>(800, 600);
        
        // Create grid
        grid = std::make_unique<Grid>();
        
        // Create initial objects
        createInitialObjects();
        
        return true;
    }

    void run() {
        while (!glfwWindowShouldClose(window) && running) {
            // Calculate delta time
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            // Process input
            inputHandler->processInput(window);
            
            // Update physics
            physics.update(objects, deltaTime);
            
            // Update grid
            grid->updateGrid(objects);
            
            // Render
            renderer->beginFrame();
            renderer->updateCamera(camera);
            renderer->render(objects, *grid);
            
            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    // ISimulationCallbacks implementation
    void createObject() override {
        objects.push_back(std::make_shared<Object>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            Constants::DEFAULT_MASS
        ));
        objects.back()->setInitializing(true);
    }

    void launchObject() override {
        if (!objects.empty()) {
            objects.back()->setInitializing(false);
            objects.back()->setLaunched(true);
        }
    }

    void processMouseMovement(double xpos, double ypos) override {
        camera.processMouseMovement(xpos, ypos);
    }

    void processScroll(double yoffset) override {
        camera.processScroll(yoffset, deltaTime);
    }

private:
    void createInitialObjects() {
        // Create initial celestial bodies
        objects.push_back(std::make_shared<Object>(
            glm::vec3(-5000, 650, -350),
            glm::vec3(30000, 15000, 0),
            5.97219 * pow(10, 22),
            5515,
            glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
        ));
        
        objects.push_back(std::make_shared<Object>(
            glm::vec3(5000, 650, -350),
            glm::vec3(15000, 30000, 0),
            5.97219 * pow(10, 22),
            5515,
            glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
        ));
        
        objects.push_back(std::make_shared<Object>(
            glm::vec3(0, 0, -350),
            glm::vec3(0, 0, 0),
            1.989 * pow(10, 25),
            8000,
            glm::vec4(1.0f, 0.929f, 0.176f, 1.0f),
            true // Glowing
        ));
    }
};
