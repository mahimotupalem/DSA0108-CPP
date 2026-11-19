# Module Description

## Module 1 – Player Management and Game Environment Setup
1. **Player Registration (`Player::registerPlayer`)**: Validates player names (2–20 alphanumeric/space characters) and generates a collision-resistant ID (`QC-XXX-####`).
2. **Game Menu Interface (`GameMenu`)**: Full SFML graphical state machine covering 12+ interactive screens (Main Menu, Registration, Profile, Difficulty, Level Map, Gameplay, Decision Gate, Hint Overlay, Level Summary, Final Results, Leaderboard, Instructions & Settings).
3. **Player Profile Manager (`Player`)**: Tracks ID, name, difficulty, unlocked/completed levels, best score, coins, stars, keys, and earned achievements.
4. **Difficulty Level Selector (`getDifficultySettings`)**: Scales starting lives (5/3/2), level timer (90s/60s/45s), memory sequence length (4/5/7), hint penalties (15/25/40), and score multipliers (1.0x/1.5x/2.0x).
5. **Progress Storage Manager (`SaveManager`)**: Performs CSV upsert operations on `players.csv` and `progress.csv` and restores progress on application restart.

## Module 2 – Puzzle Challenges, Game Interaction and Performance Analysis
1. **Puzzle Engine (`PuzzleEngine`)**: Generates dynamic memory rune sequences, algebraic equations, numerical progressions, and multi-clue forest riddles.
2. **Level Manager (`LevelManager`)**: Manages all 5 playable chambers (*Memory Temple*, *Logic Chamber*, *Mystery Forest*, *Decision Gate*, *Crystal Challenge*) and sequential unlocking.
3. **Decision Impact System (`DecisionSystem`)**: Presents branching choices in Level 4 (*Golden Door* vs *Secret Passage* vs *Wisdom Altar*) that alter score, coins, Level 5 timer duration, and unlock secret clues, while logging choices for the Final Performance Report.
4. **Hint & Timer Systems (`HintSystem`, `GameMenu::update`)**: Tracks per-level hints with immediate point deductions and real-time countdown timers.
5. **Score, Result & Leaderboard Managers (`ScoreManager`, `Leaderboard`)**: Computes base points, time bonuses, penalties, accuracy percentages, ranks players by score, and persists `leaderboard.csv` and `game_history.csv`.
