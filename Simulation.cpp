//

#include "Simulation.h"

Simulation::Simulation(size_t population, std::random_device &rng)
: population(population)
{
    std::allocator<Person> allocator;
    persons = allocator.allocate(population);
    for(size_t i = 0; i < population; i++) {
        allocator.construct(&persons[i], rng);
    }
}

Simulation::~Simulation() {
    std::allocator<Person> allocator;
    for(size_t i = 0; i < population; i++) {
        allocator.destroy(&persons[i]);
    }
    allocator.deallocate(persons, population);
}

void Simulation::advanceTime() {
    currentTime++;
    for(size_t i = 0; i < population; i++) {
        persons[i].advanceTime();
    }
}

