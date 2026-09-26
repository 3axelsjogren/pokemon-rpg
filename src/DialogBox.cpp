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

    // Text, radbryts så långa repliker inte klipps av
    const int fontSize = 16;
    const int maxWidth = SCREEN_WIDTH - 48;
    int y = SCREEN_HEIGHT - 85;
    std::string line;
    size_t pos = 0;
    while (pos < m_text.size()) {
        size_t space = m_text.find(' ', pos);
        if (space == std::string::npos) space = m_text.size();
        std::string word = m_text.substr(pos, space - pos);
        std::string candidate = line.empty() ? word : line + " " + word;
        if (!line.empty() && MeasureText(candidate.c_str(), fontSize) > maxWidth) {
            DrawText(line.c_str(), 24, y, fontSize, WHITE);
            y += fontSize + 4;
            line = word;
        } else {
            line = candidate;
        }
        pos = space + 1;
    }
    if (!line.empty()) DrawText(line.c_str(), 24, y, fontSize, WHITE);

    // Hint
    DrawText("E - Stäng", SCREEN_WIDTH - 100, SCREEN_HEIGHT - 30, 14, LIGHTGRAY);
}
