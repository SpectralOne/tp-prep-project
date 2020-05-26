#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Managers/AssetManager.h"
#include "Managers/InputManager.h"
#include "Managers/StateManager.h"
#include "States/StateMainMenu.h"

class Game {
   public:
    Game();
    ~Game();
    void run();

   private:
    StateManager m_stateManager;
    AssetManager m_assetManager;
    InputManager m_inputManager;

    sf::RenderWindow m_window;

    static const sf::Time m_timePerFrame;
    sf::Clock m_clock;
    sf::Time m_tempTime;
};
