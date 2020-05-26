#include "StateDead.h"

StateDead::StateDead(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) : 
    State(machine, window, info, replace) {
    m_view.setSize(1280, 720);
    m_view.setCenter(sf::Vector2f(1280 / 2, 720 / 2));

    background.setTexture(AssetManager::GetTexture("res/img/background_gameover.jpg"));
    background.setScale({1.50234742, 1.5});
    logo.setTexture(AssetManager::GetTexture("res/img/game_over_logo.png"));
    logo.setPosition({1280/2 - 248, 0});
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 200), sf::Vector2f(400, 60), "Spectate"));
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 300), sf::Vector2f(400, 60), "Main Menu"));
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 400), sf::Vector2f(400, 60), "Exit"));
}

StateDead::~StateDead() {
}

void StateDead::pause() {
}

void StateDead::resume() {
}

void StateDead::update(sf::Time dt) {
    // Process the events
    handleEvents();
    // Check button press
    for (unsigned int i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i].update(m_window);
        bool status = m_buttons[i].isButtonClicked();

        // Menu options
        if (i == 0 && status) {
            m_machine.lastState();
        } else if (i == 1 && status) {
            m_next = StateManager::build<StateMainMenu>(m_machine, m_window, this->info, true);
            m_clear = true;
        } else if (i == 2 && status)
            m_machine.quit();
    }
}

void StateDead::handleEvents() {
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
            default:
                break;
        }
    }
}

void StateDead::handleKeyboardInput(sf::Keyboard::Key& key, bool status) {
}

void StateDead::draw() {
    m_window.setView(m_view);
    m_window.clear(sf::Color::White);
    m_window.draw(background);
    m_window.draw(logo);
    for (auto& button : m_buttons)
        button.draw(m_window);
    m_window.display();
}
