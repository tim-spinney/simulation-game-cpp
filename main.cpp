#include "Simulation.h"

using namespace std;

int main() {
    for(int i = 0; i < 1000; i++) {
        std::random_device rng;
        Simulation simulation(10, 5, rng);
        for (int i = 0; i < 100; i++) {
            cout << i << endl;
            simulation.advanceTime();
            if (i % 10 == 9) {
                simulation.addResidence(1);
            }
        }
    }
    return 0;
}
