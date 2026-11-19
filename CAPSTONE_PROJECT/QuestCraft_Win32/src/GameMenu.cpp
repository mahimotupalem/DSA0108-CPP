#include "GameMenu.h"
#include <windowsx.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

namespace qc {

namespace {
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 800;
constexpr UINT_PTR kTimerId = 1;
} // namespace

GameMenu::GameMenu() : saveManager_("data") {
    initFonts();
    saveManager_.loadLeaderboard(leaderboard_);
    Player loaded;
    if (saveManager_.loadMostRecentPlayer(loaded)) {
        player_ = loaded;
        statusBanner_ = "Welcome back, " + player_.getName() + " (" + player_.getPlayerId() + ")! Progress restored.";
    } else {
        statusBanner_ = "Welcome to QuestCraft! Register your hero to begin the Quest for the Magical Crystal.";
    }

    setupMainMenuUI();
    setupRegistrationUI();
    setupDifficultyUI();
    setupLevelSelectUI();
    setupGameplayUI();
    setupDecisionUI();
    setupSubScreenButtons();
}

GameMenu::~GameMenu() {
    if (titleFont_) DeleteObject(titleFont_);
    if (headingFont_) DeleteObject(headingFont_);
    if (bodyFont_) DeleteObject(bodyFont_);
    if (smallFont_) DeleteObject(smallFont_);
}

void GameMenu::initFonts() {
    titleFont_ = CreateFontA(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
    headingFont_ = CreateFontA(23, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                               OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
    bodyFont_ = CreateFontA(19, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                            OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
    smallFont_ = CreateFontA(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
}

void GameMenu::playClickTone() {
    if (soundEnabled_) Beep(640, 35);
}

void GameMenu::playCorrectChime() {
    if (soundEnabled_) {
        Beep(523, 50);
        Beep(659, 50);
        Beep(784, 80);
    }
}

void GameMenu::playErrorTone() {
    if (soundEnabled_) {
        Beep(240, 85);
        Beep(185, 125);
    }
}

void GameMenu::playVictoryFanfare() {
    if (soundEnabled_) {
        Beep(523, 65);
        Beep(659, 65);
        Beep(784, 65);
        Beep(1046, 150);
    }
}

void GameMenu::triggerScreenShake(float duration) {
    shakeTimer_ = duration;
}

std::string GameMenu::getHeroRankTitle() const {
    int s = std::max(player_.getBestScore(), player_.getCurrentScore());
    int done = static_cast<int>(player_.getCompletedLevels().size());
    if (done >= 5 && s >= 1500) return "Grand Archmage of Eldoria";
    if (done >= 4) return "Royal Crystal Guardian";
    if (done >= 2 || s >= 500) return "Knight of the Silver Rune";
    return "Apprentice Seeker";
}

std::string GameMenu::getGuardianName(int levelNum) const {
    switch (levelNum) {
        case 1: return "Oracle Lyra (Keeper of Runes)";
        case 2: return "Archon Vael (Master of Seals)";
        case 3: return "Sylvan Warden (Forest Spirit)";
        case 4: return "Arbiter Solis (Gatekeeper)";
        case 5: return "Malakor (Shadow Sorcerer)";
    }
    return "Chamber Guardian";
}

void GameMenu::spawnParticleBurst(float x, float y, int count, COLORREF baseColor) {
    for (int i = 0; i < count; ++i) {
        float angle = (6.283185f * static_cast<float>(i)) / static_cast<float>(count) +
                      (static_cast<float>(std::rand() % 100) / 100.0f) * 0.4f;
        float speed = 60.0f + static_cast<float>(std::rand() % 165);
        Particle p;
        p.x = x;
        p.y = y;
        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed - 30.0f;
        p.maxLife = 0.75f + (static_cast<float>(std::rand() % 50) / 100.0f);
        p.life = p.maxLife;
        p.size = 3 + (std::rand() % 4);
        p.color = baseColor;
        particles_.push_back(p);
    }
}

void GameMenu::spawnFloatingPopup(const std::string& text, float x, float y, COLORREF color) {
    FloatingPopup fp;
    fp.text = text;
    fp.x = x;
    fp.y = y;
    fp.vy = -42.0f;
    fp.life = 1.6f;
    fp.color = color;
    popups_.push_back(fp);
}

void GameMenu::setupMainMenuUI() {
    std::vector<std::string> labels = {
        "Start Quest",
        "Register / Switch Hero",
        "Player Profile & Badges",
        "Select Difficulty",
        "Hall of Heroes (Leaderboard)",
        "Instructions & Settings",
        "Exit Kingdom"
    };
    mainMenuButtons_.resize(labels.size());
    for (std::size_t i = 0; i < labels.size(); ++i) {
        mainMenuButtons_[i].setup(labels[i], 460, 200 + static_cast<int>(i) * 66, 360, 52);
    }
    royalChestBtn_.setup("Claim Daily Chest (+35 Coins)", 905, 515, 290, 44);
    royalChestBtn_.normalFill = RGB(86, 62, 22);
    royalChestBtn_.hoverFill = RGB(126, 92, 32);

    buyShieldBtn_.setup("Buy Aegis Shield (40 Coins)", 905, 568, 290, 42);
    buyShieldBtn_.normalFill = RGB(28, 66, 98);
    buyShieldBtn_.hoverFill = RGB(42, 95, 138);

    buyLensBtn_.setup("Buy Oracle Lens (30 Coins)", 905, 618, 290, 42);
    buyLensBtn_.normalFill = RGB(48, 52, 98);
    buyLensBtn_.hoverFill = RGB(72, 78, 138);
}

void GameMenu::setupRegistrationUI() {
    nameInput_.setup("Enter Hero Name (e.g., Aria Storm)...", 390, 320, 500, 52);
    registerSubmitBtn_.setup("Create Hero & Generate ID", 390, 395, 500, 52);
    loadSavedBtn_.setup("Restore Last Saved Hero Profile", 390, 465, 500, 50);
}

void GameMenu::setupDifficultyUI() {
    difficultyButtons_.resize(3);
    difficultyButtons_[0].setup("EASY  (5 Lives | 90s | Gentle Penalties)", 340, 260, 600, 64);
    difficultyButtons_[1].setup("MEDIUM  (3 Lives | 60s | 1.5x Score Multiplier)", 340, 350, 600, 64);
    difficultyButtons_[2].setup("HARD  (2 Lives | 45s | 2.0x Score Multiplier)", 340, 440, 600, 64);
}

void GameMenu::setupLevelSelectUI() {
    levelButtons_.resize(5);
    // Positioned as an interactive 2D Kingdom Overworld Map!
    int coords[5][2] = {
        {110, 470}, // Lv 1: Memory Temple (Southwest)
        {345, 315}, // Lv 2: Logic Chamber (Northwest)
        {575, 485}, // Lv 3: Mystery Forest (Central Valley)
        {805, 315}, // Lv 4: Decision Gate (Highland Pass)
        {1015, 455} // Lv 5: Crystal Sanctum (Royal Citadel)
    };
    for (int i = 0; i < 5; ++i) {
        levelButtons_[i].setup("Enter Chamber " + std::to_string(i + 1), coords[i][0] - 85, coords[i][1] + 58, 170, 40);
    }
    viewFinalReportBtn_.setup("View Kingdom Performance & Decision Report", 420, 665, 440, 48);
}

void GameMenu::setupGameplayUI() {
    answerInput_.setup("Type integer answer...", 340, 495, 400, 52);
    answerInput_.numericOnly = true;
    submitAnswerBtn_.setup("Submit Answer", 760, 495, 200, 52);
    requestHintBtn_.setup("Use Hint", 935, 118, 190, 42);
    freezeTimeBtn_.setup("Chrono Freeze (+15s | 25 Coins)", 680, 118, 240, 42);
    freezeTimeBtn_.normalFill = RGB(24, 68, 96);
    freezeTimeBtn_.hoverFill = RGB(36, 98, 138);
    dismissHintBtn_.setup("Got It - Return to Puzzle", 480, 495, 320, 50);

    runeButtons_.resize(6);
    for (int i = 0; i < 6; ++i) {
        std::string label = std::to_string(i + 1) + ": " + PuzzleEngine::getRuneName(i);
        runeButtons_[i].setup(label, 175 + i * 158, 455, 142, 74, i);
    }
    clearMemoryBtn_.setup("Clear Rune Input", 530, 545, 220, 44);

    mcOptionButtons_.resize(4);
    for (int i = 0; i < 4; ++i) {
        mcOptionButtons_[i].setup("Option", 270, 415 + i * 56, 740, 46);
    }
}

void GameMenu::setupDecisionUI() {
    decisionOptionButtons_.resize(3);
    for (int i = 0; i < 3; ++i) {
        decisionOptionButtons_[i].setup("Choice", 210, 325 + i * 105, 860, 52);
    }
}

void GameMenu::setupSubScreenButtons() {
    backButton_.setup("< Main Menu", 36, 26, 170, 44);
    nextLevelBtn_.setup("Continue to Next Chamber >", 440, 565, 400, 52);
    retryLevelBtn_.setup("Retry This Chamber", 440, 565, 400, 52);
    toLevelSelectBtn_.setup("Return to Kingdom Map", 440, 632, 400, 48);
    playAgainBtn_.setup("Start New Quest Run", 440, 645, 400, 52);

    toggleSoundBtn_.setup("Sound Effects: ON", 440, 520, 400, 48);
    resetSaveBtn_.setup("Reset Current Session Run", 440, 582, 400, 48);
}

int GameMenu::run(HINSTANCE hInstance, int nCmdShow) {
    WNDCLASSA wc{};
    wc.lpfnWndProc = GameMenu::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "QuestCraftWin32Window";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassA(&wc);

    RECT wr = {0, 0, kWindowWidth, kWindowHeight};
    AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

    hwnd_ = CreateWindowA(
        wc.lpszClassName,
        "QuestCraft - The Lost Kingdom & Magical Crystal (C++17 Edition)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, hInstance, this
    );

    if (!hwnd_) return 1;
    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);
    SetTimer(hwnd_, kTimerId, 33, nullptr);

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK GameMenu::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    GameMenu* self = nullptr;
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTA*>(lParam);
        self = static_cast<GameMenu*>(cs->lpCreateParams);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<GameMenu*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    }
    if (self) {
        return self->handleMessage(hwnd, msg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

LRESULT GameMenu::handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_TIMER:
            if (wParam == kTimerId) {
                onTimerTick(0.033f);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_MOUSEMOVE: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            handleMouseMove(pt);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            handleMouseClick(pt);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_CHAR:
            handleCharInput(static_cast<wchar_t>(wParam));
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_KEYDOWN:
            handleKeyDown(wParam);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBmp = CreateCompatibleBitmap(hdc, kWindowWidth, kWindowHeight);
            HGDIOBJ oldBmp = SelectObject(memDC, memBmp);

            renderFrame(memDC);

            int shakeX = 0, shakeY = 0;
            if (shakeTimer_ > 0.0f) {
                shakeX = (std::rand() % 11) - 5;
                shakeY = (std::rand() % 11) - 5;
            }
            BitBlt(hdc, shakeX, shakeY, kWindowWidth, kWindowHeight, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldBmp);
            DeleteObject(memBmp);
            DeleteDC(memDC);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            KillTimer(hwnd, kTimerId);
            saveManager_.savePlayerProfile(player_);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void GameMenu::handleMouseMove(POINT pt) {
    backButton_.updateHover(pt);
    switch (state_) {
        case ScreenState::MainMenu:
            for (auto& b : mainMenuButtons_) b.updateHover(pt);
            royalChestBtn_.enabled = player_.isRegistered() && !royalChestClaimed_;
            royalChestBtn_.updateHover(pt);
            buyShieldBtn_.enabled = player_.isRegistered() && !hasAegisShield_ && player_.getCoins() >= 40;
            buyShieldBtn_.updateHover(pt);
            buyLensBtn_.enabled = player_.isRegistered() && player_.getCoins() >= 30;
            buyLensBtn_.updateHover(pt);
            break;
        case ScreenState::PlayerRegistration:
            registerSubmitBtn_.updateHover(pt);
            loadSavedBtn_.updateHover(pt);
            break;
        case ScreenState::DifficultySelection:
            for (auto& b : difficultyButtons_) b.updateHover(pt);
            break;
        case ScreenState::LevelSelection:
            for (std::size_t i = 0; i < levelButtons_.size(); ++i) {
                levelButtons_[i].enabled = player_.isLevelUnlocked(static_cast<int>(i + 1));
                levelButtons_[i].updateHover(pt);
            }
            viewFinalReportBtn_.updateHover(pt);
            break;
        case ScreenState::PuzzleGameplay:
            requestHintBtn_.updateHover(pt);
            freezeTimeBtn_.enabled = (!freezeTimeUsedInLevel_ && player_.getCoins() >= 25);
            freezeTimeBtn_.updateHover(pt);
            submitAnswerBtn_.updateHover(pt);
            clearMemoryBtn_.updateHover(pt);
            for (auto& rb : runeButtons_) rb.updateHover(pt);
            for (auto& mb : mcOptionButtons_) mb.updateHover(pt);
            break;
        case ScreenState::HintOverlay:
            dismissHintBtn_.updateHover(pt);
            break;
        case ScreenState::DecisionSelection:
            for (auto& db : decisionOptionButtons_) db.updateHover(pt);
            break;
        case ScreenState::LevelCompletion:
            nextLevelBtn_.updateHover(pt);
            retryLevelBtn_.updateHover(pt);
            toLevelSelectBtn_.updateHover(pt);
            break;
        case ScreenState::FinalResults:
            playAgainBtn_.updateHover(pt);
            break;
        case ScreenState::InstructionsScreen:
        case ScreenState::SettingsScreen:
            toggleSoundBtn_.updateHover(pt);
            resetSaveBtn_.updateHover(pt);
            break;
        default:
            break;
    }
}

void GameMenu::handleCharInput(wchar_t ch) {
    if (state_ == ScreenState::PlayerRegistration) {
        nameInput_.handleChar(ch);
    } else if (state_ == ScreenState::PuzzleGameplay && !memoryRevealPhase_ &&
               currentChallengeIdx_ < activeChallenges_.size()) {
        const auto& c = activeChallenges_[currentChallengeIdx_];
        if (c.type == PuzzleType::NumberLogic || c.type == PuzzleType::PatternRecognition) {
            answerInput_.handleChar(ch);
        }
    }
}

void GameMenu::handleKeyDown(WPARAM key) {
    if (key == VK_RETURN) {
        if (state_ == ScreenState::PlayerRegistration) {
            std::string err;
            if (player_.registerPlayer(nameInput_.value, err)) {
                saveManager_.savePlayerProfile(player_);
                scoreManager_.resetRunStats();
                decisionSystem_.reset();
                answerStreak_ = 0;
                hasAegisShield_ = true;
                oracleLenses_ = 1;
                registrationFeedback_ = "Registered! ID: " + player_.getPlayerId();
                statusBanner_ = "Hero " + player_.getName() + " [" + player_.getPlayerId() + "] ready with Aegis Shield & Oracle Lens!";
                spawnParticleBurst(640.0f, 380.0f, 28, RGB(255, 215, 85));
                playVictoryFanfare();
                state_ = ScreenState::DifficultySelection;
            } else {
                registrationFeedback_ = err;
                playErrorTone();
            }
        } else if (state_ == ScreenState::PuzzleGameplay && !memoryRevealPhase_ &&
                   currentChallengeIdx_ < activeChallenges_.size()) {
            const auto& c = activeChallenges_[currentChallengeIdx_];
            if (c.type == PuzzleType::NumberLogic || c.type == PuzzleType::PatternRecognition) {
                submitPlayerAnswer(answerInput_.value);
            }
        }
    } else if (key == VK_ESCAPE) {
        if (state_ == ScreenState::HintOverlay) {
            hintSystem_.dismissHint();
            state_ = ScreenState::PuzzleGameplay;
        } else if (state_ != ScreenState::MainMenu) {
            state_ = ScreenState::MainMenu;
        }
    }
}

void GameMenu::handleMouseClick(POINT pt) {
    if (state_ != ScreenState::MainMenu && state_ != ScreenState::HintOverlay && backButton_.isClicked(pt)) {
        playClickTone();
        saveManager_.savePlayerProfile(player_);
        state_ = ScreenState::MainMenu;
        return;
    }

    switch (state_) {
        case ScreenState::MainMenu: {
            if ((pt.x > 220 && pt.x < 350 && pt.y > 350 && pt.y < 490) ||
                (pt.x > 930 && pt.x < 1060 && pt.y > 350 && pt.y < 490)) {
                spawnParticleBurst(static_cast<float>(pt.x), static_cast<float>(pt.y), 22, RGB(95, 225, 255));
                spawnFloatingPopup("Crystal Resonance!", static_cast<float>(pt.x - 60), static_cast<float>(pt.y - 20), RGB(125, 235, 255));
                playCorrectChime();
                return;
            }

            if (royalChestBtn_.isClicked(pt)) {
                royalChestClaimed_ = true;
                player_.addCoins(35);
                player_.addAchievement("Royal Treasury Benefactor");
                saveManager_.savePlayerProfile(player_);
                spawnParticleBurst(1050.0f, 515.0f, 30, RGB(255, 215, 65));
                spawnFloatingPopup("+35 GOLD COINS!", 980.0f, 480.0f, RGB(255, 225, 95));
                statusBanner_ = "Daily Royal Chest opened! +35 Gold Coins added.";
                playVictoryFanfare();
                return;
            }

            if (buyShieldBtn_.isClicked(pt) && !hasAegisShield_ && player_.getCoins() >= 40) {
                player_.addCoins(-40);
                hasAegisShield_ = true;
                saveManager_.savePlayerProfile(player_);
                spawnParticleBurst(1050.0f, 568.0f, 24, RGB(85, 205, 255));
                spawnFloatingPopup("AEGIS SHIELD EQUIPPED!", 950.0f, 540.0f, RGB(115, 225, 255));
                statusBanner_ = "Aegis Shield equipped! It will block your next wrong answer.";
                playCorrectChime();
                return;
            }

            if (buyLensBtn_.isClicked(pt) && player_.getCoins() >= 30) {
                player_.addCoins(-30);
                oracleLenses_++;
                saveManager_.savePlayerProfile(player_);
                spawnParticleBurst(1050.0f, 618.0f, 24, RGB(185, 145, 255));
                spawnFloatingPopup("+1 ORACLE LENS!", 980.0f, 590.0f, RGB(205, 175, 255));
                statusBanner_ = "Oracle Lens crafted! Use it for a penalty-free hint in any chamber.";
                playCorrectChime();
                return;
            }

            for (std::size_t i = 0; i < mainMenuButtons_.size(); ++i) {
                if (!mainMenuButtons_[i].isClicked(pt)) continue;
                playClickTone();
                if (i == 0) {
                    if (!player_.isRegistered()) {
                        registrationFeedback_ = "Please register your hero name first!";
                        state_ = ScreenState::PlayerRegistration;
                    } else {
                        state_ = ScreenState::LevelSelection;
                    }
                } else if (i == 1) {
                    registrationFeedback_.clear();
                    state_ = ScreenState::PlayerRegistration;
                } else if (i == 2) {
                    state_ = ScreenState::PlayerProfile;
                } else if (i == 3) {
                    state_ = ScreenState::DifficultySelection;
                } else if (i == 4) {
                    saveManager_.loadLeaderboard(leaderboard_);
                    state_ = ScreenState::LeaderboardScreen;
                } else if (i == 5) {
                    state_ = ScreenState::InstructionsScreen;
                } else if (i == 6) {
                    saveManager_.savePlayerProfile(player_);
                    DestroyWindow(hwnd_);
                }
                break;
            }
            break;
        }
        case ScreenState::PlayerRegistration: {
            if (registerSubmitBtn_.isClicked(pt)) {
                std::string err;
                if (player_.registerPlayer(nameInput_.value, err)) {
                    saveManager_.savePlayerProfile(player_);
                    scoreManager_.resetRunStats();
                    decisionSystem_.reset();
                    answerStreak_ = 0;
                    hasAegisShield_ = true;
                    oracleLenses_ = 1;
                    registrationFeedback_ = "Hero Created! ID: " + player_.getPlayerId();
                    statusBanner_ = "Active Hero: " + player_.getName() + " (" + player_.getPlayerId() + ")";
                    spawnParticleBurst(640.0f, 380.0f, 28, RGB(255, 215, 85));
                    playVictoryFanfare();
                    state_ = ScreenState::DifficultySelection;
                } else {
                    registrationFeedback_ = err;
                    playErrorTone();
                }
            } else if (loadSavedBtn_.isClicked(pt)) {
                Player loaded;
                if (saveManager_.loadMostRecentPlayer(loaded)) {
                    player_ = loaded;
                    registrationFeedback_ = "Restored " + player_.getName() + " (" + player_.getPlayerId() + ")!";
                    statusBanner_ = "Restored Hero: " + player_.getName() + " | Best Score: " + std::to_string(player_.getBestScore());
                    spawnParticleBurst(640.0f, 465.0f, 22, RGB(120, 240, 175));
                    playCorrectChime();
                } else {
                    registrationFeedback_ = "No previous save record found in data/progress.csv.";
                    playErrorTone();
                }
            }
            break;
        }
        case ScreenState::DifficultySelection: {
            for (std::size_t i = 0; i < difficultyButtons_.size(); ++i) {
                if (difficultyButtons_[i].isClicked(pt)) {
                    playClickTone();
                    Difficulty chosen = static_cast<Difficulty>(i);
                    player_.setDifficulty(chosen);
                    saveManager_.savePlayerProfile(player_);
                    statusBanner_ = "Difficulty set to " + difficultyToString(chosen) + ".";
                    state_ = player_.isRegistered() ? ScreenState::LevelSelection : ScreenState::MainMenu;
                    break;
                }
            }
            break;
        }
        case ScreenState::LevelSelection: {
            for (std::size_t i = 0; i < levelButtons_.size(); ++i) {
                int lvlNum = static_cast<int>(i + 1);
                if (levelButtons_[i].isClicked(pt) && player_.isLevelUnlocked(lvlNum)) {
                    playClickTone();
                    beginLevel(lvlNum);
                    return;
                }
            }
            if (viewFinalReportBtn_.isClicked(pt)) {
                playClickTone();
                state_ = ScreenState::FinalResults;
            }
            break;
        }
        case ScreenState::PuzzleGameplay: {
            if (currentChallengeIdx_ >= activeChallenges_.size()) break;
            const auto& ch = activeChallenges_[currentChallengeIdx_];

            if (freezeTimeBtn_.isClicked(pt) && !freezeTimeUsedInLevel_ && player_.getCoins() >= 25) {
                freezeTimeUsedInLevel_ = true;
                player_.addCoins(-25);
                levelTimeRemaining_ += 15.0f;
                levelMaxTime_ = std::max(levelMaxTime_, levelTimeRemaining_);
                spawnParticleBurst(800.0f, 138.0f, 24, RGB(95, 225, 255));
                spawnFloatingPopup("+15s CHRONO FREEZE!", 720.0f, 165.0f, RGB(115, 235, 255));
                statusBanner_ = "Chrono Freeze activated! +15 seconds added to chamber timer.";
                statusColor_ = RGB(115, 235, 255);
                playCorrectChime();
                return;
            }

            if (requestHintBtn_.isClicked(pt)) {
                if (oracleLenses_ > 0) {
                    oracleLenses_--;
                    hintSystem_.requestHint("[FREE ORACLE LENS] " + ch.hint);
                    spawnFloatingPopup("FREE LENS HINT!", 960.0f, 165.0f, RGB(175, 225, 255));
                } else if (hintSystem_.canRequestHint()) {
                    scoreManager_.recordHintUsed();
                    player_.addScore(-hintSystem_.getCurrentPenaltyCost());
                    hintSystem_.requestHint(ch.hint);
                    spawnFloatingPopup("-" + std::to_string(hintSystem_.getCurrentPenaltyCost()) + " PTS", 970.0f, 165.0f, RGB(255, 195, 95));
                } else {
                    hintSystem_.requestHint(ch.hint);
                }
                playClickTone();
                state_ = ScreenState::HintOverlay;
                return;
            }

            if (ch.type == PuzzleType::MemorySequence) {
                if (memoryRevealPhase_) return;
                for (std::size_t i = 0; i < runeButtons_.size(); ++i) {
                    if (runeButtons_[i].isClicked(pt)) {
                        playClickTone();
                        spawnParticleBurst(static_cast<float>(pt.x), static_cast<float>(pt.y), 10, RGB(130, 225, 255));
                        playerMemoryInput_.push_back(static_cast<int>(i));
                        if (playerMemoryInput_.size() == ch.memorySequence.size()) {
                            std::ostringstream oss;
                            for (std::size_t k = 0; k < playerMemoryInput_.size(); ++k) {
                                if (k > 0) oss << "-";
                                oss << (playerMemoryInput_[k] + 1);
                            }
                            submitPlayerAnswer(oss.str());
                        }
                        return;
                    }
                }
                if (clearMemoryBtn_.isClicked(pt)) {
                    playClickTone();
                    playerMemoryInput_.clear();
                }
            } else if (ch.isMultipleChoice) {
                for (std::size_t i = 0; i < mcOptionButtons_.size() && i < ch.choices.size(); ++i) {
                    if (mcOptionButtons_[i].isClicked(pt)) {
                        submitPlayerAnswer(std::to_string(i));
                        return;
                    }
                }
            } else {
                if (submitAnswerBtn_.isClicked(pt)) {
                    submitPlayerAnswer(answerInput_.value);
                }
            }
            break;
        }
        case ScreenState::HintOverlay: {
            if (dismissHintBtn_.isClicked(pt)) {
                playClickTone();
                hintSystem_.dismissHint();
                state_ = ScreenState::PuzzleGameplay;
            }
            break;
        }
        case ScreenState::DecisionSelection: {
            for (std::size_t i = 0; i < decisionOptionButtons_.size() && i < activeDecisionScenario_.options.size(); ++i) {
                if (decisionOptionButtons_[i].isClicked(pt)) {
                    DecisionRecord rec = decisionSystem_.evaluateChoice(activeDecisionScenario_, i);
                    player_.recordDecision(rec);
                    player_.addScore(rec.scoreDelta);
                    player_.addCoins(rec.coinsDelta);
                    scoreManager_.applyDecisionImpact(rec.scoreDelta, rec.coinsDelta);
                    lastDecisionConsequence_ = rec.consequenceSummary;
                    spawnParticleBurst(640.0f, 380.0f, 35, RGB(255, 215, 85));
                    spawnFloatingPopup("+" + std::to_string(rec.scoreDelta) + " HONOR SCORE!", 560.0f, 320.0f, RGB(255, 225, 105));
                    playVictoryFanfare();
                    loadCurrentChallengeIntoUI();
                    state_ = ScreenState::PuzzleGameplay;
                    return;
                }
            }
            break;
        }
        case ScreenState::LevelCompletion: {
            if (!levelFailed_ && nextLevelBtn_.isClicked(pt)) {
                playClickTone();
                if (activeLevelNumber_ < 5) {
                    beginLevel(activeLevelNumber_ + 1);
                } else {
                    state_ = ScreenState::FinalResults;
                }
            } else if (levelFailed_ && retryLevelBtn_.isClicked(pt)) {
                playClickTone();
                beginLevel(activeLevelNumber_);
            } else if (toLevelSelectBtn_.isClicked(pt)) {
                playClickTone();
                state_ = ScreenState::LevelSelection;
            }
            break;
        }
        case ScreenState::FinalResults: {
            if (playAgainBtn_.isClicked(pt)) {
                playClickTone();
                player_.resetCurrentRun();
                scoreManager_.resetRunStats();
                decisionSystem_.reset();
                answerStreak_ = 0;
                state_ = ScreenState::LevelSelection;
            }
            break;
        }
        case ScreenState::InstructionsScreen:
        case ScreenState::SettingsScreen: {
            if (toggleSoundBtn_.isClicked(pt)) {
                soundEnabled_ = !soundEnabled_;
                toggleSoundBtn_.setup(soundEnabled_ ? "Sound Effects: ON" : "Sound Effects: OFF", 440, 520, 400, 48);
                playClickTone();
            } else if (resetSaveBtn_.isClicked(pt)) {
                player_.resetCurrentRun();
                scoreManager_.resetRunStats();
                decisionSystem_.reset();
                answerStreak_ = 0;
                statusBanner_ = "Current run reset. Unlocked progress remains saved.";
                playClickTone();
            }
            break;
        }
        default:
            break;
    }
}

void GameMenu::beginLevel(int levelNumber) {
    activeLevelNumber_ = levelNumber;
    const auto& lvl = levelManager_.getLevel(levelNumber);
    Difficulty diff = player_.getDifficulty();
    DifficultySettings ds = getDifficultySettings(diff);

    if (player_.getLives() <= 0) {
        player_.setLives(ds.startingLives);
    }

    levelTimeRemaining_ = static_cast<float>(ds.baseTimeSeconds + decisionSystem_.getPendingTimeBonus());
    levelMaxTime_ = levelTimeRemaining_;
    decisionSystem_.consumePendingTimeBonus();
    levelFailed_ = false;
    freezeTimeUsedInLevel_ = false;
    lastDecisionConsequence_.clear();
    guardianSpeech_ = "\"Prove your intellect, seeker, if you wish to pass!\"";

    hintSystem_.resetForLevel(diff);
    scoreManager_.startLevel(levelNumber, lvl.name, diff);
    activeChallenges_ = puzzleEngine_.buildChallengesForLevel(levelNumber, diff, decisionSystem_.getBonusClueText());
    currentChallengeIdx_ = 0;

    if (lvl.hasDecisionPhase) {
        activeDecisionScenario_ = decisionSystem_.getScenarioForLevel(levelNumber, diff);
        for (std::size_t i = 0; i < decisionOptionButtons_.size(); ++i) {
            if (i < activeDecisionScenario_.options.size()) {
                decisionOptionButtons_[i].setup(activeDecisionScenario_.options[i].label, 210, 325 + static_cast<int>(i) * 105, 860, 52);
            }
        }
        state_ = ScreenState::DecisionSelection;
    } else {
        loadCurrentChallengeIntoUI();
        state_ = ScreenState::PuzzleGameplay;
    }
}

void GameMenu::loadCurrentChallengeIntoUI() {
    if (currentChallengeIdx_ >= activeChallenges_.size()) return;
    const auto& ch = activeChallenges_[currentChallengeIdx_];
    DifficultySettings ds = getDifficultySettings(player_.getDifficulty());

    answerInput_.value.clear();
    playerMemoryInput_.clear();

    if (ch.type == PuzzleType::MemorySequence) {
        memoryRevealPhase_ = true;
        memoryRevealTimer_ = ds.memoryShowSeconds;
    } else {
        memoryRevealPhase_ = false;
        memoryRevealTimer_ = 0.0f;
    }

    if (ch.isMultipleChoice) {
        for (std::size_t i = 0; i < mcOptionButtons_.size() && i < ch.choices.size(); ++i) {
            mcOptionButtons_[i].setup(ch.choices[i], 270, 415 + static_cast<int>(i) * 56, 740, 46);
        }
    }
}

void GameMenu::submitPlayerAnswer(const std::string& answer) {
    if (currentChallengeIdx_ >= activeChallenges_.size()) return;
    const auto& ch = activeChallenges_[currentChallengeIdx_];
    DifficultySettings ds = getDifficultySettings(player_.getDifficulty());

    if (puzzleEngine_.validateAnswer(ch, answer)) {
        playCorrectChime();
        answerStreak_++;
        if (answerStreak_ > maxStreak_) maxStreak_ = answerStreak_;

        int streakBonus = (answerStreak_ >= 2) ? (answerStreak_ * 25) : 0;
        scoreManager_.recordCorrectAnswer(140 + streakBonus);
        int gained = static_cast<int>((140 + streakBonus) * ds.scoreMultiplier);
        player_.addScore(gained);

        if (answerStreak_ >= 3) {
            player_.addAchievement("Unstoppable Mind (3+ Streak)");
        }

        guardianSpeech_ = "\"Impressive! A seal has been shattered!\"";
        spawnParticleBurst(220.0f, 265.0f, 28, RGB(115, 245, 165));
        std::string popMsg = "+" + std::to_string(gained) + " PTS";
        if (answerStreak_ >= 2) {
            popMsg += "  (" + std::to_string(answerStreak_) + "x STREAK!)";
        }
        spawnFloatingPopup(popMsg, 560.0f, 390.0f, RGB(125, 250, 175));

        statusBanner_ = "Correct! +" + std::to_string(gained) + " Points awarded!";
        statusColor_ = RGB(115, 235, 160);
        advanceChallengeOrFinishLevel();
    } else {
        triggerScreenShake(0.25f);
        answerStreak_ = 0;
        playerMemoryInput_.clear();
        answerInput_.value.clear();

        // Check if Aegis Shield absorbs the blow!
        if (hasAegisShield_) {
            hasAegisShield_ = false;
            playClickTone();
            guardianSpeech_ = "\"Your Aegis Shield absorbed my counter-spell!\"";
            spawnParticleBurst(640.0f, 430.0f, 26, RGB(95, 215, 255));
            spawnFloatingPopup("AEGIS SHIELD BLOCKED DAMAGE!", 520.0f, 390.0f, RGB(115, 225, 255));
            statusBanner_ = "Aegis Shield shattered to protect your Hearts & Score! Try again.";
            statusColor_ = RGB(115, 225, 255);
            if (ch.type == PuzzleType::MemorySequence) {
                memoryRevealPhase_ = true;
                memoryRevealTimer_ = ds.memoryShowSeconds * 0.75f;
            }
            return;
        }

        playErrorTone();
        scoreManager_.recordIncorrectAnswer();
        player_.addScore(-ds.wrongAnswerPenalty);
        player_.decrementLife();
        guardianSpeech_ = "\"Haha! The chamber rejects your flawed logic!\"";

        spawnParticleBurst(640.0f, 430.0f, 18, RGB(255, 95, 95));
        spawnFloatingPopup("-1 LIFE  -" + std::to_string(ds.wrongAnswerPenalty) + " PTS", 560.0f, 390.0f, RGB(255, 110, 110));

        if (player_.getLives() <= 0) {
            levelFailed_ = true;
            scoreManager_.finalizeLevel(static_cast<int>(levelTimeRemaining_), false);
            saveManager_.savePlayerProfile(player_);
            state_ = ScreenState::LevelCompletion;
            return;
        }

        if (ch.type == PuzzleType::MemorySequence) {
            memoryRevealPhase_ = true;
            memoryRevealTimer_ = ds.memoryShowSeconds * 0.75f;
            statusBanner_ = "Incorrect sequence! -1 Life & -" + std::to_string(ds.wrongAnswerPenalty) + " pts. Watch again!";
        } else {
            statusBanner_ = "Incorrect answer! -1 Life & -" + std::to_string(ds.wrongAnswerPenalty) + " pts. Try again!";
        }
        statusColor_ = RGB(245, 115, 115);
    }
}

void GameMenu::advanceChallengeOrFinishLevel() {
    currentChallengeIdx_++;
    if (currentChallengeIdx_ < activeChallenges_.size()) {
        loadCurrentChallengeIntoUI();
    } else {
        playVictoryFanfare();
        LevelResultSummary summary = scoreManager_.finalizeLevel(static_cast<int>(levelTimeRemaining_), true);
        player_.addScore(summary.timeBonus);
        player_.addCoins(summary.coinsEarned);
        player_.addStars(summary.starsEarned);
        if (summary.keyAwarded) player_.addKeys(1);
        if (!summary.unlockedAchievement.empty()) {
            player_.addAchievement(summary.unlockedAchievement);
        }
        if (activeLevelNumber_ == 5) {
            player_.addAchievement("Savior of the Lost Kingdom");
        }
        player_.markLevelCompleted(activeLevelNumber_, player_.getCurrentScore());
        saveManager_.savePlayerProfile(player_);
        recordRunCompletionAndPersist();
        spawnParticleBurst(640.0f, 320.0f, 48, RGB(255, 215, 85));
        levelFailed_ = false;
        state_ = ScreenState::LevelCompletion;
    }
}

void GameMenu::recordRunCompletionAndPersist() {
    LeaderboardEntry lb;
    lb.playerId = player_.getPlayerId();
    lb.playerName = player_.getName();
    lb.difficulty = difficultyToString(player_.getDifficulty());
    lb.score = std::max(player_.getBestScore(), player_.getCurrentScore());
    lb.accuracy = scoreManager_.getOverallAccuracy();
    lb.levelsCompleted = static_cast<int>(player_.getCompletedLevels().size());
    leaderboard_.addOrUpdateEntry(lb);
    saveManager_.saveLeaderboard(leaderboard_);

    if (activeLevelNumber_ == 5) {
        GameHistoryEntry hist;
        hist.playerId = player_.getPlayerId();
        hist.playerName = player_.getName();
        hist.difficulty = difficultyToString(player_.getDifficulty());
        hist.levelsCompleted = static_cast<int>(player_.getCompletedLevels().size());
        hist.totalScore = player_.getCurrentScore();
        hist.accuracyPercent = scoreManager_.getOverallAccuracy();
        hist.totalTimeSeconds = scoreManager_.getTotalTimeSeconds();

        std::ostringstream decStream;
        const auto& decs = player_.getDecisions();
        if (decs.empty()) {
            decStream << "None";
        } else {
            for (std::size_t i = 0; i < decs.size(); ++i) {
                if (i > 0) decStream << " | ";
                decStream << decs[i].choiceMade;
            }
        }
        hist.decisionsSummary = decStream.str();
        saveManager_.appendGameHistory(hist);
    }
}

void GameMenu::onTimerTick(float dt) {
    elapsedSec_ += dt;
    if (shakeTimer_ > 0.0f) {
        shakeTimer_ -= dt;
        if (shakeTimer_ < 0.0f) shakeTimer_ = 0.0f;
    }

    for (std::size_t i = 0; i < particles_.size();) {
        particles_[i].x += particles_[i].vx * dt;
        particles_[i].y += particles_[i].vy * dt;
        particles_[i].vy += 120.0f * dt;
        particles_[i].life -= dt;
        if (particles_[i].life <= 0.0f) {
            particles_.erase(particles_.begin() + static_cast<std::ptrdiff_t>(i));
        } else {
            ++i;
        }
    }

    for (std::size_t i = 0; i < popups_.size();) {
        popups_[i].y += popups_[i].vy * dt;
        popups_[i].life -= dt;
        if (popups_[i].life <= 0.0f) {
            popups_.erase(popups_.begin() + static_cast<std::ptrdiff_t>(i));
        } else {
            ++i;
        }
    }

    if (state_ == ScreenState::PuzzleGameplay) {
        levelTimeRemaining_ -= dt;
        if (levelTimeRemaining_ <= 0.0f) {
            levelTimeRemaining_ = 0.0f;
            levelFailed_ = true;
            scoreManager_.finalizeLevel(0, false);
            saveManager_.savePlayerProfile(player_);
            playErrorTone();
            state_ = ScreenState::LevelCompletion;
            return;
        }
        if (memoryRevealPhase_) {
            memoryRevealTimer_ -= dt;
            if (memoryRevealTimer_ <= 0.0f) {
                memoryRevealTimer_ = 0.0f;
                memoryRevealPhase_ = false;
            }
        }
    }
}

void GameMenu::renderFrame(HDC hdc) {
    COLORREF topBg = RGB(12, 18, 42);
    COLORREF botBg = RGB(32, 22, 62);
    int theme = 0;
    if (state_ == ScreenState::PuzzleGameplay && activeLevelNumber_ == 3) {
        topBg = RGB(10, 32, 34);
        botBg = RGB(22, 54, 44);
        theme = 3;
    } else if (state_ == ScreenState::PuzzleGameplay && (activeLevelNumber_ == 1 || activeLevelNumber_ == 4)) {
        topBg = RGB(18, 22, 50);
        botBg = RGB(46, 30, 72);
        theme = 1;
    }
    drawVerticalGradient(hdc, 0, 0, kWindowWidth, kWindowHeight, topBg, botBg);

    for (int i = 0; i < 85; ++i) {
        int sx = (i * 197 + 43) % kWindowWidth;
        int sy = (i * 131 + 29) % 520;
        bool twinkle = ((static_cast<int>(elapsedSec_ * 3.0f) + i) % 5 != 0);
        if (twinkle) {
            SetPixel(hdc, sx, sy, RGB(230, 240, 255));
            if (i % 3 == 0) SetPixel(hdc, sx + 1, sy, RGB(170, 195, 245));
        }
    }

    drawKingdomScenery(hdc, kWindowWidth, kWindowHeight, elapsedSec_, theme);

    switch (state_) {
        case ScreenState::MainMenu: renderMainMenu(hdc); break;
        case ScreenState::PlayerRegistration: renderRegistration(hdc); break;
        case ScreenState::PlayerProfile: renderProfile(hdc); break;
        case ScreenState::DifficultySelection: renderDifficulty(hdc); break;
        case ScreenState::LevelSelection: renderLevelSelection(hdc); break;
        case ScreenState::PuzzleGameplay: renderGameplay(hdc); break;
        case ScreenState::DecisionSelection: renderDecisionSelection(hdc); break;
        case ScreenState::HintOverlay:
            renderGameplay(hdc);
            renderHintOverlay(hdc);
            break;
        case ScreenState::LevelCompletion: renderLevelCompletion(hdc); break;
        case ScreenState::FinalResults: renderFinalResults(hdc); break;
        case ScreenState::LeaderboardScreen: renderLeaderboard(hdc); break;
        case ScreenState::InstructionsScreen:
        case ScreenState::SettingsScreen:
            renderInstructions(hdc);
            break;
    }

    renderParticlesAndPopups(hdc);
}

void GameMenu::renderParticlesAndPopups(HDC hdc) {
    for (const auto& p : particles_) {
        HBRUSH b = CreateSolidBrush(p.color);
        HPEN pen = CreatePen(PS_NULL, 0, 0);
        HGDIOBJ oldB = SelectObject(hdc, b);
        HGDIOBJ oldP = SelectObject(hdc, pen);
        Ellipse(hdc, static_cast<int>(p.x - p.size), static_cast<int>(p.y - p.size),
                static_cast<int>(p.x + p.size), static_cast<int>(p.y + p.size));
        SelectObject(hdc, oldB);
        SelectObject(hdc, oldP);
        DeleteObject(b);
        DeleteObject(pen);
    }

    for (const auto& fp : popups_) {
        drawTextLeft(hdc, headingFont_, fp.text, static_cast<int>(fp.x), static_cast<int>(fp.y), fp.color);
    }
}

void GameMenu::renderBackgroundAndHeader(HDC hdc, const std::string& pageTitle) {
    drawPanel(hdc, 0, 0, kWindowWidth, 98, RGB(12, 16, 36), RGB(218, 175, 68), 2);
    backButton_.draw(hdc, smallFont_);
    drawTextLeft(hdc, headingFont_, pageTitle, 230, 18, RGB(245, 205, 92));

    if (player_.isRegistered()) {
        std::ostringstream hud;
        hud << player_.getName() << " [" << player_.getPlayerId() << "]  •  "
            << getHeroRankTitle() << "  •  " << difficultyToString(player_.getDifficulty())
            << "  •  Score: " << player_.getCurrentScore();
        drawTextLeft(hdc, smallFont_, hud.str(), 230, 56, RGB(195, 220, 248));

        int maxLives = getDifficultySettings(player_.getDifficulty()).startingLives;
        int heartStartX = 945;
        for (int i = 0; i < maxLives; ++i) {
            drawHeartIcon(hdc, heartStartX + i * 25, 32, 8, i < player_.getLives());
        }
        // Aegis Shield indicator next to hearts
        drawShieldIcon(hdc, heartStartX + maxLives * 25 + 14, 32, 10, hasAegisShield_);

        drawCoinIcon(hdc, 945, 68, 9);
        drawTextLeft(hdc, smallFont_, std::to_string(player_.getCoins()) + " Coins  |  Lenses: " + std::to_string(oracleLenses_), 962, 60, RGB(255, 225, 110));

        if (answerStreak_ >= 2) {
            drawTextLeft(hdc, smallFont_, "STREAK: " + std::to_string(answerStreak_) + "x", 1140, 60, RGB(120, 245, 175));
        }
    }
}

void GameMenu::renderMainMenu(HDC hdc) {
    drawPanel(hdc, 360, 24, 560, 134, RGB(20, 28, 58), RGB(218, 175, 68), 3);
    drawCrystalEmblem(hdc, 422, 91, 36, elapsedSec_);
    drawTextLeft(hdc, titleFont_, "QUESTCRAFT", 488, 46, RGB(245, 205, 92));
    drawTextLeft(hdc, smallFont_, "THE LOST KINGDOM: QUEST FOR THE MAGICAL CRYSTAL", 488, 94, RGB(205, 222, 248));

    int bob = static_cast<int>(std::sin(elapsedSec_ * 3.0f) * 9.0f);
    drawCrystalEmblem(hdc, 285, 420 + bob, 44, elapsedSec_);
    drawCrystalEmblem(hdc, 995, 420 - bob, 44, elapsedSec_ + 1.5f);

    drawPanel(hdc, 420, 180, 440, 505, RGB(18, 24, 50), RGB(218, 175, 68), 2);
    for (const auto& b : mainMenuButtons_) {
        b.draw(hdc, bodyFont_);
    }

    // Right-Side Hero Chronicle & Relic Armory Card
    if (player_.isRegistered()) {
        drawPanel(hdc, 890, 185, 320, 495, RGB(18, 24, 50), RGB(115, 175, 245), 2);
        drawTextLeft(hdc, headingFont_, "HERO & RELIC ARMORY", 915, 204, RGB(245, 205, 92));
        std::ostringstream card;
        card << "Hero: " << player_.getName() << " (" << player_.getPlayerId() << ")\n"
             << "Rank: " << getHeroRankTitle() << "\n"
             << "Best Score: " << player_.getBestScore() << " pts\n"
             << "Chambers: " << player_.getCompletedLevels().size() << "/5 Cleared\n"
             << "Gold Pouch: " << player_.getCoins() << " Coins\n"
             << "Aegis Shield: " << (hasAegisShield_ ? "ACTIVE (Blocks 1 Error)" : "None") << "\n"
             << "Oracle Lenses: " << oracleLenses_ << " (Free Hints)";
        drawMultilineText(hdc, smallFont_, card.str(), 915, 245, 275, 255, RGB(225, 236, 255));

        royalChestBtn_.enabled = !royalChestClaimed_;
        royalChestBtn_.setup(royalChestClaimed_ ? "Daily Chest Claimed" : "Claim Daily Chest (+35 Coins)", 905, 515, 290, 44);
        royalChestBtn_.draw(hdc, smallFont_);

        buyShieldBtn_.enabled = (!hasAegisShield_ && player_.getCoins() >= 40);
        buyShieldBtn_.draw(hdc, smallFont_);

        buyLensBtn_.enabled = (player_.getCoins() >= 30);
        buyLensBtn_.draw(hdc, smallFont_);
    }

    drawPanel(hdc, 140, 718, 1000, 44, RGB(12, 16, 36), RGB(105, 145, 205), 1);
    drawTextLeft(hdc, smallFont_, statusBanner_, 165, 731, statusColor_);
}

void GameMenu::renderRegistration(HDC hdc) {
    renderBackgroundAndHeader(hdc, "HERO REGISTRATION & PROFILE SETUP");
    drawPanel(hdc, 310, 165, 660, 440);

    drawTextLeft(hdc, headingFont_, "Enter the Lost Kingdom", 495, 195, RGB(245, 205, 92));
    drawMultilineText(hdc, bodyFont_,
        "Register a new adventurer to receive a unique Player ID and CSV save profile,\n"
        "or restore your previous session from local storage.",
        365, 240, 560, 65, RGB(210, 222, 242));

    bool cursor = (static_cast<int>(elapsedSec_ * 2.0f) % 2 == 0);
    nameInput_.draw(hdc, bodyFont_, cursor);
    registerSubmitBtn_.draw(hdc, bodyFont_);
    loadSavedBtn_.draw(hdc, bodyFont_);

    if (!registrationFeedback_.empty()) {
        drawTextLeft(hdc, bodyFont_, registrationFeedback_, 390, 540, RGB(125, 235, 175));
    }
}

void GameMenu::renderProfile(HDC hdc) {
    renderBackgroundAndHeader(hdc, "ROYAL ADVENTURER PROFILE & ACHIEVEMENTS");
    drawPanel(hdc, 180, 140, 920, 570);

    if (!player_.isRegistered()) {
        drawTextLeft(hdc, bodyFont_, "No active hero registered yet. Click 'Register / Switch Hero' on the Main Menu!", 265, 380, RGB(245, 195, 95));
        return;
    }

    std::ostringstream info;
    info << "Hero Name:               " << player_.getName() << "\n"
         << "Royal Title:             " << getHeroRankTitle() << "\n"
         << "Unique Player ID:        " << player_.getPlayerId() << "\n"
         << "Selected Difficulty:     " << difficultyToString(player_.getDifficulty()) << "\n"
         << "Highest Unlocked Level:  Level " << player_.getHighestUnlockedLevel() << " of 5\n"
         << "Completed Levels:        " << player_.serializeCompletedLevels() << "\n"
         << "All-Time Best Score:     " << player_.getBestScore() << " pts\n"
         << "Best Answer Streak:      " << maxStreak_ << " in a row\n"
         << "Treasury & Relics:       " << player_.getCoins() << " Coins  |  "
         << player_.getStars() << " Stars  |  " << player_.getKeys() << " Royal Keys";

    drawMultilineText(hdc, bodyFont_, info.str(), 230, 175, 520, 460, RGB(235, 242, 255));

    drawPanel(hdc, 740, 175, 320, 480, RGB(14, 20, 42), RGB(218, 175, 68), 1);
    drawTextLeft(hdc, headingFont_, "EARNED BADGES", 775, 195, RGB(245, 205, 92));
    const auto& badges = player_.getAchievements();
    if (badges.empty()) {
        drawMultilineText(hdc, smallFont_, "Complete chambers with high accuracy or streaks to unlock Royal Badges!", 765, 245, 270, 120, RGB(185, 205, 235));
    } else {
        for (std::size_t i = 0; i < badges.size() && i < 8; ++i) {
            drawStarIcon(hdc, 775, 252 + static_cast<int>(i) * 44, 10, true);
            drawTextLeft(hdc, smallFont_, badges[i], 795, 244 + static_cast<int>(i) * 44, RGB(130, 245, 175));
        }
    }
}

void GameMenu::renderDifficulty(HDC hdc) {
    renderBackgroundAndHeader(hdc, "SELECT QUEST DIFFICULTY");
    drawPanel(hdc, 260, 155, 760, 430);
    drawTextLeft(hdc, smallFont_, "Difficulty scales starting lives, timers, puzzle complexity, and score multipliers.", 320, 200, RGB(205, 220, 245));

    for (std::size_t i = 0; i < difficultyButtons_.size(); ++i) {
        difficultyButtons_[i].outlineColor = (static_cast<int>(player_.getDifficulty()) == static_cast<int>(i))
            ? RGB(115, 245, 165) : RGB(218, 175, 68);
        difficultyButtons_[i].draw(hdc, bodyFont_);
    }
}

void GameMenu::renderLevelSelection(HDC hdc) {
    renderBackgroundAndHeader(hdc, "KINGDOM OVERWORLD QUEST MAP");

    int coords[5][2] = {
        {110, 470},
        {345, 315},
        {575, 485},
        {805, 315},
        {1015, 455}
    };

    // Draw glowing golden quest trail connecting the 5 kingdom landmarks
    for (int i = 0; i < 4; ++i) {
        bool pathOpen = player_.isLevelUnlocked(i + 2);
        HPEN trailPen = CreatePen(pathOpen ? PS_SOLID : PS_DOT, pathOpen ? 4 : 2,
                                  pathOpen ? RGB(245, 205, 92) : RGB(105, 120, 155));
        HGDIOBJ oldP = SelectObject(hdc, trailPen);
        MoveToEx(hdc, coords[i][0] + 85, coords[i][1], nullptr);
        LineTo(hdc, coords[i + 1][0] + 85, coords[i + 1][1]);
        SelectObject(hdc, oldP);
        DeleteObject(trailPen);
    }

    int heroNode = std::max(0, std::min(4, player_.getHighestUnlockedLevel() - 1));

    for (int i = 0; i < 5; ++i) {
        int lvlNum = i + 1;
        const auto& lvl = levelManager_.getLevel(lvlNum);
        bool unlocked = player_.isLevelUnlocked(lvlNum);
        bool completed = player_.isLevelCompleted(lvlNum);

        int cx = coords[i][0] + 85;
        int cy = coords[i][1];

        drawPanel(hdc, cx - 105, cy - 115, 210, 215,
                  unlocked ? RGB(20, 28, 56) : RGB(16, 18, 30),
                  completed ? RGB(115, 240, 165) : (unlocked ? RGB(245, 205, 92) : RGB(90, 95, 112)), 2);

        // Node emblem
        if (lvlNum == 5) {
            drawCrystalEmblem(hdc, cx, cy - 68, 22, elapsedSec_);
        } else {
            drawRuneSymbol(hdc, i, cx, cy - 68, 20);
        }

        RECT titleR = {cx - 98, cy - 42, cx + 98, cy - 18};
        drawTextCentered(hdc, smallFont_, lvl.name, titleR, RGB(245, 225, 145));

        std::string badge = completed ? "[CLEARED]" : (unlocked ? "[UNLOCKED]" : "[LOCKED]");
        COLORREF badgeCol = completed ? RGB(115, 240, 165) : (unlocked ? RGB(115, 215, 255) : RGB(145, 150, 165));
        RECT stR = {cx - 98, cy - 16, cx + 98, cy + 8};
        drawTextCentered(hdc, smallFont_, badge, stR, badgeCol);

        levelButtons_[i].setup(unlocked ? "Enter Chamber" : "Locked", cx - 85, cy + 44, 170, 40);
        levelButtons_[i].enabled = unlocked;
        levelButtons_[i].draw(hdc, smallFont_);

        if (i == heroNode) {
            drawHeroAvatar(hdc, cx, cy - 135, elapsedSec_);
        }
    }

    viewFinalReportBtn_.draw(hdc, bodyFont_);
}

void GameMenu::renderGameplay(HDC hdc) {
    const auto& lvl = levelManager_.getLevel(activeLevelNumber_);
    renderBackgroundAndHeader(hdc, lvl.name);

    if (currentChallengeIdx_ >= activeChallenges_.size()) return;
    const auto& ch = activeChallenges_[currentChallengeIdx_];

    drawPanel(hdc, 140, 108, 1000, 74, RGB(18, 24, 48), RGB(115, 175, 245), 2);
    std::ostringstream prog;
    prog << "Stage " << (currentChallengeIdx_ + 1) << "/" << activeChallenges_.size()
         << "   |   Timer: " << static_cast<int>(std::ceil(levelTimeRemaining_)) << "s"
         << "   |   Hints: " << (hintSystem_.getMaxHintsPerLevel() - hintSystem_.getHintsUsedThisLevel());
    drawTextLeft(hdc, bodyFont_, prog.str(), 165, 120,
                 levelTimeRemaining_ < 12.0f ? RGB(255, 105, 105) : RGB(245, 225, 130));

    float timeRatio = (levelMaxTime_ > 0.0f) ? (levelTimeRemaining_ / levelMaxTime_) : 0.0f;
    COLORREF barCol = timeRatio > 0.45f ? RGB(95, 225, 155) : (timeRatio > 0.2f ? RGB(245, 195, 65) : RGB(245, 85, 85));
    drawProgressBar(hdc, 165, 152, 480, 14, timeRatio, barCol);

    freezeTimeBtn_.enabled = (!freezeTimeUsedInLevel_ && player_.getCoins() >= 25);
    freezeTimeBtn_.draw(hdc, smallFont_);

    std::string hintBtnText = (oracleLenses_ > 0)
        ? ("Use Oracle Lens (FREE)")
        : ("Hint (-" + std::to_string(hintSystem_.getCurrentPenaltyCost()) + " pts)");
    requestHintBtn_.setup(hintBtnText, 935, 118, 190, 42);
    requestHintBtn_.enabled = (oracleLenses_ > 0 || hintSystem_.canRequestHint());
    requestHintBtn_.draw(hdc, smallFont_);

    // Guardian Boss Encounter Strip
    drawPanel(hdc, 140, 192, 1000, 118, RGB(16, 22, 44), RGB(175, 135, 235), 2);
    int hpLeft = static_cast<int>(activeChallenges_.size() - currentChallengeIdx_);
    drawGuardianBoss(hdc, 210, 242, activeLevelNumber_, hpLeft, static_cast<int>(activeChallenges_.size()), elapsedSec_);
    drawTextLeft(hdc, headingFont_, getGuardianName(activeLevelNumber_), 285, 206, RGB(245, 205, 92));
    drawTextLeft(hdc, bodyFont_, guardianSpeech_, 285, 242, RGB(215, 230, 255));
    drawTextLeft(hdc, smallFont_, "Chamber Seals Remaining: " + std::to_string(hpLeft), 285, 276, RGB(255, 145, 155));

    // Puzzle Arena Card
    drawPanel(hdc, 140, 320, 1000, 385);
    drawTextLeft(hdc, headingFont_, ch.title, 180, 336, RGB(245, 205, 92));
    drawMultilineText(hdc, bodyFont_, ch.prompt, 180, 372, 920, 95, RGB(238, 244, 255));

    if (ch.type == PuzzleType::MemorySequence) {
        if (memoryRevealPhase_) {
            drawTextLeft(hdc, bodyFont_,
                "MEMORIZE THESE RUNES (" + std::to_string(static_cast<int>(std::ceil(memoryRevealTimer_))) + "s):",
                180, 435, RGB(115, 235, 255));

            int count = static_cast<int>(ch.memorySequence.size());
            int cardW = 122;
            int startX = 640 - (count * (cardW + 14)) / 2;
            for (int i = 0; i < count; ++i) {
                int rx = startX + i * (cardW + 14);
                int rId = ch.memorySequence[i];
                drawPanel(hdc, rx, 475, cardW, 102, RGB(34, 58, 108), RGB(125, 235, 255), 3);
                drawRuneSymbol(hdc, rId, rx + cardW / 2, 512, 18);
                RECT rBox = {rx, 538, rx + cardW, 570};
                drawTextCentered(hdc, smallFont_, PuzzleEngine::getRuneName(rId), rBox, RGB(255, 255, 255));
            }
        } else {
            std::ostringstream entered;
            entered << "Your Rune Sequence (" << playerMemoryInput_.size() << "/" << ch.memorySequence.size() << "):  ";
            for (std::size_t i = 0; i < playerMemoryInput_.size(); ++i) {
                if (i > 0) entered << " -> ";
                entered << PuzzleEngine::getRuneName(playerMemoryInput_[i]);
            }
            drawTextLeft(hdc, bodyFont_, entered.str(), 180, 425, RGB(145, 240, 185));
            for (int i = 0; i < 6; ++i) {
                runeButtons_[i].setup(std::to_string(i + 1) + ": " + PuzzleEngine::getRuneName(i), 175 + i * 158, 465, 142, 74, i);
                runeButtons_[i].draw(hdc, smallFont_);
            }
            clearMemoryBtn_.setup("Clear Rune Input", 530, 552, 220, 42);
            clearMemoryBtn_.draw(hdc, smallFont_);
        }
    } else if (ch.isMultipleChoice) {
        for (std::size_t i = 0; i < mcOptionButtons_.size() && i < ch.choices.size(); ++i) {
            mcOptionButtons_[i].setup(ch.choices[i], 270, 445 + static_cast<int>(i) * 52, 740, 44);
            mcOptionButtons_[i].draw(hdc, bodyFont_);
        }
    } else {
        answerInput_.setup("Type integer answer...", 340, 510, 400, 52);
        submitAnswerBtn_.setup("Submit Answer", 760, 510, 200, 52);
        bool cursor = (static_cast<int>(elapsedSec_ * 2.0f) % 2 == 0);
        answerInput_.draw(hdc, bodyFont_, cursor);
        submitAnswerBtn_.draw(hdc, bodyFont_);
    }

    drawTextLeft(hdc, bodyFont_, statusBanner_, 180, 662, statusColor_);
}

void GameMenu::renderDecisionSelection(HDC hdc) {
    renderBackgroundAndHeader(hdc, "LEVEL 4: DECISION IMPACT CHAMBER");
    drawPanel(hdc, 160, 130, 960, 580);
    drawTextLeft(hdc, headingFont_, activeDecisionScenario_.title, 210, 155, RGB(245, 205, 92));
    drawMultilineText(hdc, bodyFont_, activeDecisionScenario_.narrative, 210, 198, 860, 95, RGB(232, 240, 255));

    for (std::size_t i = 0; i < activeDecisionScenario_.options.size() && i < decisionOptionButtons_.size(); ++i) {
        decisionOptionButtons_[i].draw(hdc, bodyFont_);
        drawTextLeft(hdc, smallFont_, activeDecisionScenario_.options[i].description, 225, 384 + static_cast<int>(i) * 105, RGB(185, 210, 240));
    }
}

void GameMenu::renderHintOverlay(HDC hdc) {
    drawPanel(hdc, 260, 230, 760, 340, RGB(14, 20, 44), RGB(115, 225, 255), 3);
    drawTextLeft(hdc, headingFont_, "ORACLE'S HINT", 550, 258, RGB(115, 225, 255));
    drawMultilineText(hdc, bodyFont_, hintSystem_.getActiveHintText(), 310, 320, 660, 150, RGB(255, 255, 255));
    dismissHintBtn_.draw(hdc, bodyFont_);
}

void GameMenu::renderLevelCompletion(HDC hdc) {
    renderBackgroundAndHeader(hdc, levelFailed_ ? "CHAMBER FAILED - OUT OF LIVES OR TIME" : "CHAMBER CLEARED!");
    drawPanel(hdc, 260, 130, 760, 585);

    const auto& sum = scoreManager_.getLastLevelSummary();
    drawTextLeft(hdc, headingFont_,
                 levelFailed_ ? "THE SHADOWS PREVAILED" : (sum.levelTitle + " - VICTORY!"),
                 310, 155, levelFailed_ ? RGB(255, 110, 110) : RGB(125, 245, 170));

    for (int s = 0; s < 3; ++s) {
        drawStarIcon(hdc, 830 + s * 42, 170, 16, !levelFailed_ && (s < sum.starsEarned));
    }

    std::ostringstream details;
    details << "Correct Answers:       " << sum.correctAnswers << "\n"
            << "Incorrect Answers:     " << sum.incorrectAnswers << " (-" << sum.wrongPenalty << " pts)\n"
            << "Hints Used:            " << sum.hintsUsed << " (-" << sum.hintPenalty << " pts)\n"
            << "Completion Time:       " << sum.timeTakenSeconds << " seconds\n"
            << "Speed Time Bonus:      +" << sum.timeBonus << " pts\n"
            << "Decision Bonus:        +" << sum.decisionBonus << " pts\n"
            << "Chamber Score Earned:  " << sum.finalLevelScore << " pts\n"
            << "Rewards Collected:     +" << sum.coinsEarned << " Coins  |  +" << sum.starsEarned << " Stars"
            << (sum.keyAwarded ? "  |  +1 Royal Key" : "");

    if (!sum.unlockedAchievement.empty()) {
        details << "\nAchievement Unlocked:  " << sum.unlockedAchievement;
    }
    if (!lastDecisionConsequence_.empty()) {
        details << "\n\nDecision Impact: " << lastDecisionConsequence_;
    }

    drawMultilineText(hdc, bodyFont_, details.str(), 310, 208, 660, 345, RGB(235, 242, 255));

    if (levelFailed_) {
        retryLevelBtn_.draw(hdc, bodyFont_);
    } else {
        nextLevelBtn_.setup(activeLevelNumber_ < 5 ? "Proceed to Level " + std::to_string(activeLevelNumber_ + 1) + " >" : "Claim Magical Crystal & Final Report >",
                            440, 565, 400, 52);
        nextLevelBtn_.draw(hdc, bodyFont_);
    }
    toLevelSelectBtn_.draw(hdc, bodyFont_);
}

void GameMenu::renderFinalResults(HDC hdc) {
    renderBackgroundAndHeader(hdc, "FINAL KINGDOM PERFORMANCE & DECISION REPORT");
    drawPanel(hdc, 170, 120, 940, 510);

    drawTextLeft(hdc, headingFont_, "THE QUEST FOR THE MAGICAL CRYSTAL - PERFORMANCE ANALYSIS", 215, 142, RGB(245, 205, 92));

    std::ostringstream report;
    report << "Hero: " << player_.getName() << " (" << player_.getPlayerId() << ")    |    Rank: " << getHeroRankTitle() << "\n"
           << "Total Quest Score:        " << std::max(player_.getCurrentScore(), player_.getBestScore()) << " pts   |   Best Streak: " << maxStreak_ << "x\n"
           << "Levels Completed:         " << player_.getCompletedLevels().size() << " / 5\n"
           << "Correct vs Incorrect:     " << scoreManager_.getTotalCorrect() << " Correct  /  "
           << scoreManager_.getTotalIncorrect() << " Incorrect\n"
           << "Overall Accuracy:         " << std::fixed << std::setprecision(1) << scoreManager_.getOverallAccuracy() << "%\n"
           << "Total Challenge Time:     " << scoreManager_.getTotalTimeSeconds() << " seconds\n"
           << "Total Hints Used:         " << scoreManager_.getTotalHintsUsed() << "\n"
           << "Treasury & Relics:        " << player_.getCoins() << " Coins  |  " << player_.getStars()
           << " Stars  |  " << player_.getKeys() << " Keys\n\n"
           << "DECISION IMPACT LOG:\n";

    const auto& decs = player_.getDecisions();
    if (decs.empty()) {
        report << "  - No decision chambers recorded in this session yet (Complete Level 4!).\n";
    } else {
        for (const auto& d : decs) {
            report << "  * [Level " << d.levelNumber << "] " << d.choiceMade << "\n    Consequence: " << d.consequenceSummary << "\n";
        }
    }

    drawMultilineText(hdc, smallFont_, report.str(), 215, 185, 850, 430, RGB(235, 242, 255));
    playAgainBtn_.draw(hdc, bodyFont_);
}

void GameMenu::renderLeaderboard(HDC hdc) {
    renderBackgroundAndHeader(hdc, "HALL OF HEROES - GLOBAL LEADERBOARD");
    drawPanel(hdc, 170, 135, 940, 580);

    drawTextLeft(hdc, bodyFont_, "RANK      PLAYER ID             HERO NAME            DIFFICULTY      SCORE      ACCURACY     LEVELS", 205, 162, RGB(245, 205, 92));

    const auto& entries = leaderboard_.getEntries();
    if (entries.empty()) {
        drawTextLeft(hdc, bodyFont_, "No scores recorded yet. Complete a level to claim #1 on the Leaderboard!", 310, 340, RGB(200, 215, 240));
        return;
    }

    std::size_t maxRows = std::min<std::size_t>(10, entries.size());
    for (std::size_t i = 0; i < maxRows; ++i) {
        const auto& e = entries[i];
        std::ostringstream row;
        row << "#" << std::left << std::setw(8) << e.rank
            << std::setw(22) << e.playerId
            << std::setw(21) << e.playerName
            << std::setw(16) << e.difficulty
            << std::setw(11) << e.score
            << std::fixed << std::setprecision(1) << e.accuracy << "%         "
            << e.levelsCompleted << "/5";
        drawTextLeft(hdc, smallFont_, row.str(), 205, 215 + static_cast<int>(i) * 44,
                     i == 0 ? RGB(125, 245, 175) : RGB(232, 240, 255));
    }
}

void GameMenu::renderInstructions(HDC hdc) {
    renderBackgroundAndHeader(hdc, "INSTRUCTIONS, STORYLINE & SETTINGS");
    drawPanel(hdc, 170, 120, 940, 540);

    std::string guide =
        "STORY: THE LOST KINGDOM - THE QUEST FOR THE MAGICAL CRYSTAL\n"
        "A shadow sorcerer has stolen the Kingdom's Magical Crystal. Conquer 5 chambers to restore peace:\n"
        "  * Level 1 (Memory Temple): Watch the glowing rune symbols, then click the runes in order.\n"
        "  * Level 2 (Logic Chamber): Type the integer answer and press Enter or Submit.\n"
        "  * Level 3 (Mystery Forest): Analyze the clues and select the right option.\n"
        "  * Level 4 (Decision Gate): Your choice (Golden Door vs Secret Passage) alters rewards & Level 5!\n"
        "  * Level 5 (Crystal Challenge): Combined memory, logic, and mystery finale.\n\n"
        "RELIC ARMORY & INNOVATIVE FEATURES:\n"
        "  * Aegis Shield: Automatically absorbs 1 wrong answer without losing a Heart or Streak!\n"
        "  * Oracle Lens: Grants a completely FREE hint with zero point penalty.\n"
        "  * Chrono Freeze: Spend 25 Gold Coins mid-puzzle for +15 seconds on the chamber clock.";

    drawMultilineText(hdc, bodyFont_, guide, 205, 145, 870, 350, RGB(232, 240, 255));
    toggleSoundBtn_.draw(hdc, bodyFont_);
    resetSaveBtn_.draw(hdc, bodyFont_);
}

} // namespace qc
