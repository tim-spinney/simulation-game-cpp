#include "Simulation.h"

using namespace std;

int main() {
    std::random_device rng;
    Simulation simulation(10, 5, rng);
    for(int i = 0; i < 100; i++) {
        simulation.advanceTime();
    }
    return 0;
}
