#include "Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f dimensions, const sf::String& text) : Label(text, position) {
    m_body.setSize(dimensions);
    m_body.setPosition({position.x - dimensions.x / 2, position.y - dimensions.y / 2});

    m_body.setOutlineThickness(2);
    m_body.setFillColor(sf::Color(107, 153, 255, 100));
    m_body.setOutlineColor(sf::Color(35, 44, 127, 200));

    hover.setBuffer(AssetManager::GetSoundBuffer("res/sound/click_error.wav"));
    click.setBuffer(AssetManager::GetSoundBuffer("res/sound/menu_click.wav"));
    h = false;
    c = false;
}

Button::~Button() {
}

void Button::draw(sf::RenderTarget& target) {
    target.draw(m_body);
    target.draw(m_text);
}

// FUTURE:  here might be sound manager
void Button::update(sf::Window& window) {
    if (m_body.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
        const auto& button = InputManager::getButton(sf::Mouse::Left);
        if (m_body.getGlobalBounds().contains(sf::Vector2f(button.clickPosition)) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            m_body.setFillColor(sf::Color(107, 153, 255, 255));
            if (!c) {
                click.play();
                c = true;
            }
        } else {
            m_body.setFillColor(sf::Color(107, 153, 255, 180));
            if (!h) {
                hover.play();
                h = true;
            }
        }
    } else if (m_body.getFillColor() != sf::Color(107, 153, 255, 100)) {
        m_body.setFillColor(sf::Color(107, 153, 255, 100));
        h = false;
        c = false;
    }
}

bool Button::isButtonClicked() {
    const auto& button = InputManager::getButton(sf::Mouse::Left);
    if (m_body.getGlobalBounds().contains(sf::Vector2f(button.clickPosition)) &&
        m_body.getGlobalBounds().contains(sf::Vector2f(button.releasePosition)))
        return true;
    return false;
}
