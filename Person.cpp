#include "Person.h"

#include <utility>
#include "NameGenerator.h"

Person::Person(std::random_device &rng) : Person(generateName(rng), rng) {
}

Person::Person(FullName name, std::random_device &rng)
: name(name) {
    std::uniform_int_distribution<Stat> statGenerator(0, MAX_STAT_VALUE);
    hunger = statGenerator(rng);
    energy = statGenerator(rng);
}

void Person::advanceTime() {
    /* TODO: make thresholds and increases into constants or generalize stat updates
       into some mapping of stat to update behavior
    */
    if (activity == Activity::Eating && residence && residence->tryTransferInventory(ItemType::Food, 1))
    {
        hunger -= 5;
    }
    else
    {
        hunger++;
    }

    if (activity == Activity::Sleeping)
    {
        energy += 5;
    }
    else
    {
        energy--;
    }

    if (hunger > 200)
    {
        activity = Activity::Eating;
    }

    if (energy < 40)
    {
        activity = Activity::Sleeping;
    }
}

void Person::moveIn(Residence &newHome) {
    residence = &newHome;
}

void Person::moveOutOfResidence() {
    residence = nullptr;
}

