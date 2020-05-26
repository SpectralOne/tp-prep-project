#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../Entities/Enemy.h"
#include "../Entities/Player.h"
#include "../Entities/Bullet.h"
#include "../GUI/GUI.h"
#include "../GUI/TabOverlay.h"
#include "../Managers/AssetManager.h"
#include "../Managers/StateManager.h"
#include "../Network/Network.h"
#include "../World/Level.h"
#include "State.h"
#include "StatePlayMenu.h"
#include "StateDead.h"
#include "StateWin.h"

class StatePlay : public State {
   public:
    StatePlay(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);
    ~StatePlay();

    void pause();
    void resume();

    void update(sf::Time dt);
    void draw();

    void onConnect();

    void updateCombat();
    void updateBullets();

   private:
    void handleKeyboardInput(sf::Keyboard::Key& key, bool status);
    void handleTextInput(sf::Uint32 code);

    sf::View m_view;
    std::unique_ptr<Network> m_network;
    sf::IpAddress m_ipAddress;
    unsigned short m_port;

    Player m_player;
    std::vector<Enemy> m_enemies;
    std::vector<Bullet> m_bullets;
    GUI m_gui;

    sf::Vector2f m_oldVelocity;
    sf::Time m_tempTime;
    sf::Clock m_clock;

    TabOverlay m_tabOverlay;
    bool m_firstTimeConnected;

    //  FUTURE: lvl manager
    Level m_current_level;

    bool oldShootStatus;    
    std::vector<sf::RectangleShape> m_collisionRects;
};
