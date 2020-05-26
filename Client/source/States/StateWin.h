#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../GUI/Button.h"
#include "../Managers/InputManager.h"
#include "../Managers/StateManager.h"
#include "State.h"
#include "StateConnect.h"
#include "StateMainMenu.h"
#include "StatePlay.h"

class StateWin : public State {
   public:
    StateWin(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);
    ~StateWin();

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
