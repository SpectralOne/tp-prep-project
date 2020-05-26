#include "Game.h"

const sf::Time Game::m_timePerFrame = sf::seconds(1.0f / 60.0f);

Game::Game() {
    m_window.create(sf::VideoMode(1280, 720), "Game", sf::Style::Titlebar | sf::Style::Close);
    AssetManager::GetFont("res/font/ka1.ttf");

    pInfo_t info;
    info.pName = "Noname";
    
    m_stateManager.run(StateManager::build<StateMainMenu>(m_stateManager, m_window, info, true));
}

Game::~Game() {
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSincleLastUpdate = sf::Time::Zero;

    while (m_stateManager.running()) {
        m_stateManager.nextState();

        timeSincleLastUpdate += clock.restart();
        while (timeSincleLastUpdate > m_timePerFrame) {
            timeSincleLastUpdate -= m_timePerFrame;
            m_stateManager.update(m_timePerFrame);
        }
        m_stateManager.draw();
    }
}
