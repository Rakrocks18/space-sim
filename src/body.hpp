#pragma once

#include "../interfaces/IBody.hpp"

class Body : public IBody {
    Vector3 _position, velocity;
    float _mass;

public:
    Body(const Vector3& position, const Vector3& velocity, float mass)
        : _position(position), velocity(velocity), _mass(mass) {}

    inline Vector3 position() const override { return _position; }
    inline Vector3 velocity() const override { return velocity; }
    inline float mass() const override { return _mass; }

    void applyForce(const Vector3& force, float dt) override {
        velocity += (force * (dt / _mass));
    }

    void integrate(float dt) override {
        _position += (velocity * dt);
    }
};