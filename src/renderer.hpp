#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../interfaces/IDrawable.hpp"
#include "shaderprogram.hpp"
#include "grid.hpp"
#include "object.hpp"
#include "shaders.hpp"


class Renderer {
private:
    ShaderProgram shader;
    glm::mat4 projection;

public:
    Renderer(int width, int height) 
        : shader(Shaders::vertexShaderSource, Shaders::fragmentShaderSource) {
        
        // Set up projection matrix
        projection = glm::perspective(glm::radians(45.0f), 
                                     static_cast<float>(width) / static_cast<float>(height), 
                                     0.1f, 750000.0f);
        
        // Set up OpenGL state
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void beginFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("projection", projection);
    }

    void updateCamera(const Camera& camera) {
        shader.setMat4("view", camera.getViewMatrix());
    }

    void render(const IDrawable& drawable) {
        drawable.draw(shader);
    }

    void render(const std::vector<std::shared_ptr<Object>>& objects, const Grid& grid) {
        // Draw grid
        render(grid);
        
        // Draw objects
        for (const auto& obj : objects) {
            render(*obj);
        }
    }
};