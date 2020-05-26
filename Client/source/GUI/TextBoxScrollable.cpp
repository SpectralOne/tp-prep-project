#include "TextBoxScrollable.h"

TextBoxScrollable::TextBoxScrollable(sf::Vector2f position, sf::Vector2f size) {
    m_frame.setSize(size);
    m_frame.setPosition(position);
    m_frame.setFillColor(sf::Color(70, 60, 50, 80));
    m_frame.setOutlineColor(sf::Color(80, 70, 60, 140));
    m_frame.setOutlineThickness(3);

    m_slider.setFillColor(sf::Color(80, 70, 60, 100));
    m_slider.setOutlineColor(sf::Color(80, 70, 60, 140));
    m_slider.setSize({20, 80});
    m_characterSize = 15;
    m_sliderOffset = m_frame.getSize().y - m_slider.getSize().y;
    m_textOffset = 0;
    m_visibleTextStart = 0;
    m_visibleTextEnd = 0;
    m_step = 0;
}

void TextBoxScrollable::draw(sf::RenderTarget& window) {
    window.draw(m_frame);
    window.draw(m_slider);
    for (int i = m_visibleTextStart - m_step; i > m_visibleTextEnd - m_step; --i) {
        window.draw(m_text[i - 1]);
    }
}

void TextBoxScrollable::update(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_slider.getPosition().y >= m_frame.getPosition().y) {
        scroll(dt * -0.2);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_slider.getPosition().y + m_slider.getSize().y <= m_frame.getPosition().y + m_frame.getSize().y) {
        scroll(dt * 0.2);
    }
}

void TextBoxScrollable::setPosition(sf::Vector2f position) {
    m_frame.setPosition(position);
    m_slider.setPosition({position.x + m_frame.getSize().x - m_slider.getSize().x, position.y + m_sliderOffset});

    for (int i = 0; i < m_text.size(); ++i) {
        float row_height = (m_text.size() - i) * m_characterSize;
        m_text[i].setPosition(position.x + 5, m_frame.getPosition().y + m_frame.getSize().y - (row_height - m_textOffset) - 5);
    }
}

void TextBoxScrollable::addString(const std::string& text, sf::Color color) {
    sf::Text temp_message = sf::Text(text, AssetManager::GetFont("res/font/arial.ttf"), m_characterSize - 2);
    temp_message.setFillColor(color);
    m_text.push_back(std::move(temp_message));

    // Set the liming of visible messages
    m_visibleTextStart = m_text.size();
    if (m_visibleTextStart > 13) {
        m_visibleTextEnd = m_visibleTextStart - 13;
    }
}

void TextBoxScrollable::scroll(int value) {
    float current_difference = m_frame.getPosition().y + m_frame.getSize().y - m_slider.getPosition().y - m_slider.getSize().y + 1;
    float total_difference = m_frame.getSize().y - m_slider.getSize().y + 1;

    if (m_text.size() >= 14) {
        float increment = total_difference / (m_text.size() - 13);
        m_step = current_difference / increment;
        m_textOffset = m_step * m_characterSize;
    }
    m_sliderOffset += value;
}