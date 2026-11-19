#pragma once
#include "Types.h"
#include <string>
#include <vector>

namespace qc {

struct DecisionOption {
    std::string id;
    std::string label;
    std::string description;
    std::string consequenceText;
    int scoreDelta = 0;
    int coinsDelta = 0;
    int extraTimeDelta = 0;
    bool grantsBonusClue = false;
    std::string bonusClueText;
};

struct DecisionScenario {
    int levelNumber = 4;
    std::string title;
    std::string narrative;
    std::vector<DecisionOption> options;
};

class DecisionSystem {
public:
    DecisionSystem();

    DecisionScenario getScenarioForLevel(int levelNumber, Difficulty diff) const;
    DecisionRecord evaluateChoice(const DecisionScenario& scenario, std::size_t optionIndex);

    bool hasBonusClue() const { return bonusClueUnlocked_; }
    const std::string& getBonusClueText() const { return bonusClueText_; }
    int getPendingTimeBonus() const { return pendingTimeBonus_; }
    void consumePendingTimeBonus() { pendingTimeBonus_ = 0; }
    void reset();

private:
    bool bonusClueUnlocked_ = false;
    std::string bonusClueText_;
    int pendingTimeBonus_ = 0;
};

} // namespace qc
