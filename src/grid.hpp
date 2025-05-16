#pragma once

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

#include "constants.hpp"
#include "../interfaces/IDrawable.hpp"
#include "object.hpp"
#include "utils.hpp"

class Grid : public IDrawable {
private:
    GLuint VAO, VBO;
    std::vector<float> vertices;
    float size;
    int divisions;
    glm::vec4 color;

public:
    Grid(float size = 20000.0f, int divisions = 25, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 0.25f))
        : size(size), divisions(divisions), color(color) {
        
        vertices = createGridVertices();
        Utils::createVBOVAO(VAO, VBO, vertices.data(), vertices.size());
    }

    ~Grid() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // IDrawable implementation
    void draw(const ShaderProgram& shader) const override {
        shader.setVec4("objectColor", color);
        shader.setBool("isGrid", true);
        shader.setBool("GLOW", false);
        
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        glBindVertexArray(0);
    }

    // Update grid vertices based on gravitational effects
    void updateGrid(const std::vector<std::shared_ptr<Object>>& objects) {
        // Calculate center of mass
        float totalMass = 0.0f;
        float comY = 0.0f;
        
        for (const auto& obj : objects) {
            if (obj->isInitializing()) continue;
            comY += obj->getMass() * obj->getPosition().y;
            totalMass += obj->getMass();
        }
        
        if (totalMass > 0) comY /= totalMass;
        
        // Find original max Y
        float originalMaxY = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < vertices.size(); i += 3) {
            originalMaxY = std::max(originalMaxY, vertices[i+1]);
        }
        
        float verticalShift = comY - originalMaxY;
        
        // Update vertices based on gravitational effects
        for (int i = 0; i < vertices.size(); i += 3) {
            glm::vec3 vertexPos(vertices[i], vertices[i+1], vertices[i+2]);
            glm::vec3 totalDisplacement(0.0f);
            
            for (const auto& obj : objects) {
                glm::vec3 toObject = obj->getPosition() - vertexPos;
                float distance = glm::length(toObject);
                float distance_m = distance * 1000.0f;
                float rs = (2 * Constants::G * obj->getMass()) / (Constants::C * Constants::C);
                
                float dz = 2 * sqrt(rs * (distance_m - rs));
                totalDisplacement.y += dz * 2.0f;
            }
            
            vertices[i+1] = totalDisplacement.y + -abs(verticalShift);
        }
        
        // Update VBO with new vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    }

private:
    std::vector<float> createGridVertices() const {
        std::vector<float> gridVertices;
        float step = size / divisions;
        float halfSize = size / 2.0f;
        
        // X axis lines
        for (int yStep = 3; yStep <= 3; ++yStep) {
            float y = -halfSize * 0.3f + yStep * step;
            for (int zStep = 0; zStep <= divisions; ++zStep) {
                float z = -halfSize + zStep * step;
                for (int xStep = 0; xStep < divisions; ++xStep) {
                    float xStart = -halfSize + xStep * step;
                    float xEnd = xStart + step;
                    gridVertices.insert(gridVertices.end(), {xStart, y, z, xEnd, y, z});
                }
            }
        }
        
        // Z axis lines
        for (int xStep = 0; xStep <= divisions; ++xStep) {
            float x = -halfSize + xStep * step;
            for (int yStep = 3; yStep <= 3; ++yStep) {
                float y = -halfSize * 0.3f + yStep * step;
                for (int zStep = 0; zStep < divisions; ++zStep) {
                    float zStart = -halfSize + zStep * step;
                    float zEnd = zStart + step;
                    gridVertices.insert(gridVertices.end(), {x, y, zStart, x, y, zEnd});
                }
            }
        }
        
        return gridVertices;
    }
};