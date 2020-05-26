#include "GUI.h"

GUI::GUI(std::string name) {
    m_name.setString(name);
    m_name.setPosition({200, 200});
    m_name.setTextSize(20);

    info.setFont(AssetManager::GetFont("res/font/arial.ttf"));
    // info.setString("");
    info.setCharacterSize(50);
    info.setFillColor(sf::Color::Black);
    info.setPosition({360 - info.getGlobalBounds().width / 2, 360 - info.getGlobalBounds().height / 2 - 260});
}

GUI::~GUI() {
}

void GUI::draw(sf::RenderTarget& window) {
    m_name.draw(window);
    chat.draw(window);
    window.draw(info);
}

void GUI::update(float dt) {
    chat.update(dt);
}

void GUI::setCenter(sf::Vector2f center) {
    m_name.setPosition({center.x, center.y - 50});
    info.setPosition({center.x - info.getGlobalBounds().width / 2, center.y - info.getGlobalBounds().height / 2 - 260});
    chat.setPosition(center);
}

void GUI::setText(std::string text) {
    info.setString(text);
}
