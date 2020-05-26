#include "TextEntry.h"

TextEntry::TextEntry() {
    m_frame.setSize({400, 20});
    m_frame.setFillColor(sf::Color(50, 50, 50, 20));
    m_frame.setOutlineColor(sf::Color(70, 70, 70, 100));
    m_frame.setOutlineThickness(-1);

    m_text.setFont(AssetManager::GetFont("res/font/arial.ttf"));
    m_text.setCharacterSize(15);
    m_text.setPosition(100, 100);

    m_isActive = false;
    m_caretTimer = 500;
    m_caret = false;
}

TextEntry::TextEntry(float sizeX, float sizeY, float charSize, sf::String deftxt) {
    m_frame.setSize({sizeX, sizeY});
    m_frame.setFillColor(sf::Color(50, 50, 50, sizeY));
    m_frame.setOutlineColor(sf::Color(70, 70, 70, 100));
    m_frame.setOutlineThickness(-1);

    m_text.setFont(AssetManager::GetFont("res/font/arial.ttf"));
    m_text.setCharacterSize(charSize);
    m_text.setPosition(100, 100);
    m_text.setString(deftxt);
    m_isActive = false;
    m_caretTimer = 500;
    m_caret = false;
}

void TextEntry::update(float dt) {
    if (m_isActive) {
        m_caretTimer += dt;
        if (m_caretTimer >= 700 && !m_caret && (static_cast<char>(m_text.getString().getData()[m_text.getString().getSize() - 1]) != '|')) {
            m_text.setString(m_text.getString() + "|");
            m_caret = true;
        } else if (m_caretTimer >= 1400) {
            removeCaret();
            m_caretTimer = 0;
            m_caret = false;
        }
    }
}

void TextEntry::draw(sf::RenderTarget& window) {
    window.draw(m_frame);
    window.draw(m_text);
}

void TextEntry::addChar(char text) {
    removeCaret();
    m_text.setString(m_text.getString() + text);
}

void TextEntry::removeChar() {
    removeCaret();
    sf::String temp = m_text.getString();
    if (temp.getSize() > 0)
        temp.erase(temp.getSize() - 1, 1);
    m_text.setString(temp);
}

void TextEntry::setPosition(sf::Vector2f position) {
    m_frame.setPosition(position);
    m_text.setPosition(position.x + 5, position.y);
}

std::string TextEntry::getString() {
    removeCaret();
    std::string x = m_text.getString();
    return x;
}

void TextEntry::clear() {
    m_text.setString("");
}

void TextEntry::setActive(bool status) {
    m_isActive = status;

    if (status) {
        m_frame.setFillColor(sf::Color(50, 50, 50, 200));
        m_frame.setOutlineColor(sf::Color(70, 70, 70, 255));
    } else {
        m_frame.setFillColor(sf::Color(50, 50, 50, 20));
        m_frame.setOutlineColor(sf::Color(70, 70, 70, 100));
        (*this).removeCaret();
    }
}

void TextEntry::removeCaret() {
    if (m_text.getString().getData()[m_text.getString().getSize() - 1] == '|' && m_caret) {
        sf::String temp = m_text.getString();
        temp.erase(temp.getSize() - 1, 1);
        m_text.setString(temp);
    }
}

void TextEntry::updateActive(sf::Window& window) {
    if (m_frame.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
        const auto& button = InputManager::getButton(sf::Mouse::Left);
        if (m_frame.getGlobalBounds().contains(sf::Vector2f(button.clickPosition)) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->setActive(!this->m_isActive);
            sf::sleep(sf::milliseconds(200));
        }
    }
}

bool TextEntry::isActive() {
    return this->m_isActive;
}

void TextEntry::setString(std::string text) {
    m_text.setString(sf::String(text));
}
