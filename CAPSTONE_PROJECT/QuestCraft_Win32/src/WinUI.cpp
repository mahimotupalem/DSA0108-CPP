#include "WinUI.h"
#include <algorithm>
#include <cmath>

namespace qc {

void UIButton::setup(const std::string& text, int x, int y, int w, int h, int iconId) {
    label = text;
    rect = {x, y, x + w, y + h};
    badgeIconId = iconId;
}

void UIButton::updateHover(POINT pt) {
    if (!enabled) {
        hovered = false;
        return;
    }
    hovered = (PtInRect(&rect, pt) != 0);
}

bool UIButton::isClicked(POINT pt) const {
    return enabled && (PtInRect(&rect, pt) != 0);
}

void UIButton::draw(HDC hdc, HFONT font) const {
    COLORREF fill = !enabled ? disabledFill : (hovered ? hoverFill : normalFill);
    COLORREF border = !enabled ? RGB(95, 100, 118) : (hovered ? RGB(255, 226, 120) : outlineColor);
    COLORREF txtCol = !enabled ? RGB(140, 145, 162) : RGB(248, 250, 255);

    int offsetY = hovered && enabled ? -2 : 0;
    RECT r = {rect.left, rect.top + offsetY, rect.right, rect.bottom + offsetY};

    HBRUSH shadowBrush = CreateSolidBrush(RGB(8, 10, 22));
    HPEN nullPen = CreatePen(PS_NULL, 0, 0);
    HGDIOBJ oldB = SelectObject(hdc, shadowBrush);
    HGDIOBJ oldP = SelectObject(hdc, nullPen);
    RoundRect(hdc, rect.left + 3, rect.top + 4, rect.right + 3, rect.bottom + 4, 14, 14);

    HBRUSH brush = CreateSolidBrush(fill);
    HPEN pen = CreatePen(PS_SOLID, hovered && enabled ? 3 : 2, border);
    SelectObject(hdc, brush);
    SelectObject(hdc, pen);
    RoundRect(hdc, r.left, r.top, r.right, r.bottom, 14, 14);

    if (enabled) {
        HPEN hiPen = CreatePen(PS_SOLID, 1, hovered ? RGB(140, 175, 255) : RGB(75, 95, 155));
        SelectObject(hdc, hiPen);
        MoveToEx(hdc, r.left + 10, r.top + 2, nullptr);
        LineTo(hdc, r.right - 10, r.top + 2);
        SelectObject(hdc, pen);
        DeleteObject(hiPen);
    }

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(shadowBrush);
    DeleteObject(nullPen);
    DeleteObject(brush);
    DeleteObject(pen);

    if (badgeIconId >= 0) {
        int iconCx = (r.left + r.right) / 2;
        int iconCy = r.top + 24;
        drawRuneSymbol(hdc, badgeIconId, iconCx, iconCy, 14);
        RECT lblRect = {r.left + 4, r.top + 38, r.right - 4, r.bottom - 4};
        drawTextCentered(hdc, font, label, lblRect, txtCol);
    } else {
        drawTextCentered(hdc, font, label, r, txtCol);
    }
}

void UITextInput::setup(const std::string& ph, int x, int y, int w, int h) {
    placeholder = ph;
    rect = {x, y, x + w, y + h};
}

void UITextInput::handleChar(wchar_t ch) {
    if (!focused) return;
    if (ch == 8) {
        if (!value.empty()) value.pop_back();
        return;
    }
    if (ch < 32 || ch > 126 || ch == L',') return;
    char c = static_cast<char>(ch);
    if (numericOnly) {
        if (!((c >= '0' && c <= '9') || (c == '-' && value.empty()))) return;
    }
    if (value.size() < maxLength) {
        value.push_back(c);
    }
}

void UITextInput::draw(HDC hdc, HFONT font, bool cursorVisible) const {
    COLORREF border = focused ? RGB(245, 205, 92) : RGB(110, 145, 195);
    HBRUSH brush = CreateSolidBrush(RGB(11, 15, 32));
    HPEN pen = CreatePen(PS_SOLID, 2, border);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 12, 12);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);

    RECT textRect = {rect.left + 16, rect.top, rect.right - 16, rect.bottom};
    SetBkMode(hdc, TRANSPARENT);
    HGDIOBJ oldFont = SelectObject(hdc, font);

    if (value.empty() && !focused) {
        SetTextColor(hdc, RGB(135, 148, 175));
        DrawTextA(hdc, placeholder.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    } else {
        std::string shown = value + (focused && cursorVisible ? "|" : "");
        SetTextColor(hdc, RGB(245, 248, 255));
        DrawTextA(hdc, shown.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
    SelectObject(hdc, oldFont);
}

void drawVerticalGradient(HDC hdc, int x, int y, int w, int h, COLORREF topCol, COLORREF botCol) {
    TRIVERTEX vert[2]{};
    vert[0].x = x;
    vert[0].y = y;
    vert[0].Red = static_cast<COLOR16>(GetRValue(topCol) << 8);
    vert[0].Green = static_cast<COLOR16>(GetGValue(topCol) << 8);
    vert[0].Blue = static_cast<COLOR16>(GetBValue(topCol) << 8);
    vert[0].Alpha = 0xFF00;

    vert[1].x = x + w;
    vert[1].y = y + h;
    vert[1].Red = static_cast<COLOR16>(GetRValue(botCol) << 8);
    vert[1].Green = static_cast<COLOR16>(GetGValue(botCol) << 8);
    vert[1].Blue = static_cast<COLOR16>(GetBValue(botCol) << 8);
    vert[1].Alpha = 0xFF00;

    GRADIENT_RECT gRect = {0, 1};
    GradientFill(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}

void drawPanel(HDC hdc, int x, int y, int w, int h, COLORREF fill, COLORREF outline, int borderWidth) {
    HBRUSH shadow = CreateSolidBrush(RGB(8, 11, 24));
    HPEN nullPen = CreatePen(PS_NULL, 0, 0);
    HGDIOBJ oldB = SelectObject(hdc, shadow);
    HGDIOBJ oldP = SelectObject(hdc, nullPen);
    RoundRect(hdc, x + 5, y + 6, x + w + 5, y + h + 6, 16, 16);

    HBRUSH brush = CreateSolidBrush(fill);
    HPEN pen = CreatePen(PS_SOLID, borderWidth, outline);
    SelectObject(hdc, brush);
    SelectObject(hdc, pen);
    RoundRect(hdc, x, y, x + w, y + h, 16, 16);

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(shadow);
    DeleteObject(nullPen);
    DeleteObject(brush);
    DeleteObject(pen);
}

void drawProgressBar(HDC hdc, int x, int y, int w, int h, float ratio, COLORREF fillCol, COLORREF bgCol) {
    ratio = std::max(0.0f, std::min(1.0f, ratio));
    HBRUSH bgBrush = CreateSolidBrush(bgCol);
    HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(115, 145, 195));
    HGDIOBJ oldB = SelectObject(hdc, bgBrush);
    HGDIOBJ oldP = SelectObject(hdc, borderPen);
    RoundRect(hdc, x, y, x + w, y + h, h, h);

    int fillW = static_cast<int>((w - 4) * ratio);
    if (fillW > 2) {
        HBRUSH fBrush = CreateSolidBrush(fillCol);
        HPEN nullPen = CreatePen(PS_NULL, 0, 0);
        SelectObject(hdc, fBrush);
        SelectObject(hdc, nullPen);
        RoundRect(hdc, x + 2, y + 2, x + 2 + fillW, y + h - 2, h - 2, h - 2);
        DeleteObject(fBrush);
        DeleteObject(nullPen);
    }

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(bgBrush);
    DeleteObject(borderPen);
}

void drawHeartIcon(HDC hdc, int cx, int cy, int size, bool filled) {
    COLORREF col = filled ? RGB(240, 68, 92) : RGB(65, 72, 95);
    HBRUSH brush = CreateSolidBrush(col);
    HPEN pen = CreatePen(PS_SOLID, 1, filled ? RGB(255, 175, 185) : RGB(95, 102, 125));
    HGDIOBJ oldB = SelectObject(hdc, brush);
    HGDIOBJ oldP = SelectObject(hdc, pen);

    int r = size / 2;
    Ellipse(hdc, cx - size, cy - r, cx, cy + r);
    Ellipse(hdc, cx, cy - r, cx + size, cy + r);
    POINT tri[3] = {
        {cx - size + 1, cy + 1},
        {cx + size - 1, cy + 1},
        {cx, cy + size + 2}
    };
    Polygon(hdc, tri, 3);

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(brush);
    DeleteObject(pen);
}

void drawShieldIcon(HDC hdc, int cx, int cy, int size, bool active) {
    COLORREF fill = active ? RGB(75, 195, 255) : RGB(52, 60, 82);
    COLORREF border = active ? RGB(220, 248, 255) : RGB(95, 105, 130);
    HBRUSH brush = CreateSolidBrush(fill);
    HPEN pen = CreatePen(PS_SOLID, 2, border);
    HGDIOBJ oldB = SelectObject(hdc, brush);
    HGDIOBJ oldP = SelectObject(hdc, pen);

    POINT pts[5] = {
        {cx - size, cy - size},
        {cx + size, cy - size},
        {cx + size, cy + size / 5},
        {cx, cy + size + 3},
        {cx - size, cy + size / 5}
    };
    Polygon(hdc, pts, 5);

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(brush);
    DeleteObject(pen);
}

void drawCoinIcon(HDC hdc, int cx, int cy, int r) {
    HBRUSH outer = CreateSolidBrush(RGB(245, 195, 55));
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 238, 145));
    HGDIOBJ oldB = SelectObject(hdc, outer);
    HGDIOBJ oldP = SelectObject(hdc, pen);
    Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);
    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(outer);
    DeleteObject(pen);
}

void drawStarIcon(HDC hdc, int cx, int cy, int outerR, bool filled) {
    POINT pts[10];
    double innerR = outerR * 0.42;
    for (int i = 0; i < 10; ++i) {
        double angle = -3.14159265 / 2.0 + i * (3.14159265 / 5.0);
        double rad = (i % 2 == 0) ? outerR : innerR;
        pts[i].x = cx + static_cast<int>(std::cos(angle) * rad);
        pts[i].y = cy + static_cast<int>(std::sin(angle) * rad);
    }
    HBRUSH brush = CreateSolidBrush(filled ? RGB(255, 212, 72) : RGB(55, 62, 85));
    HPEN pen = CreatePen(PS_SOLID, 1, filled ? RGB(255, 245, 180) : RGB(95, 105, 130));
    HGDIOBJ oldB = SelectObject(hdc, brush);
    HGDIOBJ oldP = SelectObject(hdc, pen);
    Polygon(hdc, pts, 10);
    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(brush);
    DeleteObject(pen);
}

void drawRuneSymbol(HDC hdc, int runeId, int cx, int cy, int radius) {
    int id = runeId % 6;
    COLORREF cols[6] = {
        RGB(255, 195, 65),  // SUN
        RGB(145, 185, 255), // MOON
        RGB(255, 235, 115), // STAR
        RGB(255, 105, 75),  // FLAME
        RGB(75, 210, 255),  // WAVE
        RGB(95, 235, 145)   // LEAF
    };
    HBRUSH brush = CreateSolidBrush(cols[id]);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(245, 250, 255));
    HGDIOBJ oldB = SelectObject(hdc, brush);
    HGDIOBJ oldP = SelectObject(hdc, pen);

    if (id == 0) {
        Ellipse(hdc, cx - radius + 3, cy - radius + 3, cx + radius - 3, cy + radius - 3);
    } else if (id == 1) {
        POINT p[4] = {{cx, cy - radius}, {cx + radius - 2, cy}, {cx, cy + radius}, {cx - radius + 5, cy}};
        Polygon(hdc, p, 4);
    } else if (id == 2) {
        drawStarIcon(hdc, cx, cy, radius, true);
    } else if (id == 3) {
        POINT p[3] = {{cx, cy - radius}, {cx + radius - 2, cy + radius - 2}, {cx - radius + 2, cy + radius - 2}};
        Polygon(hdc, p, 3);
    } else if (id == 4) {
        POINT p[6] = {
            {cx - radius + 2, cy - radius / 2}, {cx, cy - radius},
            {cx + radius - 2, cy - radius / 2}, {cx + radius - 2, cy + radius / 2},
            {cx, cy + radius}, {cx - radius + 2, cy + radius / 2}
        };
        Polygon(hdc, p, 6);
    } else {
        POINT p[4] = {{cx, cy - radius}, {cx + radius - 3, cy}, {cx, cy + radius}, {cx - radius + 3, cy}};
        Polygon(hdc, p, 4);
    }

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(brush);
    DeleteObject(pen);
}

void drawCrystalEmblem(HDC hdc, int cx, int cy, int radius, float pulsePhase) {
    int glowR = radius + 12 + static_cast<int>(std::sin(pulsePhase * 4.0f) * 5.0f);
    HBRUSH glowBrush = CreateSolidBrush(RGB(32, 68, 118));
    HPEN nullPen = CreatePen(PS_NULL, 0, 0);
    HGDIOBJ oldB = SelectObject(hdc, glowBrush);
    HGDIOBJ oldP = SelectObject(hdc, nullPen);
    Ellipse(hdc, cx - glowR, cy - glowR, cx + glowR, cy + glowR);
    DeleteObject(glowBrush);
    DeleteObject(nullPen);

    POINT pts[5] = {
        {cx, cy - radius},
        {cx + static_cast<int>(radius * 0.72), cy - static_cast<int>(radius * 0.2)},
        {cx + static_cast<int>(radius * 0.45), cy + radius},
        {cx - static_cast<int>(radius * 0.45), cy + radius},
        {cx - static_cast<int>(radius * 0.72), cy - static_cast<int>(radius * 0.2)}
    };
    HBRUSH bodyBrush = CreateSolidBrush(RGB(65, 195, 245));
    HPEN borderPen = CreatePen(PS_SOLID, 2, RGB(225, 250, 255));
    SelectObject(hdc, bodyBrush);
    SelectObject(hdc, borderPen);
    Polygon(hdc, pts, 5);

    POINT facetLeft[3] = {pts[0], pts[4], {cx, cy}};
    HBRUSH darkFacet = CreateSolidBrush(RGB(42, 150, 210));
    SelectObject(hdc, darkFacet);
    Polygon(hdc, facetLeft, 3);

    POINT facetRight[3] = {pts[0], pts[1], {cx, cy}};
    HBRUSH lightFacet = CreateSolidBrush(RGB(155, 238, 255));
    SelectObject(hdc, lightFacet);
    Polygon(hdc, facetRight, 3);

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(bodyBrush);
    DeleteObject(darkFacet);
    DeleteObject(lightFacet);
    DeleteObject(borderPen);
}

void drawKingdomScenery(HDC hdc, int width, int height, float elapsedSec, int themeMode) {
    // Distant mountain ridge
    COLORREF mtnCol = (themeMode == 3) ? RGB(12, 36, 34) : RGB(18, 24, 52);
    HBRUSH mtnBrush = CreateSolidBrush(mtnCol);
    HPEN nullPen = CreatePen(PS_NULL, 0, 0);
    HGDIOBJ oldB = SelectObject(hdc, mtnBrush);
    HGDIOBJ oldP = SelectObject(hdc, nullPen);

    POINT ridge[7] = {
        {0, height},
        {0, height - 140},
        {240, height - 290},
        {520, height - 170},
        {860, height - 310},
        {width, height - 150},
        {width, height}
    };
    Polygon(hdc, ridge, 7);
    DeleteObject(mtnBrush);

    // Foreground castle towers
    HBRUSH silBrush = CreateSolidBrush(RGB(10, 14, 30));
    HPEN silPen = CreatePen(PS_SOLID, 1, RGB(28, 38, 72));
    SelectObject(hdc, silBrush);
    SelectObject(hdc, silPen);

    Rectangle(hdc, 35, height - 170, 105, height);
    POINT spire1[3] = {{30, height - 170}, {70, height - 245}, {110, height - 170}};
    Polygon(hdc, spire1, 3);
    Rectangle(hdc, 118, height - 125, 205, height);

    Rectangle(hdc, width - 105, height - 170, width - 35, height);
    POINT spire2[3] = {{width - 110, height - 170}, {width - 70, height - 245}, {width - 30, height - 170}};
    Polygon(hdc, spire2, 3);
    Rectangle(hdc, width - 205, height - 125, width - 118, height);

    // Glowing castle windows
    HBRUSH winBrush = CreateSolidBrush(RGB(245, 205, 85));
    SelectObject(hdc, winBrush);
    Rectangle(hdc, 62, height - 140, 78, height - 118);
    Rectangle(hdc, width - 78, height - 140, width - 62, height - 118);

    // Drifting fireflies / magic motes
    HBRUSH fireflyBrush = CreateSolidBrush(themeMode == 3 ? RGB(135, 255, 165) : RGB(130, 215, 255));
    SelectObject(hdc, fireflyBrush);
    SelectObject(hdc, nullPen);
    for (int i = 0; i < 18; ++i) {
        int fx = (i * 73 + static_cast<int>(elapsedSec * (14 + (i % 5) * 3))) % width;
        int fy = height - 80 - ((i * 47) % 260) + static_cast<int>(std::sin(elapsedSec * 2.2f + i) * 14.0f);
        Ellipse(hdc, fx - 2, fy - 2, fx + 2, fy + 2);
    }

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(silBrush);
    DeleteObject(silPen);
    DeleteObject(winBrush);
    DeleteObject(fireflyBrush);
    DeleteObject(nullPen);
}

void drawGuardianBoss(HDC hdc, int cx, int cy, int levelNum, int hpRemaining, int maxHp, float elapsedSec) {
    int hoverY = cy + static_cast<int>(std::sin(elapsedSec * 3.5f) * 5.0f);
    COLORREF robeColors[5] = {
        RGB(68, 125, 215),  // Temple Oracle
        RGB(215, 155, 55),  // Golden Architect
        RGB(52, 168, 118),  // Forest Warden
        RGB(165, 85, 215),  // Portal Arbiter
        RGB(220, 65, 95)    // Shadow Sorcerer Malakor
    };
    COLORREF col = robeColors[(levelNum - 1) % 5];

    // Aura ring
    HBRUSH aura = CreateSolidBrush(RGB(28, 38, 72));
    HPEN auraPen = CreatePen(PS_SOLID, 2, col);
    HGDIOBJ oldB = SelectObject(hdc, aura);
    HGDIOBJ oldP = SelectObject(hdc, auraPen);
    Ellipse(hdc, cx - 44, hoverY - 44, cx + 44, hoverY + 44);

    // Cloak / Body
    HBRUSH body = CreateSolidBrush(col);
    SelectObject(hdc, body);
    POINT cloak[3] = {{cx, hoverY - 24}, {cx + 28, hoverY + 32}, {cx - 28, hoverY + 32}};
    Polygon(hdc, cloak, 3);

    // Hood & Glowing Eyes
    HBRUSH hood = CreateSolidBrush(RGB(18, 22, 42));
    SelectObject(hdc, hood);
    Ellipse(hdc, cx - 16, hoverY - 32, cx + 16, hoverY - 2);

    HBRUSH eyeBrush = CreateSolidBrush(RGB(255, 235, 110));
    SelectObject(hdc, eyeBrush);
    Ellipse(hdc, cx - 8, hoverY - 20, cx - 3, hoverY - 15);
    Ellipse(hdc, cx + 3, hoverY - 20, cx + 8, hoverY - 15);

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(aura);
    DeleteObject(auraPen);
    DeleteObject(body);
    DeleteObject(hood);
    DeleteObject(eyeBrush);

    // Guardian HP Bar below avatar
    float ratio = (maxHp > 0) ? (static_cast<float>(hpRemaining) / static_cast<float>(maxHp)) : 0.0f;
    drawProgressBar(hdc, cx - 45, cy + 48, 90, 10, ratio, RGB(235, 78, 95));
}

void drawHeroAvatar(HDC hdc, int cx, int cy, float elapsedSec) {
    int bob = static_cast<int>(std::sin(elapsedSec * 5.0f) * 3.0f);
    HBRUSH cape = CreateSolidBrush(RGB(235, 185, 55));
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 245, 200));
    HGDIOBJ oldB = SelectObject(hdc, cape);
    HGDIOBJ oldP = SelectObject(hdc, pen);

    POINT body[3] = {{cx, cy - 14 + bob}, {cx + 12, cy + 14 + bob}, {cx - 12, cy + 14 + bob}};
    Polygon(hdc, body, 3);
    Ellipse(hdc, cx - 8, cy - 26 + bob, cx + 8, cy - 10 + bob);

    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(cape);
    DeleteObject(pen);
}

void drawTextLeft(HDC hdc, HFONT font, const std::string& text, int x, int y, COLORREF color) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    HGDIOBJ oldFont = SelectObject(hdc, font);
    TextOutA(hdc, x, y, text.c_str(), static_cast<int>(text.size()));
    SelectObject(hdc, oldFont);
}

void drawTextCentered(HDC hdc, HFONT font, const std::string& text, RECT rect, COLORREF color) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    HGDIOBJ oldFont = SelectObject(hdc, font);
    DrawTextA(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
}

void drawMultilineText(HDC hdc, HFONT font, const std::string& text, int x, int y, int w, int h, COLORREF color) {
    RECT r = {x, y, x + w, y + h};
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    HGDIOBJ oldFont = SelectObject(hdc, font);
    DrawTextA(hdc, text.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_WORDBREAK);
    SelectObject(hdc, oldFont);
}

} // namespace qc
