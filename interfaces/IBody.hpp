#pragma once

#include "../utils/vector3.hpp"


struct IBody {
    virtual ~IBody() {};
    virtual Vector3 position() const = 0;
    virtual Vector3 velocity() const = 0;
    virtual float mass() const = 0;

    virtual void applyForce(const Vector3& force, float dt) = 0;
    virtual void integrate(float dt) = 0;
};