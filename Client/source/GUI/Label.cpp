#include "Label.h"

Label::Label(const sf::String& text, sf::Vector2f position, unsigned int text_size) {
    m_text.setFont(AssetManager::GetFont("res/font/arial.ttf"));
    m_text.setString(text);
    m_text.setCharacterSize(text_size);
    m_text.setFillColor(sf::Color::Black);
    m_text.setPosition({position.x - m_text.getGlobalBounds().width / 2, position.y - m_text.getGlobalBounds().height / 2 - 12});
}

void Label::draw(sf::RenderTarget& target) {
    target.draw(m_text);
}

void Label::setString(const std::string& text) {
    m_text.setString(text);
}

void Label::setTextSize(unsigned int size) {
    m_text.setCharacterSize(size);
}

void Label::setPosition(sf::Vector2f position) {
    m_text.setPosition({position.x - m_text.getGlobalBounds().width / 2, position.y - m_text.getGlobalBounds().height / 2});
}
