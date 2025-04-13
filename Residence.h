#pragma once

#include "Inventory.h"
#include <unordered_set>

class Person;

class Residence {
public:
    void movePersonIn(Person &);
    void movePersonOut(Person &);
    bool tryTransferInventory(ItemType, int);
private:
    std::unordered_set<Person*> residents;
    Inventory inventory;
};
