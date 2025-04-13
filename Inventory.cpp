//

#include "Inventory.h"

unsigned int Inventory::getQuantity(ItemType itemType) const {
    auto item = items.find(itemType);
    if(item == items.end()) {
        return 0;
    }
    return item->second;
}

bool Inventory::tryAdjustQuantity(ItemType itemType, int delta) {
    auto item = items.find(itemType);
    if(item == items.end()) {
        if(delta > 0) {
            items[itemType] = delta;
            return true;
        }
    } else {
        int newQuantity = item->second + delta;
        if(newQuantity >= 0) {
            item->second = newQuantity;
            return true;
        }
    }
    return false;
}
