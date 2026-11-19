#include "LevelManager.h"
#include <algorithm>

namespace qc {

LevelManager::LevelManager() {
    levels_ = {
        {
            1,
            "Level 1: Memory Temple",
            "Hall of Echoing Runes",
            "Memorize the sacred temple symbol sequences before they fade into shadow, then reconstruct them.",
            "assets/backgrounds/temple_bg.png",
            false
        },
        {
            2,
            "Level 2: Logic Chamber",
            "Vault of Royal Equations",
            "Crack numerical seals and ancient mathematical progressions guarded by the Stone Architects.",
            "assets/backgrounds/kingdom_bg.png",
            false
        },
        {
            3,
            "Level 3: Mystery Forest",
            "Grove of Hidden Clues",
            "Investigate forest clues, solve the Guardian's riddles, and track the Crystal thief's trail.",
            "assets/backgrounds/forest_bg.png",
            false
        },
        {
            4,
            "Level 4: Decision Gate",
            "Crossroads of Destiny",
            "Choose between the Golden Door and the Secret Passage. Your decision reshapes Level 5!",
            "assets/backgrounds/temple_bg.png",
            true
        },
        {
            5,
            "Level 5: Crystal Challenge",
            "Sanctum of the Magical Crystal",
            "Master memory, logic, and mystery in the final trial to reclaim the Lost Kingdom's Crystal.",
            "assets/backgrounds/kingdom_bg.png",
            false
        }
    };
}

const LevelInfo& LevelManager::getLevel(int levelNumber) const {
    int idx = std::max(1, std::min(static_cast<int>(levels_.size()), levelNumber)) - 1;
    return levels_[static_cast<std::size_t>(idx)];
}

} // namespace qc
