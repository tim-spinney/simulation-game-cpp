#pragma once

#include <string>
#include <random>
#include <iostream>
#include "FullName.h"

enum Activity {
    Idle,
    Eating,
    Sleeping,
};

class Person {
public:
    Person(FullName name, std::random_device &rng);
    explicit Person(std::random_device &rng);
    void advanceTime();
private:
    typedef unsigned char Stat;
    static constexpr Stat MAX_STAT_VALUE = std::numeric_limits<Person::Stat>::max();

    FullName name;
    Stat hunger;
    Stat energy;
    Activity activity = Activity::Idle;
};
