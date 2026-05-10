//
// Created by axel on 2026-05-10.
//

#ifndef POKEMONRPG_DIALOGBOX_H
#define POKEMONRPG_DIALOGBOX_H

#include <string>

class DialogBox {
public:
    DialogBox();

    void Show(const std::string& speaker, const std::string& text);
    void Close();
    bool IsOpen() const { return m_open; }
    void Draw() const;

private:
    bool m_open;
    std::string m_speaker;
    std::string m_text;
};

#endif //POKEMONRPG_DIALOGBOX_H
