#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>
#include <iostream>

#include "../GUI/Button.h"
#include "../GUI/TextEntry.h"
#include "../Managers/StateManager.h"
#include "State.h"
#include "StateMainMenu.h"

class StateConnect : public State {
   public:
    StateConnect(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);
    ~StateConnect();

    void pause();
    void resume();

    void update(sf::Time dt);
    void draw();

    void handleKeyboardInput(sf::Keyboard::Key& key, bool status);
    void handleTextInput(sf::Uint32 code);

   private:
    std::vector<Button> m_buttons;
    TextEntry entry;
    sf::Text nameLabel;
    sf::Sprite background;
};
