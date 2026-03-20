# 🌙 MoonCrypt: Shadows Unleashed

A DSA-powered dungeon crawler RPG built in C++.

## How to Compile & Run

```bash
# Using Makefile
make run

# Or manually
g++ -std=c++17 -Wall main.cpp -o MoonCrypt
./MoonCrypt
```

## DSA Concepts Used

| Concept | Where Used |
|---|---|
| Graph (Adjacency List) | Dungeon map — rooms & connections |
| BFS | Enemy AI & shortest path display |
| Dijkstra's Algorithm | Weighted shortest path to boss |
| Hash Map (unordered_map) | Room storage & Inventory system |
| Priority Queue (Max-Heap) | Combat turn order by speed |
| Stack | Quest log (LIFO events) |
| Queue | Event processing system |
| Min-Heap | Leaderboard top-5 scores |

## Project Structure

```
MoonCrypt/
├── main.cpp           ← Game loop
├── Makefile
├── data/
│   └── scores.txt     ← Leaderboard (auto-created)
└── include/
    ├── Room.h         ← Graph node
    ├── Enemy.h        ← Base class + polymorphism
    ├── Player.h       ← Player stats & actions
    ├── Inventory.h    ← Hash Map inventory
    ├── Dungeon.h      ← Graph + BFS + Dijkstra
    ├── Combat.h       ← Priority Queue combat
    └── Leaderboard.h  ← Min-Heap scores
```

## Gameplay

- Navigate 10 rooms connected as a **graph**
- Fight enemies using turn-based **priority queue** combat
- Collect loot stored in a **hash map** inventory
- Track events via **stack** (quest log) and **queue** (events)
- Defeat the **MoonWraith** boss to win!
