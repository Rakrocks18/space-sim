#pragma once

#include <GL/freeglut.h>

#include "../interfaces/IRenderer.hpp"

class GLRenderer : public IRenderer {
public:
    void renderBodies(const std::vector<std::shared_ptr<IBody>>& bodies) const override {
        for (auto& b : bodies) {
            Vector3 p = b->position();
            glPushMatrix();
                glTranslatef(p.x,p.y,p.z);
                float radius = std::cbrt(b->mass())*0.05f;
                glColor3f(1,0.5f,0.2f);
                glutSolidSphere(radius, 16, 16);
            glPopMatrix();
        }
    }

    void renderFabric() const override {
        // Fabric will be drawn by the IFabric instance
    }
};