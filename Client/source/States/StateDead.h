#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../GUI/Button.h"
#include "../Managers/InputManager.h"
#include "../Managers/StateManager.h"
#include "State.h"
#include "StateMainMenu.h"
#include "StatePlay.h"
#include "StateConnect.h"

class StateDead : public State {
   public:
    StateDead(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);
    ~StateDead();

    void pause();
    void resume();

    void update(sf::Time dt);
    void draw();
    void handleEvents();

    void handleKeyboardInput(sf::Keyboard::Key& key, bool status);

   private:
    std::vector<Button> m_buttons;

    sf::View m_view;
    sf::Sprite background;
    sf::Sprite logo;
};
