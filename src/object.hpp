#pragma once

#include <vector>

#include "../interfaces/IPhysicsObject.hpp"
#include "../interfaces/IDrawable.hpp"

#include "./constants.hpp"
#include "../src/utils.hpp"

class Object : public IDrawable, public IPhysicsObject {
private:
    GLuint VAO, VBO;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 lastPosition;
    glm::vec4 color;
    float mass;
    float density;
    float radius;
    size_t vertexCount;
    bool initializing;
    bool launched;
    bool isGlowing;

public:
    Object(const glm::vec3& initPosition, 
           const glm::vec3& initVelocity, 
           float mass, 
           float density = 3344.0f, 
           const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
           bool glow = false) 
        : position(initPosition),
          velocity(initVelocity),
          lastPosition(initPosition),
          color(color),
          mass(mass),
          density(density),
          initializing(false),
          launched(false),
          isGlowing(glow) {
        
        updateRadius();
        std::vector<float> vertices = generateVertices();
        vertexCount = vertices.size();
        Utils::createVBOVAO(VAO, VBO, vertices.data(), vertexCount);
    }

    ~Object() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // IDrawable implementation
    void draw(const ShaderProgram& shader) const override {
        shader.setVec4("objectColor", color);
        shader.setBool("isGrid", false);
        shader.setBool("GLOW", isGlowing);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        shader.setMat4("model", model);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
        glBindVertexArray(0);
    }

    // IPhysicsObject implementation
    void updatePhysics(float deltaTime) override {
        if (!initializing) {
            position += velocity * (deltaTime / 94.0f);
        }
    }

    glm::vec3 getPosition() const override { return position; }
    glm::vec3 getVelocity() const override { return velocity; }
    float getMass() const override { return mass; }
    float getRadius() const override { return radius; }
    
    void setVelocity(const glm::vec3& vel) override { velocity = vel; }
    
    void accelerate(const glm::vec3& acc) override {
        velocity += acc / 96.0f;
    }

    // Object-specific methods
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setMass(float newMass) { 
        mass = newMass; 
        updateRadius();
        updateVertices();
    }
    
    void increaseMass(float factor) {
        mass *= factor;
        updateRadius();
        updateVertices();
    }
    
    bool isInitializing() const { return initializing; }
    void setInitializing(bool init) { initializing = init; }
    
    bool isLaunched() const { return launched; }
    void setLaunched(bool launch) { launched = launch; }
    
    const glm::vec4& getColor() const { return color; }
    
    float checkCollision(const Object& other) const {
        float distance = glm::length(other.position - position);
        if (other.radius + radius > distance) {
            return -0.2f; // Collision occurred, return bounce factor
        }
        return 1.0f; // No collision
    }

private:
    void updateRadius() {
        radius = pow(((3.0f * mass / density) / (4.0f * Constants::PI)), (1.0f/3.0f)) / Constants::SIZE_RATIO;
    }
    
    void updateVertices() {
        std::vector<float> vertices = generateVertices();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    }
    
    std::vector<float> generateVertices() const {
        std::vector<float> vertices;
        int stacks = 10;
        int sectors = 10;

        // Generate sphere vertices
        for (float i = 0.0f; i <= stacks; ++i) {
            float theta1 = (i / stacks) * glm::pi<float>();
            float theta2 = (i+1) / stacks * glm::pi<float>();
            
            for (float j = 0.0f; j < sectors; ++j) {
                float phi1 = j / sectors * 2 * glm::pi<float>();
                float phi2 = (j+1) / sectors * 2 * glm::pi<float>();
                
                glm::vec3 v1 = Utils::sphericalToCartesian(radius, theta1, phi1);
                glm::vec3 v2 = Utils::sphericalToCartesian(radius, theta1, phi2);
                glm::vec3 v3 = Utils::sphericalToCartesian(radius, theta2, phi1);
                glm::vec3 v4 = Utils::sphericalToCartesian(radius, theta2, phi2);

                // Triangle 1: v1-v2-v3
                vertices.insert(vertices.end(), {v1.x, v1.y, v1.z});
                vertices.insert(vertices.end(), {v2.x, v2.y, v2.z});
                vertices.insert(vertices.end(), {v3.x, v3.y, v3.z});
                
                // Triangle 2: v2-v4-v3
                vertices.insert(vertices.end(), {v2.x, v2.y, v2.z});
                vertices.insert(vertices.end(), {v4.x, v4.y, v4.z});
                vertices.insert(vertices.end(), {v3.x, v3.y, v3.z});
            }
        }
        
        return vertices;
    }
};
