# Capstone Live Demonstration Guide (5–7 Minutes)

Follow this step-by-step sequence during your faculty capstone review:

1. **Launch QuestCraft (0:00–0:30):** Run `run.bat` to open the 1280x800 SFML window showing the animated Magical Crystal emblem and main menu.
2. **Main Menu Walkthrough (0:30–1:00):** Point out the 7 menu options and the bottom status bar showing persisted session state.
3. **Register a Player (1:00–1:30):** Click **Register / Switch Hero**, enter a reviewer's name (e.g., `Dr. Kumar`), and press **Enter** to generate a unique `QC-XXX-####` ID saved to `data/players.csv`.
4. **Select Difficulty (1:30–2:00):** Choose **Medium (3 Lives | 60s | 1.5x Multiplier)** and show the Kingdom Quest Map where Level 1 is `[UNLOCKED]` and Levels 2–5 are `[LOCKED]`.
5. **Level 1 – Memory Temple (2:00–2:45):** Watch the glowing rune sequence, let it hide, and click the rune buttons in exact order to clear the chamber.
6. **Level 2 – Logic Chamber (2:45–3:30):** Demonstrate the **Hint System** by clicking `Hint (-25 pts)` to view the Oracle's Hint and observe the live score deduction, then enter the correct equation value.
7. **Level 3 – Mystery Forest (3:30–4:00):** Solve the multi-clue forest deduction riddles.
8. **Level 4 – Decision Impact System (4:00–4:45):** Choose between the **Golden Door** (+150 Honor Score, +60 Coins) and the **Secret Passage** (+15s extra time for Level 5 + Archmage Scroll clue), then complete the chamber.
9. **Level 5 – Crystal Challenge & Final Report (4:45–5:45):** Complete the hybrid finale to reclaim the Magical Crystal and open the **Final Kingdom Performance & Decision Report**, highlighting accuracy %, completion time, rewards, and the recorded decision impact.
10. **Leaderboard & Persistence Proof (5:45–6:30):** Open **Leaderboard** to show ranked scores, close the window, relaunch `run.bat`, and show that the hero's profile, best score, and unlocked levels automatically restore from `data/progress.csv`.
