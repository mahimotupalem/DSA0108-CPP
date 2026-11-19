#pragma once
#include "Types.h"
#include <string>

namespace qc {

class HintSystem {
public:
    HintSystem();

    void resetForLevel(Difficulty diff);
    bool canRequestHint() const;
    std::string requestHint(const std::string& hintText);

    int getHintsUsedThisLevel() const { return hintsUsedLevel_; }
    int getMaxHintsPerLevel() const { return maxHintsPerLevel_; }
    int getCurrentPenaltyCost() const { return penaltyPerHint_; }
    bool isHintVisible() const { return hintVisible_; }
    const std::string& getActiveHintText() const { return activeHint_; }
    void dismissHint() { hintVisible_ = false; }

private:
    int hintsUsedLevel_ = 0;
    int maxHintsPerLevel_ = 2;
    int penaltyPerHint_ = 25;
    bool hintVisible_ = false;
    std::string activeHint_;
};

} // namespace qc
