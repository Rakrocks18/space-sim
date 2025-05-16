#include "simulation_app.hpp"

int main() {
    SimulationApp app;
    
    if (!app.initialize()) {
        return -1;
    }
    
    app.run();
    
    return 0;
}