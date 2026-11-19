# Test Cases & Verification Report

## Automated Logic Verification Status
- **Verified in Linux GCC C++17 Environment:** Core C++17 modules (`Player`, `ScoreManager`, `HintSystem`, `DecisionSystem`, `PuzzleEngine`, `LevelManager`, `Leaderboard`, `SaveManager`) were compiled with `-std=c++17 -Wall -Wextra` and executed against automated assertions for **TC01–TC10**. All 10 logic & CSV persistence tests passed with exit code `0`.
- **Unverified in Linux Sandbox (Requires Windows SFML Runtime):** Interactive SFML window rendering, mouse/keyboard GUI event loops, and WAV audio playback (`GameMenu.cpp`, `UIUtils.cpp`) were syntax/type-checked cleanly (`0` warnings/errors) against SFML headers, and must be executed on Windows 10/11 with SFML 2.6.1 installed.

## Capstone Test Case Matrix

| Test ID | Feature | Test Input | Expected Output | Verification Status |
| :--- | :--- | :--- | :--- | :--- |
| **TC01** | Player Registration | Valid name `"Aria Storm"` (and invalid blank `"   "`) | Rejects blank input; creates unique ID `QC-ARI-####` and saves profile | **PASS** (Automated C++ test) |
| **TC02** | Difficulty Selection | Select `Easy`, `Medium`, `Hard` | Updates lives (`5`/`3`/`2`), timer (`90s`/`60s`/`45s`), and multipliers | **PASS** (Automated C++ test) |
| **TC03** | Puzzle Validation | Submit exact expected rune/number/choice answer | Answer accepted, score increased, advances to next challenge | **PASS** (Automated C++ test) |
| **TC04** | Puzzle Validation | Submit wrong answer `"999999"` | Incorrect message displayed, 1 life & penalty points deducted | **PASS** (Automated C++ test) |
| **TC05** | Hint System | Click `Hint` button during active puzzle | Hint overlay shown, hint count incremented, penalty deducted | **PASS** (Automated C++ test) |
| **TC06** | Decision System | Select `Golden Door` vs `Secret Passage` in Level 4 | Applies distinct score/coin/time/clue consequences and logs decision | **PASS** (Automated C++ test) |
| **TC07** | Level Unlocking | Complete Level 1 successfully | Level 1 marked completed; Level 2 unlocked on Quest Map | **PASS** (Automated C++ test) |
| **TC08** | Score Calculation | Complete chamber with time remaining & penalties | Calculates `Base + TimeBonus + DecisionBonus - Penalties` accurately | **PASS** (Automated C++ test) |
| **TC09** | Progress Saving | Close application and reopen | Restores player profile, unlocked levels, coins, stars, and best score | **PASS** (Automated C++ test) |
| **TC10** | Leaderboard | Save multiple players with varying scores | Sorts descending by score and assigns ranks `#1`, `#2`, etc. | **PASS** (Automated C++ test) |
| **TC11** | Timer | Start Level 1 challenge and wait | Countdown decrements each second; triggers fail screen at `0s` | Verified in code; test GUI on Windows |
| **TC12** | Final Results | Complete Level 5 (*Crystal Challenge*) | Displays total score, accuracy %, time, rewards, and decision impact log | Verified in code; test GUI on Windows |
