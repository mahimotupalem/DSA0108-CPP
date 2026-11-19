#pragma once
#include "Types.h"
#include <string>
#include <vector>
#include <memory>

namespace qc {

struct PuzzleChallenge {
    PuzzleType type;
    std::string title;
    std::string prompt;
    std::string hint;
    std::vector<std::string> choices; // Non-empty for multiple-choice/mystery
    std::string expectedAnswer;       // Used for numeric/text or choice index ("0","1","2","3")
    std::vector<int> memorySequence;  // Used for MemorySequence (0..5 rune IDs)
    bool isMultipleChoice = false;
};

// Polymorphic base class demonstrating OOP inheritance & virtual dispatch
class BasePuzzleGenerator {
public:
    virtual ~BasePuzzleGenerator() = default;
    virtual PuzzleChallenge generate(Difficulty diff, int stepIndex) const = 0;
};

class MemoryPuzzleGenerator : public BasePuzzleGenerator {
public:
    PuzzleChallenge generate(Difficulty diff, int stepIndex) const override;
};

class LogicPuzzleGenerator : public BasePuzzleGenerator {
public:
    PuzzleChallenge generate(Difficulty diff, int stepIndex) const override;
};

class PatternPuzzleGenerator : public BasePuzzleGenerator {
public:
    PuzzleChallenge generate(Difficulty diff, int stepIndex) const override;
};

class MysteryPuzzleGenerator : public BasePuzzleGenerator {
public:
    PuzzleChallenge generate(Difficulty diff, int stepIndex) const override;
};

class PuzzleEngine {
public:
    PuzzleEngine();

    std::vector<PuzzleChallenge> buildChallengesForLevel(int levelNumber, Difficulty diff, const std::string& bonusClue = "") const;
    bool validateAnswer(const PuzzleChallenge& challenge, const std::string& playerInput) const;

    static std::string getRuneName(int runeId);

private:
    std::unique_ptr<BasePuzzleGenerator> memoryGen_;
    std::unique_ptr<BasePuzzleGenerator> logicGen_;
    std::unique_ptr<BasePuzzleGenerator> patternGen_;
    std::unique_ptr<BasePuzzleGenerator> mysteryGen_;
};

} // namespace qc
