#pragma once
#include <string>
#include <vector>

namespace qc {

struct LeaderboardEntry {
    int rank = 0;
    std::string playerId;
    std::string playerName;
    std::string difficulty;
    int score = 0;
    float accuracy = 0.0f;
    int levelsCompleted = 0;
};

class Leaderboard {
public:
    Leaderboard();

    void clear();
    void addOrUpdateEntry(const LeaderboardEntry& entry);
    void sortAndRank();
    const std::vector<LeaderboardEntry>& getEntries() const { return entries_; }

private:
    std::vector<LeaderboardEntry> entries_;
};

} // namespace qc
