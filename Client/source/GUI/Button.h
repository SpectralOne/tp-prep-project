#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "../Managers/InputManager.h"
#include "Label.h"

class Button : public Label {
   public:
    Button(sf::Vector2f position, sf::Vector2f dimensions, const sf::String& text);
    ~Button();
    void draw(sf::RenderTarget& target);
    void update(sf::Window& window);
    bool isButtonClicked();

   private:
    sf::RectangleShape m_body;
    sf::Sound hover;
    sf::Sound click;
    bool h;
    bool c;
};
