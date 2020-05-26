#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "../Managers/AssetManager.h"

class TextBoxScrollable {
   public:
    TextBoxScrollable(sf::Vector2f position, sf::Vector2f size);
    void draw(sf::RenderTarget& target);
    void update(float dt);
    void setPosition(sf::Vector2f position);
    void addString(const std::string& text, sf::Color color);
    void scroll(int value);

   private:
    sf::RectangleShape m_frame;
    sf::RectangleShape m_slider;
    std::vector<sf::Text> m_text;
    unsigned int m_characterSize;
    float m_sliderOffset;
    float m_textOffset;
    int m_visibleTextStart;
    int m_visibleTextEnd;
    int m_step;
};
