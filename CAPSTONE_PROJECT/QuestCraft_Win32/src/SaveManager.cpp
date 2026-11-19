#include "SaveManager.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace fs = std::filesystem;

namespace qc {

SaveManager::SaveManager(const std::string& dataDir)
    : dataDir_(dataDir),
      playersPath_(dataDir + "/players.csv"),
      progressPath_(dataDir + "/progress.csv"),
      leaderboardPath_(dataDir + "/leaderboard.csv"),
      historyPath_(dataDir + "/game_history.csv") {
    ensureDataFilesExist();
}

void SaveManager::ensureDataFilesExist() const {
    try {
        if (!fs::exists(dataDir_)) {
            fs::create_directories(dataDir_);
        }
        if (!fs::exists(playersPath_)) {
            std::ofstream f(playersPath_);
            f << "player_id,player_name,difficulty,created_at\n";
        }
        if (!fs::exists(progressPath_)) {
            std::ofstream f(progressPath_);
            f << "player_id,player_name,difficulty,best_score,highest_unlocked_level,completed_levels,coins,stars,keys\n";
        }
        if (!fs::exists(leaderboardPath_)) {
            std::ofstream f(leaderboardPath_);
            f << "rank,player_id,player_name,difficulty,score,accuracy,levels_completed\n";
        }
        if (!fs::exists(historyPath_)) {
            std::ofstream f(historyPath_);
            f << "timestamp,player_id,player_name,difficulty,levels_completed,total_score,accuracy,total_time_sec,decisions\n";
        }
    } catch (...) {
        // Gracefully handle read-only environments
    }
}

std::vector<std::string> SaveManager::splitCsvLine(const std::string& line) {
    std::vector<std::string> cells;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (!item.empty() && item.back() == '\r') item.pop_back();
        cells.push_back(item);
    }
    return cells;
}

std::string SaveManager::sanitizeCsv(const std::string& text) {
    std::string out = text;
    for (char& c : out) {
        if (c == ',' || c == '\n' || c == '\r') c = ';';
    }
    return out;
}

std::string SaveManager::currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tmBuf{};
#if defined(_WIN32)
    localtime_s(&tmBuf, &t);
#else
    localtime_r(&t, &tmBuf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool SaveManager::savePlayerProfile(const Player& player) const {
    if (!player.isRegistered()) return false;
    ensureDataFilesExist();

    // Update players.csv (upsert)
    std::vector<std::vector<std::string>> playerRows;
    {
        std::ifstream in(playersPath_);
        std::string line;
        bool header = true;
        while (std::getline(in, line)) {
            if (header) { header = false; continue; }
            if (line.empty()) continue;
            auto cols = splitCsvLine(line);
            if (cols.size() >= 4 && cols[0] != player.getPlayerId()) {
                playerRows.push_back(cols);
            }
        }
    }
    playerRows.push_back({
        player.getPlayerId(),
        sanitizeCsv(player.getName()),
        difficultyToString(player.getDifficulty()),
        currentTimestamp()
    });
    {
        std::ofstream out(playersPath_, std::ios::trunc);
        if (!out.is_open()) return false;
        out << "player_id,player_name,difficulty,created_at\n";
        for (const auto& r : playerRows) {
            out << r[0] << "," << r[1] << "," << r[2] << "," << r[3] << "\n";
        }
    }

    // Update progress.csv (upsert)
    std::vector<std::vector<std::string>> progRows;
    {
        std::ifstream in(progressPath_);
        std::string line;
        bool header = true;
        while (std::getline(in, line)) {
            if (header) { header = false; continue; }
            if (line.empty()) continue;
            auto cols = splitCsvLine(line);
            if (cols.size() >= 9 && cols[0] != player.getPlayerId()) {
                progRows.push_back(cols);
            }
        }
    }
    progRows.push_back({
        player.getPlayerId(),
        sanitizeCsv(player.getName()),
        difficultyToString(player.getDifficulty()),
        std::to_string(player.getBestScore()),
        std::to_string(player.getHighestUnlockedLevel()),
        player.serializeCompletedLevels(),
        std::to_string(player.getCoins()),
        std::to_string(player.getStars()),
        std::to_string(player.getKeys())
    });
    {
        std::ofstream out(progressPath_, std::ios::trunc);
        if (!out.is_open()) return false;
        out << "player_id,player_name,difficulty,best_score,highest_unlocked_level,completed_levels,coins,stars,keys\n";
        for (const auto& r : progRows) {
            out << r[0] << "," << r[1] << "," << r[2] << "," << r[3] << ","
                << r[4] << "," << r[5] << "," << r[6] << "," << r[7] << "," << r[8] << "\n";
        }
    }
    return true;
}

bool SaveManager::loadMostRecentPlayer(Player& outPlayer) const {
    std::ifstream in(progressPath_);
    if (!in.is_open()) return false;
    std::string line, lastValid;
    bool header = true;
    while (std::getline(in, line)) {
        if (header) { header = false; continue; }
        if (!line.empty()) lastValid = line;
    }
    if (lastValid.empty()) return false;
    auto cols = splitCsvLine(lastValid);
    if (cols.size() < 9) return false;
    try {
        outPlayer.loadFromRecord(
            cols[0],
            cols[1],
            stringToDifficulty(cols[2]),
            std::stoi(cols[3]),
            std::stoi(cols[4]),
            Player::parseCompletedLevels(cols[5]),
            std::stoi(cols[6]),
            std::stoi(cols[7]),
            std::stoi(cols[8])
        );
        return true;
    } catch (...) {
        return false;
    }
}

bool SaveManager::loadPlayerById(const std::string& playerId, Player& outPlayer) const {
    std::ifstream in(progressPath_);
    if (!in.is_open()) return false;
    std::string line;
    bool header = true;
    while (std::getline(in, line)) {
        if (header) { header = false; continue; }
        auto cols = splitCsvLine(line);
        if (cols.size() >= 9 && cols[0] == playerId) {
            try {
                outPlayer.loadFromRecord(
                    cols[0], cols[1], stringToDifficulty(cols[2]),
                    std::stoi(cols[3]), std::stoi(cols[4]),
                    Player::parseCompletedLevels(cols[5]),
                    std::stoi(cols[6]), std::stoi(cols[7]), std::stoi(cols[8])
                );
                return true;
            } catch (...) {
                return false;
            }
        }
    }
    return false;
}

bool SaveManager::saveLeaderboard(const Leaderboard& leaderboard) const {
    ensureDataFilesExist();
    std::ofstream out(leaderboardPath_, std::ios::trunc);
    if (!out.is_open()) return false;
    out << "rank,player_id,player_name,difficulty,score,accuracy,levels_completed\n";
    for (const auto& e : leaderboard.getEntries()) {
        out << e.rank << ","
            << e.playerId << ","
            << sanitizeCsv(e.playerName) << ","
            << e.difficulty << ","
            << e.score << ","
            << std::fixed << std::setprecision(1) << e.accuracy << ","
            << e.levelsCompleted << "\n";
    }
    return true;
}

bool SaveManager::loadLeaderboard(Leaderboard& outLeaderboard) const {
    outLeaderboard.clear();
    std::ifstream in(leaderboardPath_);
    if (!in.is_open()) return false;
    std::string line;
    bool header = true;
    while (std::getline(in, line)) {
        if (header) { header = false; continue; }
        if (line.empty()) continue;
        auto cols = splitCsvLine(line);
        if (cols.size() >= 7) {
            try {
                LeaderboardEntry e;
                e.rank = std::stoi(cols[0]);
                e.playerId = cols[1];
                e.playerName = cols[2];
                e.difficulty = cols[3];
                e.score = std::stoi(cols[4]);
                e.accuracy = std::stof(cols[5]);
                e.levelsCompleted = std::stoi(cols[6]);
                outLeaderboard.addOrUpdateEntry(e);
            } catch (...) {}
        }
    }
    return true;
}

bool SaveManager::appendGameHistory(const GameHistoryEntry& entry) const {
    ensureDataFilesExist();
    std::ofstream out(historyPath_, std::ios::app);
    if (!out.is_open()) return false;
    out << currentTimestamp() << ","
        << entry.playerId << ","
        << sanitizeCsv(entry.playerName) << ","
        << entry.difficulty << ","
        << entry.levelsCompleted << ","
        << entry.totalScore << ","
        << std::fixed << std::setprecision(1) << entry.accuracyPercent << ","
        << entry.totalTimeSeconds << ","
        << sanitizeCsv(entry.decisionsSummary) << "\n";
    return true;
}

std::vector<GameHistoryEntry> SaveManager::loadGameHistory() const {
    std::vector<GameHistoryEntry> history;
    std::ifstream in(historyPath_);
    if (!in.is_open()) return history;
    std::string line;
    bool header = true;
    while (std::getline(in, line)) {
        if (header) { header = false; continue; }
        if (line.empty()) continue;
        auto cols = splitCsvLine(line);
        if (cols.size() >= 9) {
            try {
                GameHistoryEntry h;
                h.timestamp = cols[0];
                h.playerId = cols[1];
                h.playerName = cols[2];
                h.difficulty = cols[3];
                h.levelsCompleted = std::stoi(cols[4]);
                h.totalScore = std::stoi(cols[5]);
                h.accuracyPercent = std::stof(cols[6]);
                h.totalTimeSeconds = std::stoi(cols[7]);
                h.decisionsSummary = cols[8];
                history.push_back(h);
            } catch (...) {}
        }
    }
    return history;
}

} // namespace qc
