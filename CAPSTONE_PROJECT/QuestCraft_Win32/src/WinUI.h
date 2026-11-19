#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <string>
#include <vector>

namespace qc {

struct Particle {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float life = 1.0f;
    float maxLife = 1.0f;
    int size = 4;
    COLORREF color = RGB(255, 220, 110);
};

struct FloatingPopup {
    std::string text;
    float x = 0.0f;
    float y = 0.0f;
    float vy = -38.0f;
    float life = 1.6f;
    COLORREF color = RGB(120, 250, 175);
};

struct UIButton {
    RECT rect{};
    std::string label;
    bool enabled = true;
    bool hovered = false;
    COLORREF normalFill = RGB(34, 46, 86);
    COLORREF hoverFill = RGB(62, 82, 148);
    COLORREF disabledFill = RGB(26, 30, 44);
    COLORREF outlineColor = RGB(218, 175, 68);
    int badgeIconId = -1;

    void setup(const std::string& text, int x, int y, int w, int h, int iconId = -1);
    void updateHover(POINT pt);
    bool isClicked(POINT pt) const;
    void draw(HDC hdc, HFONT font) const;
};

struct UITextInput {
    RECT rect{};
    std::string placeholder;
    std::string value;
    bool focused = true;
    std::size_t maxLength = 24;
    bool numericOnly = false;

    void setup(const std::string& ph, int x, int y, int w, int h);
    void handleChar(wchar_t ch);
    void draw(HDC hdc, HFONT font, bool cursorVisible) const;
};

void drawVerticalGradient(HDC hdc, int x, int y, int w, int h, COLORREF topCol, COLORREF botCol);
void drawPanel(HDC hdc, int x, int y, int w, int h,
               COLORREF fill = RGB(18, 24, 48),
               COLORREF outline = RGB(218, 175, 68),
               int borderWidth = 2);
void drawProgressBar(HDC hdc, int x, int y, int w, int h, float ratio, COLORREF fillCol, COLORREF bgCol = RGB(16, 20, 38));
void drawHeartIcon(HDC hdc, int cx, int cy, int size, bool filled);
void drawShieldIcon(HDC hdc, int cx, int cy, int size, bool active);
void drawCoinIcon(HDC hdc, int cx, int cy, int r);
void drawStarIcon(HDC hdc, int cx, int cy, int outerR, bool filled);
void drawRuneSymbol(HDC hdc, int runeId, int cx, int cy, int radius);
void drawCrystalEmblem(HDC hdc, int cx, int cy, int radius, float pulsePhase = 0.0f);
void drawKingdomScenery(HDC hdc, int width, int height, float elapsedSec, int themeMode);
void drawGuardianBoss(HDC hdc, int cx, int cy, int levelNum, int hpRemaining, int maxHp, float elapsedSec);
void drawHeroAvatar(HDC hdc, int cx, int cy, float elapsedSec);

void drawTextLeft(HDC hdc, HFONT font, const std::string& text, int x, int y, COLORREF color);
void drawTextCentered(HDC hdc, HFONT font, const std::string& text, RECT rect, COLORREF color);
void drawMultilineText(HDC hdc, HFONT font, const std::string& text, int x, int y, int w, int h, COLORREF color);

} // namespace qc
