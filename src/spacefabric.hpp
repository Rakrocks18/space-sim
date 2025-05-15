#pragma once

#include <algorithm>
#include <deque>
#include <GL/freeglut.h>

#include "../interfaces/IFabric.hpp"
#include "../interfaces/ISimulation.hpp"

class SpaceFabric : public IFabric {
    const ISimulation& sim;
    int    gridSize;
    float  spacing;
    float  c;              // signal speed
    float  smoothingRadius;// kernel support radius

    // history buffer per body
    struct BodyState {
        float t;
        Vector3 pos, vel;
    };
    mutable std::vector<std::deque<BodyState>> history;

public:
    SpaceFabric(const ISimulation& s,
                int N=20, float sp=1.0f,
                float signalSpeed = 0.1f,
                float smoothR = 1.5f)
      : sim(s)
      , gridSize(N)
      , spacing(sp)
      , c(signalSpeed)
      , smoothingRadius(smoothR)
    {
      history.resize(sim.bodies().size());
    }

    // call this right after sim.update(dt) each frame
    void recordHistory(float now) {
        const auto& bodies = sim.bodies();
        for (size_t i = 0; i < bodies.size(); ++i) {
            auto& buf = history[i];
            buf.push_back({ now,
                            bodies[i]->position(),
                            bodies[i]->velocity() });
            if (buf.size() > 5) buf.pop_front();
        }
    }

    // get estimated position of body i at retarded time via Hermite
    Vector3 getRetardedPos(size_t bi, const Vector3& P, float now) const {
        const auto& bodies = sim.bodies();
        auto& buf = history[bi];
        if (buf.size() < 2)
            return buf.back().pos;

        // approximate distance & retarded time
        float approxR = (bodies[bi]->position() - P).length();
        float t_ret = now - approxR / c;

        // find bracket in history
        size_t idx = 0;
        while (idx+1 < buf.size() && buf[idx+1].t < t_ret) ++idx;
        const auto& s0 = buf[idx];
        const auto& s1 = buf[ std::min(idx+1, buf.size()-1) ];

        float dt = s1.t - s0.t;
        float u = (dt > 1e-6f) ? (t_ret - s0.t) / dt : 0.0f;
        float u2 = u*u, u3 = u2*u;

        // Hermite basis
        float h00 =  2*u3 - 3*u2 + 1;
        float h10 =      u3 - 2*u2 + u;
        float h01 = -2*u3 + 3*u2;
        float h11 =      u3 -   u2;

        Vector3 m0 = s0.vel * dt;
        Vector3 m1 = s1.vel * dt;

        return s0.pos * h00
             + m0    * h10
             + s1.pos * h01
             + m1    * h11;
    }

    // cubic-Hermite smoothing kernel
    float kernel(float r) const {
        float q = r / smoothingRadius;
        if (q >= 1.0f) return 0.0f;
        return 1.0f - 3*q*q + 2*q*q*q;
    }

    void draw() const override {
        float now = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) * 0.001f;
        glBegin(GL_LINES);
        for (int i = -gridSize; i <= gridSize; ++i) {
            for (int j = -gridSize; j <= gridSize; ++j) {
                Vector3 p0(i*spacing, 0, j*spacing);
                Vector3 p1((i+1)*spacing, 0, j*spacing);
                Vector3 p2(i*spacing, 0, (j+1)*spacing);

                auto potentialAt = [&](const Vector3& P){
                    float pot = 0;
                    for (size_t bi=0; bi<sim.bodies().size(); ++bi) {
                        Vector3 preb = getRetardedPos(bi, P, now);
                        float r = (preb - P).length();
                        pot += sim.bodies()[bi]->mass() * kernel(r);
                    }
                    return -pot * 0.02f;
                };

                p0.y = potentialAt(p0);
                p1.y = potentialAt(p1);
                p2.y = potentialAt(p2);

                glColor3f(0.3f,0.3f,0.7f);
                // X line
                glVertex3f(p0.x,p0.y,p0.z);
                glVertex3f(p1.x,p1.y,p1.z);
                // Z line
                glVertex3f(p0.x,p0.y,p0.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }
        }
        glEnd();
    }
};