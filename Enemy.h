#pragma once
#include <string>

// Base class — Polymorphism
class Enemy {
public:
    std::string name;
    int hp, maxHp, atk, speed, xp;
    std::string icon;

    Enemy(std::string name, int hp, int atk, int speed, int xp, std::string icon)
        : name(name), hp(hp), maxHp(hp), atk(atk), speed(speed), xp(xp), icon(icon) {}

    virtual ~Enemy() = default;

    virtual std::string specialAttack() = 0;  // Pure virtual — overridden by each enemy
    bool isAlive() const { return hp > 0; }
};

// Enemy types — Inheritance
class Goblin : public Enemy {
public:
    Goblin() : Enemy("Goblin", 30, 8, 12, 20, "[G]") {}
    std::string specialAttack() override {
        return "The Goblin throws a poison dart! (-5 HP over 2 turns)";
    }
};

class Skeleton : public Enemy {
public:
    Skeleton() : Enemy("Skeleton", 45, 12, 8, 35, "[S]") {}
    std::string specialAttack() override {
        return "The Skeleton hurls a bone — stunning you for 1 turn!";
    }
};

class DarkKnight : public Enemy {
public:
    DarkKnight() : Enemy("Dark Knight", 65, 18, 7, 55, "[K]") {}
    std::string specialAttack() override {
        return "Dark Knight unleashes Shadow Cleave — dealing double damage!";
    }
};

class MoonWraith : public Enemy {
public:
    MoonWraith() : Enemy("MoonWraith [BOSS]", 130, 28, 15, 200, "[W]") {}
    std::string specialAttack() override {
        return "MoonWraith channels LUNAR WRATH — massive AoE strike!";
    }
};
