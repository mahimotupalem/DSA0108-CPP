#pragma once
#include "Types.h"
#include <string>

namespace qc {

struct LevelResultSummary {
    int levelNumber = 1;
    std::string levelTitle;
    int correctAnswers = 0;
    int incorrectAnswers = 0;
    int hintsUsed = 0;
    int timeTakenSeconds = 0;
    int basePoints = 0;
    int timeBonus = 0;
    int hintPenalty = 0;
    int wrongPenalty = 0;
    int decisionBonus = 0;
    int finalLevelScore = 0;
    int coinsEarned = 0;
    int starsEarned = 0;
    bool keyAwarded = false;
    std::string unlockedAchievement;
};

class ScoreManager {
public:
    ScoreManager();

    void resetRunStats();
    void startLevel(int levelNumber, const std::string& levelTitle, Difficulty diff);

    void recordCorrectAnswer(int baseReward = 120);
    void recordIncorrectAnswer();
    void recordHintUsed();
    void applyDecisionImpact(int scoreDelta, int coinsDelta);

    LevelResultSummary finalizeLevel(int timeRemainingSeconds, bool won);

    // Run-wide performance metrics
    int getTotalCorrect() const { return totalCorrect_; }
    int getTotalIncorrect() const { return totalIncorrect_; }
    int getTotalHintsUsed() const { return totalHintsUsed_; }
    int getTotalTimeSeconds() const { return totalTimeSeconds_; }
    int getRunTotalScore() const { return runTotalScore_; }
    float getOverallAccuracy() const;

    const LevelResultSummary& getLastLevelSummary() const { return lastSummary_; }

private:
    Difficulty currentDiff_ = Difficulty::Medium;
    LevelResultSummary currentLevel_;
    LevelResultSummary lastSummary_;

    int totalCorrect_ = 0;
    int totalIncorrect_ = 0;
    int totalHintsUsed_ = 0;
    int totalTimeSeconds_ = 0;
    int runTotalScore_ = 0;
};

} // namespace qc
