#pragma once

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Player.h"
#include "Server.h"

class Game {
   public:
    Game(ushort port);
    void run();

   private:
    std::vector<Player> m_playerList;
    Server m_network;

    static const sf::Time m_timePerFrame;
    static const sf::Time m_serverTick;
    bool m_running;
};
