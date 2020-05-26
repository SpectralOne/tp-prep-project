#pragma once
#include <SFML/Graphics.hpp>

#include "Chat.h"
#include "Label.h"

class GUI {
   public:
    GUI(std::string name);
    ~GUI();
    void update(float dt);
    void draw(sf::RenderTarget& window);
    void setCenter(sf::Vector2f center);
    void setText(std::string text);

    Chat chat;

   private:
    Label m_name;
    sf::Text info;
};
