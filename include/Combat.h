#pragma once
#include <queue>
#include <utility>
#include <string>
#include <cstdlib>
#include <iostream>
#include "Player.h"
#include "Enemy.h"

class Combat {
public:
    // DSA: Priority Queue — determines turn order by speed (Max-Heap)
    static bool battle(Player& player, Enemy& enemy) {
        std::cout << "\n========== COMBAT ==========\n";
        std::cout << "  " << player.name << " vs " << enemy.name << "\n";
        std::cout << "============================\n";

        int specialChance = 0;

        while (player.isAlive() && enemy.isAlive()) {
            // Build turn order using Priority Queue
            std::priority_queue<std::pair<int, std::string>> turnOrder;
            int pSpd = player.speed + rand() % 5;
            int eSpd = enemy.speed + rand() % 5;
            turnOrder.push({pSpd, "player"});
            turnOrder.push({eSpd, "enemy"});

            std::cout << "\n  [Turn Order - Priority Queue]: ";
            std::cout << (pSpd >= eSpd ? player.name : enemy.name) << " goes first!\n";

            while (!turnOrder.empty()) {
                auto [spd, who] = turnOrder.top();
                turnOrder.pop();

                if (!player.isAlive() || !enemy.isAlive()) break;

                if (who == "player") {
                    int dmg = player.atk + rand() % 8 - 3;
                    dmg = std::max(1, dmg);
                    enemy.hp -= dmg;
                    std::cout << "  You strike " << enemy.name << " for " << dmg << " damage! "
                              << "(Enemy HP: " << std::max(0, enemy.hp) << ")\n";
                } else {
                    // Enemy special attack chance
                    specialChance++;
                    if (specialChance % 4 == 0) {
                        std::cout << "\n  " << enemy.specialAttack() << "\n";
                        int dmg = enemy.atk * 2;
                        player.takeDamage(dmg);
                    } else {
                        int dmg = enemy.atk + rand() % 6 - 2;
                        player.takeDamage(dmg);
                    }
                }
            }

            // Prompt player action
            if (player.isAlive() && enemy.isAlive()) {
                std::cout << "\n  [1] Attack  [2] Use Potion  [3] Flee\n  > ";
                int choice;
                std::cin >> choice;
                if (choice == 2) {
                    if (player.inventory.hasItem("Health Potion")) {
                        player.inventory.removeItem("Health Potion");
                        player.heal(35 + rand() % 20);
                    } else {
                        std::cout << "  No potions!\n";
                    }
                } else if (choice == 3) {
                    std::cout << "  You flee from battle!\n";
                    return false;
                }
            }
        }

        if (!enemy.isAlive()) {
            std::cout << "\n  *** " << enemy.name << " defeated! ***\n";
            player.gainXP(enemy.xp);
            // Random loot drop
            if (rand() % 2 == 0) {
                player.inventory.addItem("Health Potion");
                std::cout << "  Enemy dropped a Health Potion!\n";
            }
            return true;
        }
        return false;
    }
};
