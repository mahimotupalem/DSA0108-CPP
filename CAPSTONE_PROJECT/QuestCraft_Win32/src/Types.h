#pragma once
#include <string>
#include <vector>

namespace qc {

enum class Difficulty {
    Easy = 0,
    Medium = 1,
    Hard = 2
};

inline std::string difficultyToString(Difficulty d) {
    switch (d) {
        case Difficulty::Easy: return "Easy";
        case Difficulty::Medium: return "Medium";
        case Difficulty::Hard: return "Hard";
    }
    return "Medium";
}

inline Difficulty stringToDifficulty(const std::string& s) {
    if (s == "Easy") return Difficulty::Easy;
    if (s == "Hard") return Difficulty::Hard;
    return Difficulty::Medium;
}

enum class ScreenState {
    MainMenu,
    PlayerRegistration,
    PlayerProfile,
    DifficultySelection,
    LevelSelection,
    PuzzleGameplay,
    DecisionSelection,
    HintOverlay,
    LevelCompletion,
    FinalResults,
    LeaderboardScreen,
    InstructionsScreen,
    SettingsScreen
};

enum class PuzzleType {
    MemorySequence,
    NumberLogic,
    PatternRecognition,
    MysteryClue,
    HybridCrystal
};

enum class RelicType {
    AegisShield = 0,     // Absorbs 1 wrong answer without losing a Heart!
    ChronoHourglass = 1, // Adds +15 seconds mid-puzzle
    OracleLens = 2       // Reveals a free clue without score penalty
};

struct DifficultySettings {
    Difficulty level;
    int startingLives;
    int baseTimeSeconds;
    int memorySequenceLength;
    float memoryShowSeconds;
    int hintPenalty;
    int wrongAnswerPenalty;
    float scoreMultiplier;
};

inline DifficultySettings getDifficultySettings(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:
            return {Difficulty::Easy, 5, 90, 4, 4.5f, 15, 10, 1.0f};
        case Difficulty::Medium:
            return {Difficulty::Medium, 3, 60, 5, 3.2f, 25, 20, 1.5f};
        case Difficulty::Hard:
            return {Difficulty::Hard, 2, 45, 7, 2.2f, 40, 35, 2.0f};
    }
    return {Difficulty::Medium, 3, 60, 5, 3.2f, 25, 20, 1.5f};
}

struct DecisionRecord {
    int levelNumber = 0;
    std::string scenarioTitle;
    std::string choiceMade;
    std::string consequenceSummary;
    int scoreDelta = 0;
    int coinsDelta = 0;
    int extraTimeDelta = 0;
};

struct GameHistoryEntry {
    std::string timestamp;
    std::string playerId;
    std::string playerName;
    std::string difficulty;
    int levelsCompleted = 0;
    int totalScore = 0;
    float accuracyPercent = 0.0f;
    int totalTimeSeconds = 0;
    std::string decisionsSummary;
};

} // namespace qc
