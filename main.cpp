#include <iostream>
#include <stack>       // DSA: Stack — quest log
#include <queue>       // DSA: Queue — event queue
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

#include "include/Player.h"
#include "include/Dungeon.h"
#include "include/Combat.h"
#include "include/Leaderboard.h"

// ============================================================
//  🌙 MoonCrypt: Shadows Unleashed
//  DSA: Graph, BFS, Dijkstra, Hash Map, Stack, Queue,
//       Priority Queue, Min-Heap, Binary Tree (serialization)
// ============================================================

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printTitle() {
    std::cout << R"(
  ╔══════════════════════════════════════════════╗
  ║     🌙  MoonCrypt: Shadows Unleashed  🌙     ║
  ║         A DSA-Powered Dungeon Crawler        ║
  ╚══════════════════════════════════════════════╝
)" << "\n";
}

void printDivider() { std::cout << "\n--------------------------------------------\n"; }

// DSA: Stack — quest log (LIFO)
void logEvent(std::stack<std::string>& log, const std::string& event) {
    log.push(event);
    std::cout << "  [LOG] " << event << "\n";
}

void showQuestLog(std::stack<std::string> log) {
    std::cout << "\n--- Quest Log (Stack - Last 5 events) ---\n";
    int count = 0;
    while (!log.empty() && count < 5) {
        std::cout << "  > " << log.top() << "\n";
        log.pop();
        count++;
    }
    printDivider();
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    printTitle();

    // ---- Setup ----
    std::string playerName;
    std::cout << "  Enter your name, brave adventurer: ";
    std::getline(std::cin, playerName);
    if (playerName.empty()) playerName = "Hero";

    Player player(playerName);
    Dungeon dungeon(10);
    Leaderboard leaderboard;
    leaderboard.load();

    std::stack<std::string> questLog;   // DSA: Stack
    std::queue<std::string> eventQueue; // DSA: Queue

    player.inventory.addItem("Health Potion", 2);

    // ---- Game Loop ----
    bool running = true;
    bool won = false;

    std::cout << "\n  The gates of MoonCrypt creak open...\n";
    std::cout << "  You step into the darkness. There is no turning back.\n\n";

    logEvent(questLog, "Entered MoonCrypt");
    eventQueue.push("The dungeon breathes with dark energy...");

    while (running && player.isAlive()) {

        // Process event queue
        while (!eventQueue.empty()) {
            std::cout << "\n  [EVENT] " << eventQueue.front() << "\n";
            eventQueue.pop();
        }

        int cur = player.currentRoom;
        Room& room = dungeon.getRoom(cur);

        printDivider();
        dungeon.displayMap(cur);

        // BFS path to boss
        auto bfsPath = dungeon.bfs(cur, dungeon.size() - 1);
        std::cout << "  BFS path to Boss: ";
        for (int r : bfsPath) std::cout << r << " ";
        std::cout << "\n";

        // Dijkstra path to boss
        auto dPath = dungeon.dijkstra(cur, dungeon.size() - 1);
        std::cout << "  Dijkstra (weighted) path: ";
        for (int r : dPath) std::cout << r << " ";
        std::cout << "\n";

        printDivider();
        std::cout << "\n  Current Room: " << cur << " [" << room.typeName() << "]\n";

        // Room events
        if (room.type == RoomType::ENEMY && !room.cleared) {
            auto& enemy = dungeon.enemies.at(cur);
            std::cout << "\n  A " << enemy->name << " blocks your path!\n";
            std::cout << "  [1] Fight  [2] View Stats  [3] Inventory  [4] Quest Log\n  > ";
            int ch; std::cin >> ch;
            if (ch == 1) {
                bool result = Combat::battle(player, *enemy);
                if (result) {
                    room.cleared = true;
                    logEvent(questLog, "Defeated " + enemy->name + " in Room " + std::to_string(cur));
                    eventQueue.push("Victory echoes through the dungeon!");
                } else if (!player.isAlive()) break;
            } else if (ch == 2) {
                player.showStats();
            } else if (ch == 3) {
                std::cout << "\n  [Inventory - Hash Map]\n";
                player.inventory.display();
            } else if (ch == 4) {
                showQuestLog(questLog);
            }
            clearInput();
            continue;

        } else if (room.type == RoomType::BOSS && !room.cleared) {
            auto& boss = dungeon.enemies.at(cur);
            std::cout << "\n  *** THE MOONWRAITH AWAITS ***\n";
            std::cout << "  " << boss->name << " (HP: " << boss->hp << " | ATK: " << boss->atk << ")\n";
            std::cout << "  [1] Face the MoonWraith!  [2] Retreat\n  > ";
            int ch; std::cin >> ch;
            clearInput();
            if (ch == 1) {
                bool result = Combat::battle(player, *boss);
                if (result) {
                    room.cleared = true;
                    won = true;
                    running = false;
                    logEvent(questLog, "DEFEATED THE MOONWRAITH!");
                } else if (!player.isAlive()) break;
            } else {
                std::cout << "  You step back from the chamber...\n";
            }
            continue;

        } else if (room.type == RoomType::LOOT && !room.cleared) {
            std::cout << "\n  A glowing chest! [" << room.loot << "]\n";
            std::cout << "  [1] Take loot  [2] Leave it\n  > ";
            int ch; std::cin >> ch;
            clearInput();
            if (ch == 1) {
                player.inventory.addItem(room.loot);
                // Special item effects
                if (room.loot == "Moon Shard") {
                    player.atk += 5;
                    std::cout << "  Moon Shard empowers you! ATK+5\n";
                } else if (room.loot == "Iron Shield") {
                    player.defense += 5;
                    std::cout << "  Iron Shield equipped! DEF+5\n";
                }
                room.cleared = true;
                logEvent(questLog, "Found " + room.loot + " in Room " + std::to_string(cur));
                player.score += 10;
            }
            continue;
        }

        // Movement / actions menu
        std::cout << "\n  ACTIONS:\n";
        std::cout << "  [1] Move  [2] Stats  [3] Inventory  [4] Use Potion  [5] Quest Log  [0] Quit\n  > ";

        int action; std::cin >> action; clearInput();

        if (action == 1) {
            std::cout << "\n  Available rooms: ";
            for (int nb : room.neighbors) std::cout << nb << " ";
            std::cout << "\n  Move to: ";
            int dest; std::cin >> dest; clearInput();
            if (dungeon.roomExists(dest) &&
                std::find(room.neighbors.begin(), room.neighbors.end(), dest) != room.neighbors.end()) {
                player.currentRoom = dest;
                dungeon.getRoom(dest).visited = true;
                logEvent(questLog, "Moved to Room " + std::to_string(dest));
                eventQueue.push("You entered Room " + std::to_string(dest) + "...");
            } else {
                std::cout << "  Invalid room!\n";
            }

        } else if (action == 2) {
            player.showStats();

        } else if (action == 3) {
            std::cout << "\n  [Inventory - Hash Map O(1) lookup]\n";
            player.inventory.display();

        } else if (action == 4) {
            if (player.inventory.hasItem("Health Potion")) {
                player.inventory.removeItem("Health Potion");
                player.heal(35 + rand() % 15);
                logEvent(questLog, "Used Health Potion");
            } else {
                std::cout << "  No Health Potions!\n";
            }

        } else if (action == 5) {
            showQuestLog(questLog);

        } else if (action == 0) {
            std::cout << "  Leaving MoonCrypt...\n";
            running = false;
        }
    }

    // ---- Game Over ----
    printDivider();
    if (won) {
        std::cout << R"(
  ╔══════════════════════════════════════╗
  ║   🏆  VICTORY! MoonWraith Slain!    ║
  ║   MoonCrypt is cleansed. You are    ║
  ║        legend among legends.        ║
  ╚══════════════════════════════════════╝
)";
    } else {
        std::cout << R"(
  ╔══════════════════════════════════════╗
  ║   💀  GAME OVER                     ║
  ║   The shadows consumed you.         ║
  ╚══════════════════════════════════════╝
)";
    }

    player.showStats();
    leaderboard.addScore(player.name, player.score);
    leaderboard.save();
    leaderboard.display();

    std::cout << "\n  Thanks for playing MoonCrypt: Shadows Unleashed!\n\n";
    return 0;
}
