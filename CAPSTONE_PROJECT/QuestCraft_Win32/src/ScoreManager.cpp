#include "ScoreManager.h"
#include <algorithm>

namespace qc {

ScoreManager::ScoreManager() {
    resetRunStats();
}

void ScoreManager::resetRunStats() {
    totalCorrect_ = 0;
    totalIncorrect_ = 0;
    totalHintsUsed_ = 0;
    totalTimeSeconds_ = 0;
    runTotalScore_ = 0;
}

void ScoreManager::startLevel(int levelNumber, const std::string& levelTitle, Difficulty diff) {
    currentDiff_ = diff;
    currentLevel_ = LevelResultSummary{};
    currentLevel_.levelNumber = levelNumber;
    currentLevel_.levelTitle = levelTitle;
}

void ScoreManager::recordCorrectAnswer(int baseReward) {
    DifficultySettings ds = getDifficultySettings(currentDiff_);
    int earned = static_cast<int>(baseReward * ds.scoreMultiplier);
    currentLevel_.correctAnswers++;
    currentLevel_.basePoints += earned;
    totalCorrect_++;
}

void ScoreManager::recordIncorrectAnswer() {
    DifficultySettings ds = getDifficultySettings(currentDiff_);
    currentLevel_.incorrectAnswers++;
    currentLevel_.wrongPenalty += ds.wrongAnswerPenalty;
    totalIncorrect_++;
}

void ScoreManager::recordHintUsed() {
    DifficultySettings ds = getDifficultySettings(currentDiff_);
    currentLevel_.hintsUsed++;
    currentLevel_.hintPenalty += ds.hintPenalty;
    totalHintsUsed_++;
}

void ScoreManager::applyDecisionImpact(int scoreDelta, int coinsDelta) {
    currentLevel_.decisionBonus += scoreDelta;
    currentLevel_.coinsEarned += coinsDelta;
}

LevelResultSummary ScoreManager::finalizeLevel(int timeRemainingSeconds, bool won) {
    DifficultySettings ds = getDifficultySettings(currentDiff_);
    int elapsed = std::max(1, ds.baseTimeSeconds - std::max(0, timeRemainingSeconds));
    currentLevel_.timeTakenSeconds = elapsed;
    totalTimeSeconds_ += elapsed;

    if (won) {
        currentLevel_.timeBonus = std::max(0, timeRemainingSeconds) * 2;
        int raw = currentLevel_.basePoints + currentLevel_.timeBonus + currentLevel_.decisionBonus
                  - currentLevel_.hintPenalty - currentLevel_.wrongPenalty;
        currentLevel_.finalLevelScore = std::max(50, raw);

        // Rewards calculation
        currentLevel_.coinsEarned += 25 + currentLevel_.levelNumber * 10 + (timeRemainingSeconds / 5);
        if (currentLevel_.incorrectAnswers == 0 && currentLevel_.hintsUsed == 0) {
            currentLevel_.starsEarned = 3;
            currentLevel_.unlockedAchievement = "Flawless Strategist (Lv " + std::to_string(currentLevel_.levelNumber) + ")";
        } else if (currentLevel_.incorrectAnswers <= 1) {
            currentLevel_.starsEarned = 2;
            currentLevel_.unlockedAchievement = "Kingdom Explorer (Lv " + std::to_string(currentLevel_.levelNumber) + ")";
        } else {
            currentLevel_.starsEarned = 1;
            currentLevel_.unlockedAchievement = "Persevering Adventurer (Lv " + std::to_string(currentLevel_.levelNumber) + ")";
        }
        currentLevel_.keyAwarded = true;
    } else {
        currentLevel_.timeBonus = 0;
        currentLevel_.finalLevelScore = std::max(0, currentLevel_.basePoints - currentLevel_.hintPenalty - currentLevel_.wrongPenalty);
        currentLevel_.starsEarned = 0;
        currentLevel_.keyAwarded = false;
    }

    runTotalScore_ += currentLevel_.finalLevelScore;
    lastSummary_ = currentLevel_;
    return lastSummary_;
}

float ScoreManager::getOverallAccuracy() const {
    int totalAttempts = totalCorrect_ + totalIncorrect_;
    if (totalAttempts == 0) return 100.0f;
    return (static_cast<float>(totalCorrect_) / static_cast<float>(totalAttempts)) * 100.0f;
}

} // namespace qc
