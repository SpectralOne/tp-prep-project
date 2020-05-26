#include "Game.h"

const sf::Time Game::m_timePerFrame = sf::seconds(1.0f / 60.0f);
const sf::Time Game::m_serverTick = sf::seconds(1.0f / 10.0f);

Game::Game(ushort port) : m_network(port) {
    m_running = true;
    m_network.setPlayerList(&m_playerList);
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSincleLastUpdate = sf::Time::Zero;
    sf::Time timeSincleLastTick = sf::Time::Zero;

    while (m_running) {
        sf::Time x = clock.restart();
        timeSincleLastUpdate += x;
        timeSincleLastTick += x;

        // Process received requests
        m_network.update();
        while (timeSincleLastUpdate > m_timePerFrame) {
            timeSincleLastUpdate -= m_timePerFrame;
            // Update world simulation
            m_network.updateWorld(m_timePerFrame);
        }
        // Update server tick
        while (timeSincleLastTick > m_serverTick) {
            timeSincleLastTick -= m_serverTick;
            // Update world simulation
            m_network.updateTick();
        }
    }
}
