#pragma once

#include "spacefabric.hpp"
#include "gravitysimulation.hpp"
#include "glrenderer.hpp"

class Application {
    std::unique_ptr<ISimulation> sim;
    std::unique_ptr<IFabric>     fabric;
    std::unique_ptr<IRenderer>   renderer;
    float lastTime;
public:
    Application()
      : sim(new GravitySimulation()),
        fabric(new SpaceFabric(*sim)),
        renderer(new GLRenderer()),
        lastTime(0.0f)
    {}

    void display() {
        float now = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
        float dt  = now - lastTime;
        lastTime = now;

        sim->update(dt);

        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // simple camera
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60,1,0.1,100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(3,3,3, 0,0,0, 0,1,0);

        // draw fabric & bodies
        fabric->draw();
        renderer->renderBodies(sim->bodies());

        glutSwapBuffers();
        glutPostRedisplay();
    }

    void run(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(800,800);
        glutCreateWindow("3D Gravity & Space-Time Fabric");

        glutDisplayFunc([](){ get()->display(); });
        lastTime = glutGet(GLUT_ELAPSED_TIME)*0.001f;
        glutMainLoop();
    }

    // Singleton accessor for the lambda callback
    static Application* get() {
            static Application app;
            return &app;
    }
};