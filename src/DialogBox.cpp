//
// Created by axel on 2026-05-10.
//

#include "DialogBox.h"
#include "raylib.h"
#include "Constants.h"

DialogBox::DialogBox() : m_open(false) {}

void DialogBox::Show(const std::string& speaker, const std::string& text) {
    m_speaker = speaker;
    m_text = text;
    m_open = true;
}

void DialogBox::Close() {
    m_open = false;
}

void DialogBox::Draw() const {
    if (!m_open) return;

    // Bakgrund
    DrawRectangle(10, SCREEN_HEIGHT - 120, SCREEN_WIDTH - 20, 110, { 0, 0, 0, 200 });
    DrawRectangleLines(10, SCREEN_HEIGHT - 120, SCREEN_WIDTH - 20, 110, WHITE);

    // Namn
    DrawText(m_speaker.c_str(), 24, SCREEN_HEIGHT - 110, 18, YELLOW);

    // Text
    DrawText(m_text.c_str(), 24, SCREEN_HEIGHT - 85, 16, WHITE);

    // Hint
    DrawText("E - Stäng", SCREEN_WIDTH - 100, SCREEN_HEIGHT - 30, 14, LIGHTGRAY);
}
