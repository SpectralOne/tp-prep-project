#pragma once
#include <SFML/Graphics.hpp>

#include "../Managers/AssetManager.h"

class Label {
   public:
    Label() { m_text.setFont(AssetManager::GetFont("res/font/arial.ttf")); };
    Label(const sf::String& text, sf::Vector2f position, unsigned int text_size = 50);
    void setString(const std::string& text);
    void setTextSize(unsigned int size);
    void setPosition(sf::Vector2f position);
    void draw(sf::RenderTarget& target);

   protected:
    sf::Text m_text;
};
