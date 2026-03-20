#pragma once
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct ScoreEntry {
    int score;
    std::string name;
    bool operator>(const ScoreEntry& o) const { return score > o.score; }
};

class Leaderboard {
private:
    // DSA: Min-Heap — keeps top 5 scores efficiently
    std::priority_queue<ScoreEntry,
        std::vector<ScoreEntry>,
        std::greater<ScoreEntry>> minHeap;

    const int MAX_ENTRIES = 5;
    const std::string FILE_PATH = "data/scores.txt";

public:
    void load() {
        std::ifstream file(FILE_PATH);
        if (!file.is_open()) return;
        std::string name; int score;
        while (file >> name >> score)
            addScore(name, score);
        file.close();
    }

    void save() {
        std::ofstream file(FILE_PATH);
        if (!file.is_open()) return;
        auto temp = minHeap;
        while (!temp.empty()) {
            file << temp.top().name << " " << temp.top().score << "\n";
            temp.pop();
        }
        file.close();
    }

    void addScore(const std::string& name, int score) {
        minHeap.push({score, name});
        // Keep only top MAX_ENTRIES
        while ((int)minHeap.size() > MAX_ENTRIES) minHeap.pop();
    }

    void display() {
        std::cout << "\n======= LEADERBOARD (Min-Heap) =======\n";
        if (minHeap.empty()) { std::cout << "  No scores yet!\n"; return; }

        // Extract all, sort descending for display
        std::vector<ScoreEntry> entries;
        auto temp = minHeap;
        while (!temp.empty()) { entries.push_back(temp.top()); temp.pop(); }

        int rank = entries.size();
        for (auto& e : entries)
            std::cout << "  #" << rank-- << "  " << e.name << "  -  " << e.score << "\n";
        std::cout << "======================================\n";
    }
};
