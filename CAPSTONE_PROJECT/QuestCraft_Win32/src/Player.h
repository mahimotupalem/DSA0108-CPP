#pragma once
#include "Types.h"
#include <string>
#include <vector>
#include <set>

namespace qc {

class Player {
public:
    Player();

    bool registerPlayer(const std::string& rawName, std::string& errorMessage);
    void loadFromRecord(const std::string& id, const std::string& playerName,
                        Difficulty diff, int bestScore, int maxUnlocked,
                        const std::vector<int>& completed, int totalCoins,
                        int totalStars, int totalKeys);

    void resetCurrentRun();
    void unlockLevel(int levelNumber);
    void markLevelCompleted(int levelNumber, int levelScore);

    // Getters
    const std::string& getPlayerId() const { return playerId_; }
    const std::string& getName() const { return name_; }
    bool isRegistered() const { return !playerId_.empty() && !name_.empty(); }
    Difficulty getDifficulty() const { return difficulty_; }
    int getHighestUnlockedLevel() const { return highestUnlockedLevel_; }
    const std::set<int>& getCompletedLevels() const { return completedLevels_; }
    bool isLevelUnlocked(int level) const { return level >= 1 && level <= highestUnlockedLevel_; }
    bool isLevelCompleted(int level) const { return completedLevels_.count(level) > 0; }

    int getBestScore() const { return bestScore_; }
    int getCurrentScore() const { return currentScore_; }
    int getLives() const { return lives_; }
    int getCoins() const { return coins_; }
    int getStars() const { return stars_; }
    int getKeys() const { return keys_; }
    const std::vector<std::string>& getAchievements() const { return achievements_; }
    const std::vector<DecisionRecord>& getDecisions() const { return decisions_; }

    // Mutators
    void setDifficulty(Difficulty d);
    void setLives(int l) { lives_ = l < 0 ? 0 : l; }
    void decrementLife() { if (lives_ > 0) --lives_; }
    void addScore(int delta);
    void addCoins(int delta);
    void addStars(int delta) { stars_ += delta; }
    void addKeys(int delta) { keys_ += delta; }
    void addAchievement(const std::string& badge);
    void recordDecision(const DecisionRecord& rec);
    void clearDecisions() { decisions_.clear(); }

    std::string serializeCompletedLevels() const;
    static std::vector<int> parseCompletedLevels(const std::string& raw);

private:
    static std::string generateUniqueId(const std::string& cleanName);

    std::string playerId_;
    std::string name_;
    Difficulty difficulty_ = Difficulty::Medium;
    int highestUnlockedLevel_ = 1;
    std::set<int> completedLevels_;
    int bestScore_ = 0;
    int currentScore_ = 0;
    int lives_ = 3;
    int coins_ = 0;
    int stars_ = 0;
    int keys_ = 0;
    std::vector<std::string> achievements_;
    std::vector<DecisionRecord> decisions_;
};

} // namespace qc
