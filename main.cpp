#include <iostream>
#include "Simulation.h"

int main() {
    std::random_device rng;
    Simulation simulation(4, rng);
    for(int i = 0; i < 10; i++) {
        simulation.advanceTime();
    }
    return 0;
}
