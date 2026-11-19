#include "Leaderboard.h"
#include <algorithm>

namespace qc {

Leaderboard::Leaderboard() {}

void Leaderboard::clear() {
    entries_.clear();
}

void Leaderboard::addOrUpdateEntry(const LeaderboardEntry& entry) {
    for (auto& existing : entries_) {
        if (existing.playerId == entry.playerId) {
            if (entry.score >= existing.score) {
                existing = entry;
            }
            sortAndRank();
            return;
        }
    }
    entries_.push_back(entry);
    sortAndRank();
}

void Leaderboard::sortAndRank() {
    std::sort(entries_.begin(), entries_.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.accuracy > b.accuracy;
    });
    for (std::size_t i = 0; i < entries_.size(); ++i) {
        entries_[i].rank = static_cast<int>(i + 1);
    }
}

} // namespace qc
