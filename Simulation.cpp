//

#include "Simulation.h"
#include <utility>

static const unsigned int MAX_STARTING_FOOD = 100;

Simulation::Simulation(size_t population, size_t numHomes, std::random_device &rng)
: rng(rng)
, population(population)
, numHomes(numHomes)
, homes(numHomes)
{
    /* We were able to fill in homes using new[] to construct |numHomes| objects in a contiguous block of memory.
     * We can't do the same with Person, however, because new[] doesn't know how to make a Person. Unlike Residence,
     * which can be constructed with no constructor arguments, Persons must be a random number generator at a minimum.
     * new[] doesn't know how to pass arguments to constructors, so the compiler refuses to let us use it with
     * Person. Instead, we decouple the two responsibilities of new[] into separate steps. Instead of batch memory
     * allocation and batch construction, we do batch memory allocation and individual construction.
     */
    persons = personAllocator.allocate(population);
    for(size_t i = 0; i < population; i++) {
        personAllocator.construct(&persons[i], rng);
    }
    for(size_t i = 0; i < numHomes; i++) {
        homes[i] = new Residence();
        homes[i]->tryTransferInventory(ItemType::Food, rng() % MAX_STARTING_FOOD);
    }
    /* Things to consider:
     * - Is this easy for you to understand? How would you restructure this for more clarity?
     * - Right now, this code is responsible for both directions of the bidirectional link between Person and Residence.
     *   That leads to the possibility of messing things up by only linking in one direction if we're not careful.
     *   Should that responsibility instead belong to one or both of those classes to ensure bidirectional linking?
     */
    for(size_t homeIndex = 0, personIndex = 0; homeIndex < numHomes && personIndex < population; homeIndex++) {
        size_t numResidents = rng() % (population - personIndex - 1);
        for(size_t i = 0; i < numResidents; i++) {
            persons[personIndex].moveIn(*homes[homeIndex]);
            homes[homeIndex]->movePersonIn(persons[personIndex]);
            personIndex++;
        }
    }
}

Simulation::Simulation(const Simulation &other)
: rng(other.rng) {
    initializeFrom(other);
}

Simulation &Simulation::operator=(const Simulation &other) {
    if(this == &other) { return *this; }

    cleanUp();

    initializeFrom(other);

    return *this;
}

Simulation::Simulation(Simulation &&other) noexcept : rng(other.rng) {
    moveFrom(std::move(other));
}

Simulation &Simulation::operator=(Simulation &&other) noexcept {
    if(this == &other) { return *this; }

    cleanUp();

    moveFrom(std::move(other));

    return *this;
}

Simulation::~Simulation() {
    cleanUp();
}

void Simulation::advanceTime() {
    currentTime++;
    for(size_t i = 0; i < population; i++) {
        persons[i].advanceTime();
    }
    /* TODO: have a random chance to have a person leave our simulation and be replaced by a new person who moves
     * into a random home
     */
}

void Simulation::addResidence(unsigned int numResidents) {
    Residence *residence = new Residence();
    size_t nextUnhousedPersonIndex = 0;
    for(unsigned int i = 0; i < numResidents; i++) {
        while(nextUnhousedPersonIndex < population && persons[nextUnhousedPersonIndex].hasResidence()) {
            nextUnhousedPersonIndex++;
        }
        if(nextUnhousedPersonIndex < population) {
            residence->movePersonIn(persons[nextUnhousedPersonIndex]);
            persons[nextUnhousedPersonIndex].moveIn(*residence);
        }
    }
    residence->tryTransferInventory(ItemType::Food, rng() % MAX_STARTING_FOOD);

    homes.push_back(residence);
}

void Simulation::initializeFrom(const Simulation &other) {
    currentTime = other.currentTime;
    population = other.population;
    persons = personAllocator.allocate(population);
    for(size_t i = 0; i < population; i++) {
        personAllocator.construct(&persons[i], other.persons[i]);
    }
    numHomes = other.numHomes;
    for(size_t i = 0; i < numHomes; i++) {
        homes[i] = new Residence(*other.homes[i]);
    }
}

void Simulation::cleanUp() {
    // Since we used a special way to construct our Persons, we use the corresponding way to destruct persons
    for(size_t i = 0; i < population; i++) {
        personAllocator.destroy(&persons[i]);
    }
    // Since we used a special way to allocate space for our Persons, we use the corresponding way to free that space
    personAllocator.deallocate(persons, population);
    for(size_t i = 0; i < numHomes; i++) {
        delete homes[i];
    }
}

void Simulation::moveFrom(Simulation &&other) {
    currentTime = std::exchange(other.currentTime, 0);
    population = std::exchange(other.population, 0);
    persons = std::exchange(other.persons, nullptr);
    homes = std::move(other.homes);
    numHomes = std::exchange(other.numHomes, 0);
}



