#include "PuzzleEngine.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace qc {

namespace {
std::mt19937& globalRng() {
    static std::mt19937 rng(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    return rng;
}

std::string normalizeStr(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
    }
    return out;
}
} // namespace

std::string PuzzleEngine::getRuneName(int runeId) {
    switch (runeId % 6) {
        case 0: return "SUN";
        case 1: return "MOON";
        case 2: return "STAR";
        case 3: return "FLAME";
        case 4: return "WAVE";
        case 5: return "LEAF";
    }
    return "RUNE";
}

PuzzleChallenge MemoryPuzzleGenerator::generate(Difficulty diff, int stepIndex) const {
    DifficultySettings ds = getDifficultySettings(diff);
    int len = ds.memorySequenceLength + (stepIndex > 0 ? 1 : 0);

    std::uniform_int_distribution<int> dist(0, 5);
    PuzzleChallenge ch;
    ch.type = PuzzleType::MemorySequence;
    ch.title = "ANCIENT RUNE SEQUENCE";
    ch.isMultipleChoice = false;

    std::ostringstream ans;
    for (int i = 0; i < len; ++i) {
        int r = dist(globalRng());
        ch.memorySequence.push_back(r);
        if (i > 0) ans << "-";
        ans << (r + 1);
    }
    ch.expectedAnswer = ans.str();
    ch.prompt = "Memorize the glowing temple runes (" + std::to_string(len) +
                " runes), then click the rune buttons in exact order!";
    ch.hint = "First two runes in sequence: [" + PuzzleEngine::getRuneName(ch.memorySequence[0]) +
              "] -> [" + PuzzleEngine::getRuneName(ch.memorySequence[1]) + "]";
    return ch;
}

PuzzleChallenge LogicPuzzleGenerator::generate(Difficulty diff, int stepIndex) const {
    auto& rng = globalRng();
    PuzzleChallenge ch;
    ch.type = PuzzleType::NumberLogic;
    ch.title = "CHAMBER OF ARITHMETIC SEALS";
    ch.isMultipleChoice = false;

    if (diff == Difficulty::Easy) {
        std::uniform_int_distribution<int> aDist(8, 25);
        std::uniform_int_distribution<int> bDist(4, 15);
        int a = aDist(rng);
        int b = bDist(rng);
        if (stepIndex % 2 == 0) {
            ch.prompt = "The Stone Gate bears the inscription:\nIf 2 Magic Orbs + " + std::to_string(b) +
                        " = " + std::to_string(2 * a + b) + ", what is the value of 1 Magic Orb?";
            ch.expectedAnswer = std::to_string(a);
            ch.hint = "Subtract " + std::to_string(b) + " from " + std::to_string(2 * a + b) + ", then divide by 2.";
        } else {
            int c = a + b;
            ch.prompt = "A merchant trades 1 Ruby for " + std::to_string(a) +
                        " coins and 1 Sapphire for " + std::to_string(b) +
                        " coins. How many coins for 1 Ruby + 1 Sapphire?";
            ch.expectedAnswer = std::to_string(c);
            ch.hint = "Add " + std::to_string(a) + " and " + std::to_string(b) + " together.";
        }
    } else if (diff == Difficulty::Medium) {
        std::uniform_int_distribution<int> xDist(5, 16);
        int x = xDist(rng);
        if (stepIndex % 2 == 0) {
            int rhs = 3 * x + 11;
            ch.prompt = "Solve the Royal Alchemist's Equation:\n   3 * X + 11 = " + std::to_string(rhs) + "\nEnter the integer value of X:";
            ch.expectedAnswer = std::to_string(x);
            ch.hint = "3 * X = " + std::to_string(rhs - 11) + ". Divide by 3.";
        } else {
            int a = x;
            int b = x + 4;
            ch.prompt = "Two crystal pillars sum to " + std::to_string(a + b) +
                        ", and the taller pillar is 4 units higher than the shorter. Height of the shorter pillar?";
            ch.expectedAnswer = std::to_string(a);
            ch.hint = "Subtract 4 from " + std::to_string(a + b) + " and divide by 2.";
        }
    } else {
        std::uniform_int_distribution<int> nDist(6, 14);
        int n = nDist(rng);
        int val = n * n - n + 5;
        ch.prompt = "Grand Archmage Vault Lock:\n   N^2 - N + 5 = " + std::to_string(val) +
                    " (where N > 0)\nEnter the exact value of N:";
        ch.expectedAnswer = std::to_string(n);
        ch.hint = "N * (N - 1) = " + std::to_string(val - 5) + ". Find two consecutive integers whose product is " + std::to_string(val - 5) + ".";
    }
    return ch;
}

PuzzleChallenge PatternPuzzleGenerator::generate(Difficulty diff, int /*stepIndex*/) const {
    auto& rng = globalRng();
    PuzzleChallenge ch;
    ch.type = PuzzleType::PatternRecognition;
    ch.title = "PATTERN OF THE ANCIENTS";
    ch.isMultipleChoice = false;

    if (diff == Difficulty::Easy) {
        std::uniform_int_distribution<int> startDist(2, 10);
        std::uniform_int_distribution<int> stepDist(3, 7);
        int s = startDist(rng);
        int d = stepDist(rng);
        int n1 = s, n2 = s + d, n3 = s + 2 * d, n4 = s + 3 * d, ans = s + 4 * d;
        ch.prompt = "Complete the Rune Number Sequence:\n   " +
                    std::to_string(n1) + ",  " + std::to_string(n2) + ",  " +
                    std::to_string(n3) + ",  " + std::to_string(n4) + ",  ?";
        ch.expectedAnswer = std::to_string(ans);
        ch.hint = "Each number increases by +" + std::to_string(d) + ".";
    } else if (diff == Difficulty::Medium) {
        std::uniform_int_distribution<int> startDist(2, 6);
        int s = startDist(rng);
        // Pattern: *2 + 1
        int n1 = s;
        int n2 = n1 * 2 + 1;
        int n3 = n2 * 2 + 1;
        int n4 = n3 * 2 + 1;
        int ans = n4 * 2 + 1;
        ch.prompt = "Decipher the Geometric Progression:\n   " +
                    std::to_string(n1) + ",  " + std::to_string(n2) + ",  " +
                    std::to_string(n3) + ",  " + std::to_string(n4) + ",  ?";
        ch.expectedAnswer = std::to_string(ans);
        ch.hint = "Multiply the previous term by 2 and add 1.";
    } else {
        // Fibonacci-like shifted sequence
        std::uniform_int_distribution<int> aDist(2, 5);
        int a = aDist(rng), b = a + 2;
        int c = a + b;
        int d = b + c;
        int e = c + d;
        int ans = d + e;
        ch.prompt = "Master Glyph Sequence (Sum of two preceding runes):\n   " +
                    std::to_string(a) + ",  " + std::to_string(b) + ",  " +
                    std::to_string(c) + ",  " + std::to_string(d) + ",  " +
                    std::to_string(e) + ",  ?";
        ch.expectedAnswer = std::to_string(ans);
        ch.hint = "Add the last two visible numbers (" + std::to_string(d) + " + " + std::to_string(e) + ").";
    }
    return ch;
}

PuzzleChallenge MysteryPuzzleGenerator::generate(Difficulty /*diff*/, int stepIndex) const {
    struct Riddle {
        std::string prompt;
        std::vector<std::string> options;
        std::string correctIdx;
        std::string hint;
    };

    std::vector<Riddle> bank = {
        {
            "Clue 1: Silver footprints vanish beside the Whispering Oak.\n"
            "Clue 2: A brass key lies beneath roots where no sunlight falls.\n"
            "Clue 3: The thief fled toward water that never freezes.\n"
            "Which relic hides the Crystal Map?",
            {"A) Sunlit Watchtower", "B) Hollow Oak Root", "C) Frozen Cavern", "D) Royal Stables"},
            "1",
            "Re-read Clue 2 carefully: the brass key is hidden beneath the roots."
        },
        {
            "Clue 1: Four lanterns guard the Misty Clearing: North (Gold), East (Emerald), South (Sapphire), West (Ruby).\n"
            "Clue 2: The Crystal responds only to the color of deep ocean waters.\n"
            "Which lantern must you ignite?",
            {"A) North Gold Lantern", "B) East Emerald Lantern", "C) South Sapphire Lantern", "D) West Ruby Lantern"},
            "2",
            "Sapphire is the deep blue gem associated with ocean waters."
        },
        {
            "Riddle of the Forest Guardian:\n"
            "'I have cities without houses, mountains without trees, and water without fish. What am I?'",
            {"A) An Ancient Map", "B) A Dream", "C) A Mirror", "D) A Hourglass"},
            "0",
            "Travelers unfold it to navigate the Lost Kingdom."
        },
        {
            "Clue 1: Sir Kael carries a shield; Lady Lyra carries a bow; Mage Orin carries a staff.\n"
            "Clue 2: The Crystal thief left scorch marks and dropped a wooden staff splinter.\n"
            "Who tampered with the Crystal Pedestal?",
            {"A) Sir Kael", "B) Lady Lyra", "C) Mage Orin", "D) The Castle Guard"},
            "2",
            "Match the wooden staff splinter to its owner in Clue 1."
        }
    };

    std::uniform_int_distribution<std::size_t> dist(0, bank.size() - 1);
    std::size_t idx = (dist(globalRng()) + static_cast<std::size_t>(stepIndex)) % bank.size();
    const auto& r = bank[idx];

    PuzzleChallenge ch;
    ch.type = PuzzleType::MysteryClue;
    ch.title = "MYSTERY OF THE WHISPERING FOREST";
    ch.prompt = r.prompt;
    ch.choices = r.options;
    ch.expectedAnswer = r.correctIdx;
    ch.hint = r.hint;
    ch.isMultipleChoice = true;
    return ch;
}

PuzzleEngine::PuzzleEngine()
    : memoryGen_(std::make_unique<MemoryPuzzleGenerator>()),
      logicGen_(std::make_unique<LogicPuzzleGenerator>()),
      patternGen_(std::make_unique<PatternPuzzleGenerator>()),
      mysteryGen_(std::make_unique<MysteryPuzzleGenerator>()) {}

std::vector<PuzzleChallenge> PuzzleEngine::buildChallengesForLevel(int levelNumber, Difficulty diff, const std::string& bonusClue) const {
    std::vector<PuzzleChallenge> list;
    switch (levelNumber) {
        case 1: // Level 1 - Memory Temple
            list.push_back(memoryGen_->generate(diff, 0));
            list.push_back(memoryGen_->generate(diff, 1));
            break;
        case 2: // Level 2 - Logic Chamber
            list.push_back(logicGen_->generate(diff, 0));
            list.push_back(patternGen_->generate(diff, 0));
            break;
        case 3: // Level 3 - Mystery Forest
            list.push_back(mysteryGen_->generate(diff, 0));
            list.push_back(mysteryGen_->generate(diff, 1));
            break;
        case 4: // Level 4 - Decision Gate (post-decision guardian trial)
            list.push_back(patternGen_->generate(diff, 1));
            break;
        case 5: // Level 5 - Crystal Challenge (Memory + Logic + Pattern hybrid)
        default: {
            list.push_back(memoryGen_->generate(diff, 1));
            auto logicCh = logicGen_->generate(diff, 1);
            if (!bonusClue.empty()) {
                logicCh.prompt += "\n[Unlocked Scroll Insight: " + bonusClue + "]";
            }
            list.push_back(logicCh);
            list.push_back(mysteryGen_->generate(diff, 2));
            break;
        }
    }
    return list;
}

bool PuzzleEngine::validateAnswer(const PuzzleChallenge& challenge, const std::string& playerInput) const {
    std::string cleanUser = normalizeStr(playerInput);
    std::string cleanExpected = normalizeStr(challenge.expectedAnswer);
    if (cleanUser.empty()) return false;
    return cleanUser == cleanExpected;
}

} // namespace qc
