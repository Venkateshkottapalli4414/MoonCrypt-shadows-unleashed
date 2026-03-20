#pragma once
#include <string>
#include <iostream>
#include "Inventory.h"

class Player {
public:
    std::string name;
    int hp, maxHp, atk, defense, speed;
    int level, xp, xpNext;
    int score;
    int currentRoom;
    Inventory inventory;

    Player(const std::string& name)
        : name(name), hp(100), maxHp(100), atk(15), defense(5),
          speed(10), level(1), xp(0), xpNext(100), score(0), currentRoom(0) {}

    bool isAlive() const { return hp > 0; }

    void takeDamage(int dmg) {
        int actual = std::max(1, dmg - defense / 2);
        hp = std::max(0, hp - actual);
        std::cout << "  You take " << actual << " damage! HP: " << hp << "/" << maxHp << "\n";
    }

    void heal(int amount) {
        hp = std::min(maxHp, hp + amount);
        std::cout << "  Healed " << amount << " HP! HP: " << hp << "/" << maxHp << "\n";
    }

    void gainXP(int amount) {
        xp += amount;
        score += amount;
        std::cout << "  +" << amount << " XP! [" << xp << "/" << xpNext << "]\n";
        if (xp >= xpNext) levelUp();
    }

    void levelUp() {
        level++;
        xp = 0;
        xpNext = static_cast<int>(xpNext * 1.5);
        atk += 4;
        defense += 2;
        maxHp += 25;
        hp = maxHp;
        std::cout << "\n  *** LEVEL UP! Now Level " << level << " ***\n";
        std::cout << "  ATK+" << 4 << " | DEF+" << 2 << " | MaxHP+" << 25 << " | HP Fully Restored!\n";
    }

    void showStats() const {
        std::cout << "\n========== [ " << name << " ] ==========\n";
        std::cout << "  HP    : " << hp << "/" << maxHp << "\n";
        std::cout << "  ATK   : " << atk << "  DEF: " << defense << "  SPD: " << speed << "\n";
        std::cout << "  Level : " << level << "  XP: " << xp << "/" << xpNext << "\n";
        std::cout << "  Score : " << score << "\n";
        std::cout << "==============================\n";
    }
};
