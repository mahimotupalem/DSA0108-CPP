#include "Player.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

namespace qc {

Player::Player() {
    DifficultySettings ds = getDifficultySettings(difficulty_);
    lives_ = ds.startingLives;
}

std::string Player::generateUniqueId(const std::string& cleanName) {
    std::string prefix = "QC-";
    std::string letters;
    for (char c : cleanName) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            letters.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
            if (letters.size() == 3) break;
        }
    }
    while (letters.size() < 3) letters.push_back('X');

    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned int>(now));
    std::uniform_int_distribution<int> dist(1000, 9999);

    std::ostringstream oss;
    oss << prefix << letters << "-" << dist(rng);
    return oss.str();
}

bool Player::registerPlayer(const std::string& rawName, std::string& errorMessage) {
    // Trim whitespace
    std::size_t start = rawName.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        errorMessage = "Hero name cannot be empty. Please enter 2 to 20 characters.";
        return false;
    }
    std::size_t end = rawName.find_last_not_of(" \t\r\n");
    std::string trimmed = rawName.substr(start, end - start + 1);

    if (trimmed.size() < 2 || trimmed.size() > 20) {
        errorMessage = "Name must be between 2 and 20 characters.";
        return false;
    }

    for (char c : trimmed) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '_' && c != '-') {
            errorMessage = "Use only letters, numbers, spaces, '-' or '_'.";
            return false;
        }
    }

    name_ = trimmed;
    playerId_ = generateUniqueId(name_);
    difficulty_ = Difficulty::Medium;
    highestUnlockedLevel_ = 1;
    completedLevels_.clear();
    bestScore_ = 0;
    currentScore_ = 0;
    coins_ = 50; // Welcome pouch
    stars_ = 0;
    keys_ = 0;
    achievements_.clear();
    decisions_.clear();
    lives_ = getDifficultySettings(difficulty_).startingLives;
    errorMessage.clear();
    return true;
}

void Player::loadFromRecord(const std::string& id, const std::string& playerName,
                            Difficulty diff, int bestScore, int maxUnlocked,
                            const std::vector<int>& completed, int totalCoins,
                            int totalStars, int totalKeys) {
    playerId_ = id;
    name_ = playerName;
    difficulty_ = diff;
    bestScore_ = bestScore;
    highestUnlockedLevel_ = std::max(1, std::min(5, maxUnlocked));
    completedLevels_.clear();
    for (int lvl : completed) {
        if (lvl >= 1 && lvl <= 5) completedLevels_.insert(lvl);
    }
    coins_ = totalCoins;
    stars_ = totalStars;
    keys_ = totalKeys;
    currentScore_ = 0;
    lives_ = getDifficultySettings(difficulty_).startingLives;
}

void Player::resetCurrentRun() {
    currentScore_ = 0;
    lives_ = getDifficultySettings(difficulty_).startingLives;
    decisions_.clear();
}

void Player::unlockLevel(int levelNumber) {
    if (levelNumber > highestUnlockedLevel_ && levelNumber <= 5) {
        highestUnlockedLevel_ = levelNumber;
    }
}

void Player::markLevelCompleted(int levelNumber, int levelScore) {
    if (levelNumber >= 1 && levelNumber <= 5) {
        completedLevels_.insert(levelNumber);
        if (levelNumber < 5) {
            unlockLevel(levelNumber + 1);
        }
    }
    if (levelScore > bestScore_) {
        bestScore_ = levelScore;
    }
    if (currentScore_ > bestScore_) {
        bestScore_ = currentScore_;
    }
}

void Player::setDifficulty(Difficulty d) {
    difficulty_ = d;
    lives_ = getDifficultySettings(d).startingLives;
}

void Player::addScore(int delta) {
    currentScore_ += delta;
    if (currentScore_ < 0) currentScore_ = 0;
    if (currentScore_ > bestScore_) {
        bestScore_ = currentScore_;
    }
}

void Player::addCoins(int delta) {
    coins_ += delta;
    if (coins_ < 0) coins_ = 0;
}

void Player::addAchievement(const std::string& badge) {
    if (std::find(achievements_.begin(), achievements_.end(), badge) == achievements_.end()) {
        achievements_.push_back(badge);
    }
}

void Player::recordDecision(const DecisionRecord& rec) {
    decisions_.push_back(rec);
}

std::string Player::serializeCompletedLevels() const {
    if (completedLevels_.empty()) return "NONE";
    std::ostringstream oss;
    bool first = true;
    for (int lvl : completedLevels_) {
        if (!first) oss << "|";
        oss << lvl;
        first = false;
    }
    return oss.str();
}

std::vector<int> Player::parseCompletedLevels(const std::string& raw) {
    std::vector<int> out;
    if (raw.empty() || raw == "NONE") return out;
    std::stringstream ss(raw);
    std::string token;
    while (std::getline(ss, token, '|')) {
        try {
            out.push_back(std::stoi(token));
        } catch (...) {}
    }
    return out;
}

} // namespace qc
