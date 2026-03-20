#pragma once
#include <string>
#include <vector>

enum class RoomType { ENTRANCE, EMPTY, ENEMY, LOOT, BOSS };

struct Room {
    int id;
    RoomType type;
    std::vector<int> neighbors;  // Graph: adjacency list
    bool visited;
    bool cleared;
    std::string loot;

    Room(int id, RoomType type)
        : id(id), type(type), visited(false), cleared(false) {}

    std::string typeName() const {
        switch(type) {
            case RoomType::ENTRANCE: return "ENTRANCE";
            case RoomType::ENEMY:    return "ENEMY";
            case RoomType::LOOT:     return "LOOT";
            case RoomType::BOSS:     return "BOSS";
            default:                 return "EMPTY";
        }
    }
};
