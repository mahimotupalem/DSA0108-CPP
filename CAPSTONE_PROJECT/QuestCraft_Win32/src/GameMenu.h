#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include "Types.h"
#include "WinUI.h"
#include "Player.h"
#include "LevelManager.h"
#include "PuzzleEngine.h"
#include "DecisionSystem.h"
#include "HintSystem.h"
#include "ScoreManager.h"
#include "Leaderboard.h"
#include "SaveManager.h"
#include <vector>
#include <string>

namespace qc {

class GameMenu {
public:
    GameMenu();
    ~GameMenu();

    int run(HINSTANCE hInstance, int nCmdShow);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void initFonts();
    void setupMainMenuUI();
    void setupRegistrationUI();
    void setupDifficultyUI();
    void setupLevelSelectUI();
    void setupGameplayUI();
    void setupDecisionUI();
    void setupSubScreenButtons();

    void handleMouseMove(POINT pt);
    void handleMouseClick(POINT pt);
    void handleKeyDown(WPARAM key);
    void handleCharInput(wchar_t ch);
    void onTimerTick(float dt);

    void beginLevel(int levelNumber);
    void loadCurrentChallengeIntoUI();
    void submitPlayerAnswer(const std::string& answer);
    void advanceChallengeOrFinishLevel();
    void recordRunCompletionAndPersist();

    void spawnParticleBurst(float x, float y, int count, COLORREF baseColor);
    void spawnFloatingPopup(const std::string& text, float x, float y, COLORREF color);
    void triggerScreenShake(float duration = 0.25f);
    void playVictoryFanfare();
    void playCorrectChime();
    void playErrorTone();
    void playClickTone();
    std::string getHeroRankTitle() const;
    std::string getGuardianName(int levelNum) const;

    void renderFrame(HDC hdc);
    void renderBackgroundAndHeader(HDC hdc, const std::string& pageTitle);
    void renderMainMenu(HDC hdc);
    void renderRegistration(HDC hdc);
    void renderProfile(HDC hdc);
    void renderDifficulty(HDC hdc);
    void renderLevelSelection(HDC hdc);
    void renderGameplay(HDC hdc);
    void renderDecisionSelection(HDC hdc);
    void renderHintOverlay(HDC hdc);
    void renderLevelCompletion(HDC hdc);
    void renderFinalResults(HDC hdc);
    void renderLeaderboard(HDC hdc);
    void renderInstructions(HDC hdc);
    void renderParticlesAndPopups(HDC hdc);

    HWND hwnd_ = nullptr;
    HFONT titleFont_ = nullptr;
    HFONT headingFont_ = nullptr;
    HFONT bodyFont_ = nullptr;
    HFONT smallFont_ = nullptr;

    ScreenState state_ = ScreenState::MainMenu;
    float elapsedSec_ = 0.0f;
    float shakeTimer_ = 0.0f;
    bool soundEnabled_ = true;
    std::string statusBanner_;
    COLORREF statusColor_ = RGB(120, 235, 165);

    std::vector<Particle> particles_;
    std::vector<FloatingPopup> popups_;
    int answerStreak_ = 0;
    int maxStreak_ = 0;
    bool royalChestClaimed_ = false;
    bool freezeTimeUsedInLevel_ = false;

    // Relic Inventory (purchasable with Gold Coins)
    bool hasAegisShield_ = true;   // Starts with 1 royal shield!
    int oracleLenses_ = 1;         // Free hint lenses

    Player player_;
    LevelManager levelManager_;
    PuzzleEngine puzzleEngine_;
    DecisionSystem decisionSystem_;
    HintSystem hintSystem_;
    ScoreManager scoreManager_;
    Leaderboard leaderboard_;
    SaveManager saveManager_;

    std::vector<UIButton> mainMenuButtons_;
    UIButton royalChestBtn_;
    UIButton buyShieldBtn_;
    UIButton buyLensBtn_;
    UIButton backButton_;

    UITextInput nameInput_;
    UIButton registerSubmitBtn_;
    UIButton loadSavedBtn_;
    std::string registrationFeedback_;

    std::vector<UIButton> difficultyButtons_;
    std::vector<UIButton> levelButtons_;
    UIButton viewFinalReportBtn_;

    int activeLevelNumber_ = 1;
    std::vector<PuzzleChallenge> activeChallenges_;
    std::size_t currentChallengeIdx_ = 0;
    float levelTimeRemaining_ = 60.0f;
    float levelMaxTime_ = 60.0f;
    bool levelFailed_ = false;
    std::string guardianSpeech_;

    bool memoryRevealPhase_ = false;
    float memoryRevealTimer_ = 0.0f;
    std::vector<int> playerMemoryInput_;
    std::vector<UIButton> runeButtons_;
    UIButton clearMemoryBtn_;

    UITextInput answerInput_;
    UIButton submitAnswerBtn_;
    UIButton requestHintBtn_;
    UIButton freezeTimeBtn_;
    std::vector<UIButton> mcOptionButtons_;
    UIButton dismissHintBtn_;

    DecisionScenario activeDecisionScenario_;
    std::vector<UIButton> decisionOptionButtons_;
    std::string lastDecisionConsequence_;

    UIButton nextLevelBtn_;
    UIButton retryLevelBtn_;
    UIButton toLevelSelectBtn_;
    UIButton playAgainBtn_;

    UIButton toggleSoundBtn_;
    UIButton resetSaveBtn_;
};

} // namespace qc
