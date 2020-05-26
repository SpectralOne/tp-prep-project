#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Button.h"

class Menu {
   public:
    Menu();
    ~Menu();

    void addItem(const std::string text);
    void update(sf::Time dt);

   private:
    std::vector<Button> m_items;
    float m_verticalPosition = 200;
};
