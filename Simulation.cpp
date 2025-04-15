//

#include "Simulation.h"

static const unsigned int MAX_STARTING_FOOD = 100;

Simulation::Simulation(size_t population, size_t numHomes, std::random_device &rng)
: rng(rng)
, population(population)
, numHomes(numHomes)
, homes(new Residence[numHomes])
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
    /* Things to consider:
     * - Is this easy for you to understand? How would you restructure this for more clarity?
     * - Right now, this code is responsible for both directions of the bidirectional link between Person and Residence.
     *   That leads to the possibility of messing things up by only linking in one direction if we're not careful.
     *   Should that responsibility instead belong to one or both of those classes to ensure bidirectional linking?
     */
    for(size_t homeIndex = 0, personIndex = 0; homeIndex < numHomes && personIndex < population; homeIndex++) {
        size_t numResidents = rng() % (population - personIndex - 1);
        for(size_t i = 0; i < numResidents; i++) {
            persons[personIndex].moveIn(homes[homeIndex]);
            homes[homeIndex].movePersonIn(persons[personIndex]);
            personIndex++;
        }
    }
    for(size_t homeIndex = 0; homeIndex < numHomes; homeIndex++) {
        homes[homeIndex].tryTransferInventory(ItemType::Food, rng() % MAX_STARTING_FOOD);
    }
}

Simulation::~Simulation() {
    // Since we used a special way to construct our Persons, we use the corresponding way to destruct persons
    for(size_t i = 0; i < population; i++) {
        personAllocator.destroy(&persons[i]);
    }
    // Since we used a special way to allocate space for our Persons, we use the corresponding way to free that space
    personAllocator.deallocate(persons, population);
    delete []homes;
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
    Residence residence;
    size_t nextUnhousedPersonIndex = 0;
    for(unsigned int i = 0; i < numResidents; i++) {
        while(nextUnhousedPersonIndex < population && persons[nextUnhousedPersonIndex].hasResidence()) {
            nextUnhousedPersonIndex++;
        }
        if(nextUnhousedPersonIndex < population) {
            residence.movePersonIn(persons[nextUnhousedPersonIndex]);
            persons[nextUnhousedPersonIndex].moveIn(residence);
        }
    }
    residence.tryTransferInventory(ItemType::Food, rng() % MAX_STARTING_FOOD);

    Residence *newHomes = new Residence[numHomes+1];
    for(size_t i = 0; i < numHomes; i++) {
        newHomes[i] = homes[i];
    }
    newHomes[numHomes] = std::move(residence);
    numHomes++;
    delete []homes;
    homes = newHomes;
}

