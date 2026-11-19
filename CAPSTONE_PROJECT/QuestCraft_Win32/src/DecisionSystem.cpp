#include "DecisionSystem.h"

namespace qc {

DecisionSystem::DecisionSystem() {
    reset();
}

void DecisionSystem::reset() {
    bonusClueUnlocked_ = false;
    bonusClueText_.clear();
    pendingTimeBonus_ = 0;
}

DecisionScenario DecisionSystem::getScenarioForLevel(int levelNumber, Difficulty diff) const {
    DecisionScenario sc;
    sc.levelNumber = levelNumber;

    if (levelNumber == 4) {
        sc.title = "THE TWIN PORTALS OF ELDORIA";
        sc.narrative = "Before the Inner Sanctum stand two ancient paths: the radiant Golden Door, "
                       "guarded by solar runes, and the Whispering Secret Passage veiled in shadow. "
                       "Your choice shapes the final Crystal Challenge!";

        DecisionOption golden;
        golden.id = "GOLDEN_DOOR";
        golden.label = "1. Open the Golden Door (Path of Valor)";
        golden.description = "Face the Royal Sentinel directly. High honor & treasure, standard time.";
        golden.consequenceText = "The Sun Sentinel salutes your courage! You gain +150 Honor Score and +60 Gold Coins, "
                                 "though the Crystal Sanctum clock ticks at normal speed.";
        golden.scoreDelta = (diff == Difficulty::Hard) ? 200 : 150;
        golden.coinsDelta = 60;
        golden.extraTimeDelta = 0;
        golden.grantsBonusClue = false;

        DecisionOption secret;
        secret.id = "SECRET_PASSAGE";
        secret.label = "2. Enter the Secret Passage (Path of Wisdom)";
        secret.description = "Slip behind the tapestries to uncover ancient scholar scrolls & extra time.";
        secret.consequenceText = "You discover the Archmage's Cipher! You gain +15s bonus time for Level 5, "
                                 "+90 Wisdom Score, and a secret clue for the Magical Crystal!";
        secret.scoreDelta = 90;
        secret.coinsDelta = 25;
        secret.extraTimeDelta = 15;
        secret.grantsBonusClue = true;
        secret.bonusClueText = "Archmage Scroll: The Crystal's final harmonic key is always a prime number!";

        DecisionOption ritual;
        ritual.id = "ANCIENT_ALTAR";
        ritual.label = "3. Offer 30 Coins at the Wisdom Altar";
        ritual.description = "Spend 30 Gold Coins to receive BOTH extra time (+10s) and +120 Score.";
        ritual.consequenceText = "The Altar flares with sapphire light! You receive +120 Score, +10s bonus time, "
                                 "and an ancient rune insight.";
        ritual.scoreDelta = 120;
        ritual.coinsDelta = -30;
        ritual.extraTimeDelta = 10;
        ritual.grantsBonusClue = true;
        ritual.bonusClueText = "Altar Vision: Multiply the first two sequence digits, then add 7.";

        sc.options = {golden, secret, ritual};
    } else {
        sc.title = "THE CRYSTAL GUARDIAN'S PACT";
        sc.narrative = "The Magical Crystal hovers above an abyss. How will you claim it?";

        DecisionOption bind;
        bind.id = "BIND_CRYSTAL";
        bind.label = "1. Channel Royal Seal (+180 Score)";
        bind.description = "Use your collected keys to stabilize the crystal safely.";
        bind.consequenceText = "The Crystal harmonizes peacefully with the Kingdom's crest! +180 Score.";
        bind.scoreDelta = 180;
        bind.coinsDelta = 50;

        DecisionOption purify;
        purify.id = "PURIFY_REALM";
        purify.label = "2. Release Crystal Pulse to Heal Kingdom (+220 Score)";
        purify.description = "Send a wave of light across the Lost Kingdom before claiming the shard.";
        purify.consequenceText = "The Lost Kingdom awakens from darkness! Heroic +220 Score and +80 Coins!";
        purify.scoreDelta = 220;
        purify.coinsDelta = 80;

        sc.options = {bind, purify};
    }
    return sc;
}

DecisionRecord DecisionSystem::evaluateChoice(const DecisionScenario& scenario, std::size_t optionIndex) {
    if (optionIndex >= scenario.options.size()) {
        optionIndex = 0;
    }
    const DecisionOption& opt = scenario.options[optionIndex];
    if (opt.grantsBonusClue) {
        bonusClueUnlocked_ = true;
        bonusClueText_ = opt.bonusClueText;
    }
    pendingTimeBonus_ += opt.extraTimeDelta;

    DecisionRecord rec;
    rec.levelNumber = scenario.levelNumber;
    rec.scenarioTitle = scenario.title;
    rec.choiceMade = opt.label;
    rec.consequenceSummary = opt.consequenceText;
    rec.scoreDelta = opt.scoreDelta;
    rec.coinsDelta = opt.coinsDelta;
    rec.extraTimeDelta = opt.extraTimeDelta;
    return rec;
}

} // namespace qc
