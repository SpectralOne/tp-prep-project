#include "StateSettings.h"

StateSettings::StateSettings(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) : 
    State(machine, window, info, replace),
    entry(400, 60, 46, info.pName) {
    background.setTexture(AssetManager::GetTexture("res/img/background_menu.jpg"));
    m_buttons.push_back(Button(sf::Vector2f(100, 100), sf::Vector2f(80, 60), "<="));
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 500), sf::Vector2f(400, 60), "Apply"));

    entry.setPosition(sf::Vector2f(440, 370));

    nameLabel.setFont(AssetManager::GetFont("res/font/arial.ttf"));    
    nameLabel.setString("Current name:");
    nameLabel.setCharacterSize(46);
    nameLabel.setFillColor(sf::Color::White);
    nameLabel.setPosition(440, 270);
}

StateSettings::~StateSettings() {
}

void StateSettings::pause() {
}

void StateSettings::resume() {
}

void StateSettings::update(sf::Time dt) {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                m_machine.quit();
            } break;
            case sf::Event::KeyPressed: {
                handleKeyboardInput(event.key.code, true);
            } break;
            case sf::Event::KeyReleased: {
                handleKeyboardInput(event.key.code, false);
            } break;
            case sf::Event::MouseButtonPressed: {
                InputManager::setButtonClick(event.mouseButton.button, sf::Mouse::getPosition(m_window));
            } break;
            case sf::Event::MouseButtonReleased: {
                InputManager::setButtonRelease(event.mouseButton.button, sf::Mouse::getPosition(m_window));
            } break;
            case sf::Event::TextEntered: {
                handleTextInput(event.text.unicode);
            } break;
            default:
                break;
        }
    }

    for (unsigned int i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i].update(m_window);
        bool status = m_buttons[i].isButtonClicked();

        if (i == 0 && status) {
            m_next = StateManager::build<StateMainMenu>(m_machine, m_window, this->info, true);
        } else if (i == 1 && status) {
            std::string newName = entry.getString();
            if (newName.size() > 0) {
                (*this).setPlayerName(newName);
            } else {
                (*this).setPlayerName(this->info.pName);
            }
            m_next = StateManager::build<StateSettings>(m_machine, m_window, this->info, true);
        }
    }
    entry.updateActive(m_window);
    entry.update(dt.asMilliseconds());
}

void StateSettings::handleKeyboardInput(sf::Keyboard::Key& key, bool status) {
    if (key == InputManager::getKeyAction("CHAT") && status) {
        if (entry.isActive()) {
            entry.setActive(false);
        }
    }
    if (key == sf::Keyboard::BackSpace && status) {
        entry.removeChar();
    }
    if (key == sf::Keyboard::Escape && status && !entry.isActive()) {
        m_next = StateManager::build<StateMainMenu>(m_machine, m_window, this->info, true);
    } 
    if (key == sf::Keyboard::Escape && status && entry.isActive()) {
        entry.setActive(false);
        entry.setString(this->info.pName);
    }
}

void StateSettings::handleTextInput(sf::Uint32 code) {
    // 127 shift-backspace
    if (code < 127 && code > 27) {
        entry.addChar(static_cast<char>(code));
    }
}

void StateSettings::draw() {
    m_window.clear(sf::Color::White);
    m_window.draw(background);
    for (auto& button : m_buttons)
        button.draw(m_window);
    entry.draw(m_window);

    m_window.draw(nameLabel);
    m_window.display();
}
