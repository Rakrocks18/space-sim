#pragma once

#include "../interfaces/ISimulation.hpp"
#include "body.hpp"

class GravitySimulation : public ISimulation {
    std::vector<std::shared_ptr<IBody>> _bodies;
    float G = 6.67430e-2f; // Gravitational constant

public:
    GravitySimulation () {
        _bodies.push_back(std::make_shared<Body>(Vector3(-1,0,0), Vector3(0,0.02f,0), 20.0f));
        _bodies.push_back(std::make_shared<Body>(Vector3( 1,0,0), Vector3(0,-0.02f,0), 20.0f));
        _bodies.push_back(std::make_shared<Body>(Vector3( 0,1,0), Vector3(-0.02f,0,0), 20.0f));
    }

    const std::vector<std::shared_ptr<IBody>>& bodies() const override {
        return _bodies;
    }

    void update(float dt) override {
        
        std::vector<Vector3> forces(_bodies.size(), Vector3());

        for(size_t i = 0; i < _bodies.size(); ++i) {
            for (size_t j=i+1; j < _bodies.size(); ++j)  {
                Vector3 dir = _bodies[j]->position() - _bodies[i]->position();
                float distance = dir.length() + 1e-10f; // Avoid division by zero
                float Fmag = G * _bodies[i]->mass() * _bodies[j]->mass() / (distance * distance);
                Vector3 force = dir.normalize() * Fmag;
                forces[i] += force;
                forces[j] -= force; // Action-reaction pair
            }
        }

        for (size_t i = 0; i < _bodies.size(); ++i) {
            _bodies[i]->applyForce(forces[i], dt);
            _bodies[i]->integrate(dt);
        }
    }
};
