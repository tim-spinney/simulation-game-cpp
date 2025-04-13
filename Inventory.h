#pragma once

#include <unordered_map>

enum ItemType {
    Food
};

class Inventory {
public:
    unsigned int getQuantity(ItemType) const;
    bool tryAdjustQuantity(ItemType, int);
private:
    std::unordered_map<ItemType, int> items;
};
