#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "constants.hpp"
#include "object.hpp"

class PhysicsEngine {
private:
    bool paused;

public:
    PhysicsEngine() : paused(true) {}

    void setPaused(bool pause) { paused = pause; }
    bool isPaused() const { return paused; }

    void update(std::vector<std::shared_ptr<Object>>& objects, float deltaTime) {
        if (paused) return;
        
        // Apply gravitational forces between objects
        for (size_t i = 0; i < objects.size(); ++i) {
            auto& obj1 = objects[i];
            
            // Skip objects that are being initialized
            if (obj1->isInitializing()) continue;
            
            // Update position based on velocity
            obj1->updatePhysics(deltaTime);
            
            // Apply gravitational forces from other objects
            for (size_t j = 0; j < objects.size(); ++j) {
                if (i == j) continue;
                
                auto& obj2 = objects[j];
                if (obj2->isInitializing()) continue;
                
                applyGravitationalForce(obj1, obj2);
                
                // Check for collisions
                float collisionFactor = obj1->checkCollision(*obj2);
                if (collisionFactor < 1.0f) {
                    obj1->setVelocity(obj1->getVelocity() * collisionFactor);
                }
            }
        }
    }

private:
    void applyGravitationalForce(std::shared_ptr<Object>& obj1, std::shared_ptr<Object>& obj2) {
        glm::vec3 direction = obj2->getPosition() - obj1->getPosition();
        float distance = glm::length(direction);
        
        if (distance > 0) {
            direction = glm::normalize(direction);
            distance *= 1000.0f; // Convert to meters
            
            // Calculate gravitational force
            double force = (Constants::G * obj1->getMass() * obj2->getMass()) / (distance * distance);
            float acceleration = force / obj1->getMass();
            
            // Apply acceleration
            obj1->accelerate(direction * acceleration);
        }
    }
};