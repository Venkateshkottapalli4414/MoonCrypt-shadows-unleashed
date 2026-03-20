#pragma once
#include <unordered_map>   // DSA: Hash Map — O(1) lookup
#include <string>
#include <iostream>

class Inventory {
private:
    std::unordered_map<std::string, int> items;  // item -> quantity

public:
    void addItem(const std::string& item, int qty = 1) {
        items[item] += qty;
        std::cout << "  [+] Added: " << item;
        if (qty > 1) std::cout << " x" << qty;
        std::cout << "\n";
    }

    bool removeItem(const std::string& item, int qty = 1) {
        auto it = items.find(item);
        if (it == items.end() || it->second < qty) return false;
        it->second -= qty;
        if (it->second == 0) items.erase(it);
        return true;
    }

    bool hasItem(const std::string& item) const {
        return items.count(item) && items.at(item) > 0;
    }

    int count(const std::string& item) const {
        auto it = items.find(item);
        return (it != items.end()) ? it->second : 0;
    }

    void display() const {
        if (items.empty()) {
            std::cout << "  [Inventory is empty]\n";
            return;
        }
        for (const auto& [item, qty] : items)
            std::cout << "  - " << item << " x" << qty << "\n";
    }

    bool isEmpty() const { return items.empty(); }
};
