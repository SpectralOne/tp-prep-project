#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Managers/AssetManager.h"
#include "../Managers/InputManager.h"

class TextEntry {
   public:
    TextEntry();
    TextEntry(float sizeX, float sizeY, float charSize, sf::String deftxt);
    void update(float dt);
    void draw(sf::RenderTarget& window);
    void addChar(char text);
    void removeChar();
    void setPosition(sf::Vector2f position);
    std::string getString();
    void clear();
    void setActive(bool status);
    void updateActive(sf::Window& window);
    bool isActive();
    void setString(std::string text);

   protected:
    void removeCaret();
    sf::Text m_text;
    sf::RectangleShape m_frame;

    bool m_isActive;
    float m_caretTimer;
    bool m_caret;
};
