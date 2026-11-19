#include "HintSystem.h"

namespace qc {

HintSystem::HintSystem() {
    resetForLevel(Difficulty::Medium);
}

void HintSystem::resetForLevel(Difficulty diff) {
    DifficultySettings ds = getDifficultySettings(diff);
    hintsUsedLevel_ = 0;
    maxHintsPerLevel_ = (diff == Difficulty::Hard) ? 1 : 2;
    penaltyPerHint_ = ds.hintPenalty;
    hintVisible_ = false;
    activeHint_.clear();
}

bool HintSystem::canRequestHint() const {
    return hintsUsedLevel_ < maxHintsPerLevel_;
}

std::string HintSystem::requestHint(const std::string& hintText) {
    if (!canRequestHint()) {
        activeHint_ = "No hints remaining for this chamber! Rely on your royal instincts.";
        hintVisible_ = true;
        return activeHint_;
    }
    hintsUsedLevel_++;
    activeHint_ = hintText;
    hintVisible_ = true;
    return activeHint_;
}

} // namespace qc
