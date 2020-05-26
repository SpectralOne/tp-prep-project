#include "StatePlayMenu.h"

StatePlayMenu::StatePlayMenu(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) : 
    State(machine, window, info, replace) {
    background.setTexture(AssetManager::GetTexture("res/img/background_menu.jpg"));
    m_view.setSize(1280, 720);
    m_view.setCenter(sf::Vector2f(1280 / 2, 720 / 2));

    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 200), sf::Vector2f(400, 60), "Resume"));
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 300), sf::Vector2f(400, 60), "Main Menu"));
    m_buttons.push_back(Button(sf::Vector2f(1280 / 2, 400), sf::Vector2f(400, 60), "Exit"));
}

StatePlayMenu::~StatePlayMenu() {
}

void StatePlayMenu::pause() {
}

void StatePlayMenu::resume() {
}

void StatePlayMenu::update(sf::Time dt) {
    // Process the events
    handleEvents();

    if (!info.isCon) {
        m_next = StateManager::build<StateConnect>(m_machine, m_window, this->info, true);
        return;
    }

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

void StatePlayMenu::handleEvents() {
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

void StatePlayMenu::handleKeyboardInput(sf::Keyboard::Key& key, bool status) {
}

void StatePlayMenu::draw() {
    m_window.setView(m_view);
    m_window.clear(sf::Color::White);
    m_window.draw(background);
    for (auto& button : m_buttons)
        button.draw(m_window);
    m_window.display();
}
