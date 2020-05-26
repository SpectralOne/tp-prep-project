#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>
#include <iostream>

#include "../GUI/Button.h"
#include "../Managers/StateManager.h"
#include "State.h"
#include "StateConnect.h"
#include "StateSettings.h"

class StateMainMenu : public State {
   public:
    StateMainMenu(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);
    ~StateMainMenu();

    void pause();
    void resume();

    void update(sf::Time dt);
    void draw();

    void handleKeyboardInput(sf::Keyboard::Key& key, bool status);

   private:
    std::vector<Button> m_buttons;
    sf::Sprite background;
};
