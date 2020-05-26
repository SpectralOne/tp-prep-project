#include "Menu.h"

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::addItem(const std::string text) {
    m_items.push_back((Button(sf::Vector2f(1280 / 2, m_verticalPosition), sf::Vector2f(400, 60), text)));
    m_verticalPosition += 100;
}

void Menu::update(sf::Time dt) {
}
