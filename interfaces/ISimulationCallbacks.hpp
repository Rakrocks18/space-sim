#pragma once

class ISimulationCallbacks {
public:
    virtual ~ISimulationCallbacks() = default;
    virtual void createObject() = 0;
    virtual void launchObject() = 0;
    virtual void processMouseMovement(double xpos, double ypos) = 0;
    virtual void processScroll(double yoffset) = 0;
};