#include <arpa/inet.h>

#include "StateConnect.h"


StateConnect::StateConnect(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) : 
    State(machine, window, info, replace),
    entry(400, 60, 42, "127.0.0.1:80") {

    background.setTexture(AssetManager::GetTexture("res/img/background_menu.jpg"));
    m_buttons.push_back(Button(sf::Vector2f(100, 100), sf::Vector2f(80, 60), "<="));
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 500), sf::Vector2f(400, 60), "Connect"));

    entry.setPosition(sf::Vector2f(440, 370));

    nameLabel.setFont(AssetManager::GetFont("res/font/arial.ttf"));    
    nameLabel.setString("Connect state:");
    nameLabel.setCharacterSize(46);
    nameLabel.setFillColor(sf::Color::White);
    nameLabel.setPosition(440, 270);
}

StateConnect::~StateConnect() {
}

void StateConnect::pause() {
}

void StateConnect::resume() {
}

void StateConnect::update(sf::Time dt) {
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
            std::string ipString = entry.getString();
            if (ipString.size() > 9) {
                char adr[16] = { 0 };
                ipString.copy(adr, ipString.find(":"));

                struct sockaddr_in sa;
                int result = inet_pton(AF_INET, adr, &(sa.sin_addr));
                if (result == 1) {
                    char port[6] = { 0 };
                    ipString.copy(port, ipString.rfind(":"), ipString.find(":") + 1);
                    this->info.ip = std::string(adr);
                    this->info.port = static_cast<unsigned short>(std::stoul(std::string(port)));

                    m_next = StateManager::build<StatePlay>(m_machine, m_window, this->info, true);

                } else {
                    std::cout << "invalid adr";
                    m_next = StateManager::build<StateConnect>(m_machine, m_window, this->info, true);
                }
            } else {
                std::cout << "invalid inp";
                m_next = StateManager::build<StateConnect>(m_machine, m_window, this->info, true);
            }
        }
    }
    entry.updateActive(m_window);
    entry.update(dt.asMilliseconds());
}

void StateConnect::handleKeyboardInput(sf::Keyboard::Key& key, bool status) {
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
}

void StateConnect::handleTextInput(sf::Uint32 code) {
    // 127 shift-backspace
    if (code < 127 && code > 27) {
        entry.addChar(static_cast<char>(code));
    }
}

void StateConnect::draw() {
    m_window.clear(sf::Color::White);
    m_window.draw(background);
    for (auto& button : m_buttons)
        button.draw(m_window);
    entry.draw(m_window);

    m_window.draw(nameLabel);
    m_window.display();
}
