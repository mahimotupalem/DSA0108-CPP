#pragma once
#include "Types.h"
#include <string>
#include <vector>

namespace qc {

struct LevelInfo {
    int number = 1;
    std::string name;
    std::string subtitle;
    std::string description;
    std::string bgAssetPath;
    bool hasDecisionPhase = false;
};

class LevelManager {
public:
    LevelManager();

    const std::vector<LevelInfo>& getAllLevels() const { return levels_; }
    const LevelInfo& getLevel(int levelNumber) const;
    int getTotalLevelCount() const { return static_cast<int>(levels_.size()); }

private:
    std::vector<LevelInfo> levels_;
};

} // namespace qc
