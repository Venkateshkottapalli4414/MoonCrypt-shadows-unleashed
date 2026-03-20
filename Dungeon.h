#pragma once
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <iostream>
#include <memory>
#include <cstdlib>
#include "Room.h"
#include "Enemy.h"

class Dungeon {
private:
    std::unordered_map<int, Room> rooms;   // DSA: Hash Map for room storage
    int numRooms;

    std::unique_ptr<Enemy> makeEnemy(int roomId) {
        int r = roomId % 3;
        if (r == 0) return std::make_unique<Goblin>();
        if (r == 1) return std::make_unique<Skeleton>();
        return std::make_unique<DarkKnight>();
    }

public:
    std::unordered_map<int, std::unique_ptr<Enemy>> enemies;  // room -> enemy

    Dungeon(int n = 10) : numRooms(n) { generate(); }

    void generate() {
        rooms.clear();
        enemies.clear();

        // Create rooms
        for (int i = 0; i < numRooms; i++) {
            RoomType type;
            if (i == 0)             type = RoomType::ENTRANCE;
            else if (i == numRooms-1) type = RoomType::BOSS;
            else if (i % 3 == 1)   type = RoomType::ENEMY;
            else if (i % 3 == 2)   type = RoomType::LOOT;
            else                    type = RoomType::EMPTY;

            rooms.emplace(i, Room(i, type));
        }

        // Set loot for loot rooms
        std::vector<std::string> lootPool = {
            "Health Potion", "Moon Shard", "Iron Shield",
            "Mystic Scroll", "Shadow Blade", "Ancient Rune"
        };
        for (int i = 0; i < numRooms; i++) {
            if (rooms.at(i).type == RoomType::LOOT)
                rooms.at(i).loot = lootPool[i % lootPool.size()];
        }

        // Connect rooms — DSA: Graph (Adjacency List)
        for (int i = 0; i < numRooms - 1; i++) {
            rooms.at(i).neighbors.push_back(i + 1);
            rooms.at(i + 1).neighbors.push_back(i);
        }
        // Extra edges for non-linear exploration
        if (numRooms > 4) { rooms.at(0).neighbors.push_back(3); rooms.at(3).neighbors.push_back(0); }
        if (numRooms > 6) { rooms.at(2).neighbors.push_back(5); rooms.at(5).neighbors.push_back(2); }

        // Place enemies
        for (int i = 0; i < numRooms; i++) {
            if (rooms.at(i).type == RoomType::ENEMY)
                enemies[i] = makeEnemy(i);
            else if (rooms.at(i).type == RoomType::BOSS)
                enemies[i] = std::make_unique<MoonWraith>();
        }

        rooms.at(0).visited = true;
    }

    Room& getRoom(int id) { return rooms.at(id); }
    bool roomExists(int id) const { return rooms.count(id) > 0; }
    int size() const { return numRooms; }

    // DSA: BFS Pathfinding — enemy AI / shortest path display
    std::vector<int> bfs(int start, int end) {
        std::unordered_map<int, int> prev;
        std::unordered_map<int, bool> visited;
        std::queue<int> q;   // DSA: Queue

        q.push(start);
        visited[start] = true;
        prev[start] = -1;

        while (!q.empty()) {
            int cur = q.front(); q.pop();
            if (cur == end) break;
            for (int nb : rooms.at(cur).neighbors) {
                if (!visited[nb]) {
                    visited[nb] = true;
                    prev[nb] = cur;
                    q.push(nb);
                }
            }
        }

        std::vector<int> path;
        for (int at = end; at != -1; at = prev.count(at) ? prev[at] : -1)
            path.insert(path.begin(), at);
        return path;
    }

    // DSA: Dijkstra's Algorithm — boss pathfinding (weighted)
    std::vector<int> dijkstra(int start, int end) {
        const int INF = std::numeric_limits<int>::max();
        std::unordered_map<int, int> dist;
        std::unordered_map<int, int> prev;

        // DSA: Min-Heap priority queue {dist, node}
        std::priority_queue<std::pair<int,int>,
            std::vector<std::pair<int,int>>,
            std::greater<>> pq;

        for (int i = 0; i < numRooms; i++) dist[i] = INF;
        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (int v : rooms.at(u).neighbors) {
                int w = 1 + (rooms.at(v).type == RoomType::ENEMY ? 2 : 0);
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        std::vector<int> path;
        for (int at = end; prev.count(at); at = prev[at])
            path.insert(path.begin(), at);
        if (!path.empty()) path.insert(path.begin(), start);
        return path;
    }

    void displayMap(int currentRoom) {
        std::cout << "\n--- Dungeon Map (Graph - Adjacency List) ---\n";
        for (int i = 0; i < numRooms; i++) {
            const Room& r = rooms.at(i);
            std::string marker = (i == currentRoom) ? ">>>" : "   ";
            std::string status = r.cleared ? "[X]" : (r.visited ? "[?]" : "[ ]");
            std::cout << marker << " Room " << i << " " << status
                      << " [" << r.typeName() << "]";
            if (!r.neighbors.empty()) {
                std::cout << "  -->  ";
                for (int nb : r.neighbors) std::cout << nb << " ";
            }
            std::cout << "\n";
        }
        std::cout << "--------------------------------------------\n";
    }
};
