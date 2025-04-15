#pragma once
#include "Person.h"
#include <random>

class Simulation {
public:
    Simulation(size_t population, size_t numHomes, std::random_device &rng);
    ~Simulation();
    // TODO: copy operations
    void advanceTime();
    void addResidence(unsigned int numResidents);
private:
    std::random_device &rng;
    unsigned int currentTime = 0;
    size_t population;
    Person *persons;
    size_t numHomes;
    Residence *homes;
    std::allocator<Person> personAllocator;
};
