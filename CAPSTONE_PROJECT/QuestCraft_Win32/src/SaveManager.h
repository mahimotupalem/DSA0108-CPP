#pragma once
#include "Player.h"
#include "Leaderboard.h"
#include "Types.h"
#include <string>
#include <vector>

namespace qc {

class SaveManager {
public:
    explicit SaveManager(const std::string& dataDir = "data");

    void ensureDataFilesExist() const;

    bool savePlayerProfile(const Player& player) const;
    bool loadMostRecentPlayer(Player& outPlayer) const;
    bool loadPlayerById(const std::string& playerId, Player& outPlayer) const;

    bool saveLeaderboard(const Leaderboard& leaderboard) const;
    bool loadLeaderboard(Leaderboard& outLeaderboard) const;

    bool appendGameHistory(const GameHistoryEntry& entry) const;
    std::vector<GameHistoryEntry> loadGameHistory() const;

private:
    std::string dataDir_;
    std::string playersPath_;
    std::string progressPath_;
    std::string leaderboardPath_;
    std::string historyPath_;

    static std::vector<std::string> splitCsvLine(const std::string& line);
    static std::string sanitizeCsv(const std::string& text);
    static std::string currentTimestamp();
};

} // namespace qc
