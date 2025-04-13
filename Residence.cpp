//

#include "Residence.h"

void Residence::movePersonIn(Person &person) {
    residents.insert(&person);
}

void Residence::movePersonOut(Person &person) {
    residents.erase(&person);
}

bool Residence::tryTransferInventory(ItemType itemType, int quantity) {
    return inventory.tryAdjustQuantity(itemType, quantity);
}
