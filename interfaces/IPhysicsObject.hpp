#pragma once

#include <glm/glm.hpp>

class IPhysicsObject {
public:
    virtual ~IPhysicsObject() = default;
    virtual void updatePhysics(float deltaTime) = 0;
    virtual glm::vec3 getPosition() const = 0;
    virtual glm::vec3 getVelocity() const = 0;
    virtual float getMass() const = 0;
    virtual float getRadius() const = 0;
    virtual void setVelocity(const glm::vec3& vel) = 0;
    virtual void accelerate(const glm::vec3& acc) = 0;
};