#include "Level.h"

Level::Level() {
    //  asset manager (rework)
    if (!m_map.load("res/img/sheet.png", {64, 64}, lvl, 64, 32))
        std::cout << "Error loading the map in Level.cpp\n";
}

void Level::draw(sf::RenderWindow& window) {
    window.draw(m_map);
}
