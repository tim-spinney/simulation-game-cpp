#pragma once
#include "Person.h"
#include <random>

class Simulation {
public:
    Simulation(size_t population, std::random_device &rng);
    ~Simulation();
    void advanceTime();
private:
    unsigned int currentTime = 0;
    size_t population;
    Person *persons;
    std::allocator<Person> allocator;
};
