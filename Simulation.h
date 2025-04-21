#pragma once
#include "Person.h"
#include <random>

class Simulation {
public:
    Simulation(size_t population, size_t numHomes, std::random_device &rng);
    ~Simulation();
    Simulation(const Simulation&);
    Simulation &operator=(const Simulation&);
    Simulation(Simulation&&) noexcept;
    Simulation &operator=(Simulation&&) noexcept;
    // TODO: copy operations
    void advanceTime();
    void addResidence(unsigned int numResidents);
private:
    std::random_device &rng;
    unsigned int currentTime = 0;
    size_t population;
    Person *persons;
    size_t numHomes;
    std::vector<Residence*> homes;
    std::allocator<Person> personAllocator;
    void initializeFrom(const Simulation&);
    void moveFrom(Simulation&&);
    void cleanUp();
};
