#include "StatePlay.h"

StatePlay::StatePlay(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) : State(machine, window, info, replace), m_player(info.pName), m_gui(info.pName), m_oldVelocity(0, 0), m_tabOverlay({0, 0}) {
    m_network = std::make_unique<Network>(this->info.ip, this->info.port);

    this->info.isCon = m_network->isConnected();

    m_view.setSize(1280, 720);
    m_view.setCenter(sf::Vector2f(0, 0));

    m_tempTime = sf::Time::Zero;

    m_player.setPosition({200, 200});
    m_network->setPlayer(&m_player);
    m_network->setEnemies(&m_enemies);
    m_network->setBullets(&m_bullets);
    m_network->setHealthkits(&m_healthkits);
    m_network->setTeleports(&m_teleports);
    m_network->setTabOverlay(&m_tabOverlay);
    m_network->setChat(&m_gui.chat);
    m_network->setGM(&m_gui);

    m_gui.chat.setPlayer(&m_player);

    m_firstTimeConnected = false;
    for (int i = 0; i < 9; ++i) {
        sf::RectangleShape temp_rect;
        temp_rect.setFillColor(sf::Color(50, 50, 50, 50));
        temp_rect.setSize({64, 64});
        m_collisionRects.push_back(temp_rect);
    }
    oldShootStatus = false;
}

StatePlay::~StatePlay() {
    m_network->sendDisconnect();
}

void StatePlay::pause() {
}

void StatePlay::resume() {
}

void StatePlay::update(sf::Time dt) {
    if (!this->info.isCon) {
        m_next = StateManager::build<StatePlayMenu>(m_machine, m_window, this->info, true);
        return;
    }

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
            case sf::Event::TextEntered: {
                handleTextInput(event.text.unicode);
            } break;
            default:
                break;
        }
    }

    if (m_clock.getElapsedTime().asSeconds() > m_tempTime.asSeconds() + 2) {
        m_tempTime = m_clock.getElapsedTime();
        m_network->sendPing();
    }

    m_network->receive();

    // Processing input
    sf::Vector2f velocity(0, 0);
    bool shootStatus = false;
    if (m_window.hasFocus() && m_gui.chat.getStatus() != CHAT_STATUS::OPEN) {
        // Normal movement
        if (sf::Keyboard::isKeyPressed(InputManager::getKeyAction("MOVE_UP"))) {
            velocity.y = -1;
        }
        if (sf::Keyboard::isKeyPressed(InputManager::getKeyAction("MOVE_LEFT"))) {
            velocity.x = -1;
        }
        if (sf::Keyboard::isKeyPressed(InputManager::getKeyAction("MOVE_RIGHT"))) {
            velocity.x = 1;
        }
        if (sf::Keyboard::isKeyPressed(InputManager::getKeyAction("MOVE_DOWN"))) {
            velocity.y = 1;
        }

        if (sf::Keyboard::isKeyPressed(InputManager::getKeyAction("SHOOT"))) {
            if (m_player.canAttack())
                shootStatus = true;
        }

        // In case of diagonal movement
        float diagonalSpeed = 0.707107;  // sqrt(2) / 2.0f

        if (velocity.x > 0 && velocity.y > 0)  // Down-Right
            velocity *= diagonalSpeed;
        else if (velocity.x < 0 && velocity.y > 0)  // Down-Left
            velocity *= diagonalSpeed;
        if (velocity.x > 0 && velocity.y < 0)  // Up-Right
            velocity *= diagonalSpeed;
        else if (velocity.x < 0 && velocity.y < 0)  // Up-Left
            velocity *= diagonalSpeed;
    }

    // If velocity changes send the new velocity to the server
    if (m_oldVelocity != velocity) {
        m_player.setVelocity(velocity);
        m_network->sendDirection();
        m_oldVelocity = velocity;
        m_network->setInputSeq(dt.asMilliseconds());
    }

    if (oldShootStatus != shootStatus) {
        m_network->sendBullets();
        oldShootStatus = shootStatus;
    }

    // Update the player position
    m_player.update(dt.asMilliseconds());
    m_gui.update(dt.asMilliseconds());

    int x = 0;
    // Check only tiles around the player
    for (int i = -1; i < 2; ++i)
        for (int j = -1; j < 2; ++j) {
            sf::Vector2i current_tile = {(int)(m_player.getPosition().x + 32 + i * 64) / 64, (int)(m_player.getPosition().y + 32 + j * 64) / 64};

            sf::Vector2i tile_pos = m_current_level.getMap().tileIndex[current_tile.x * m_current_level.getMap().getSize().y + current_tile.y] * 64;
            bool walkable = m_current_level.getMap().walkableVector[current_tile.x * m_current_level.getMap().getSize().y + current_tile.y];

            m_collisionRects[x].setPosition((sf::Vector2f)tile_pos);

            sf::FloatRect intersection;
            if (m_player.getSprite().getGlobalBounds().intersects(sf::FloatRect((sf::Vector2f)tile_pos, {64, 64}), intersection) && !walkable) {
                // Get the collision normal vector
                sf::Vector2f collision_normal = m_player.getPosition() - (sf::Vector2f)tile_pos;

                // Calculate in which direction the fix is needed
                sf::Vector3f mainfold;
                if (intersection.width < intersection.height && abs(intersection.width - intersection.height) > 1) {
                    mainfold.x = (collision_normal.x < 0) ? -1.0f : 1.0f;
                    mainfold.z = intersection.width;
                } else if (intersection.height < intersection.width && abs(intersection.width - intersection.height) > 1) {
                    mainfold.y = (collision_normal.y < 0) ? -1.0f : 1.0f;
                    mainfold.z = intersection.height;
                }
                // Fix the path
                sf::Vector2f normal(mainfold.x, mainfold.y);
                m_player.getSprite().move(normal * mainfold.z);
            }
            x++;
        }

    // Move the view and gui
    m_view.setCenter(m_player.getPosition().x + 32, m_player.getPosition().y + 32);
    m_gui.setCenter({m_player.getPosition().x + 32, m_player.getPosition().y + 32});

    // Update the stats
    m_tabOverlay.updatePlayer(m_player);
    size_t i = 0;
    for (auto& enemy : m_enemies) {
        m_tabOverlay.updatePlayer(enemy);
        enemy.m_playerAnimator.m_Sprite = &enemy.getSprite();

        enemy.update(dt.asMilliseconds());
        enemy.interpolate(dt.asMilliseconds());
    }

    for (auto& b : m_bullets) {
        b.update(dt.asMilliseconds());
    }

    m_tabOverlay.setPosition(m_player.getPosition());

    updateBullets();
    updateHealthkits();
    updateTeleports();

    m_player.updateAttack();

    if (!m_player.isAlive() && !m_player.getSpectator()) {
        m_player.setSpectator(true);
        m_player.setSpeed(0.7f);
        m_next = StateManager::build<StateDead>(m_machine, m_window, this->info, false);
    }

    if (m_player.getWon()) {
        m_next = StateManager::build<StateWin>(m_machine, m_window, this->info, false);
    }

    if (!m_firstTimeConnected && m_network->isConnected())
        onConnect();
}

void StatePlay::draw() {
    m_window.setView(m_view);
    m_window.clear(sf::Color::White);

    m_current_level.draw(m_window);

    for (auto& enemy : m_enemies)
        enemy.draw(m_window);

    m_player.draw(m_window);

    m_gui.draw(m_window);
    m_tabOverlay.draw(m_window);

    for (auto& bullet : m_bullets) {
        bullet.render(m_window);
    }

    for (auto& teleport : m_teleports) {
        teleport.render(m_window);
    }
    for (auto& healthkit : m_healthkits) {
        healthkit.render(m_window);
    }

    m_window.display();
}

void StatePlay::onConnect() {
    m_firstTimeConnected = true;
    m_tabOverlay.addPlayer(m_player);
}

//  CHECK: if resume not brake the game state
//  after resume -> get last state -> update (save state)
void StatePlay::handleKeyboardInput(sf::Keyboard::Key& key, bool status) {
    if (key == InputManager::getKeyAction("TOGGLEMENU") && status && m_gui.chat.getStatus() != CHAT_STATUS::OPEN) {
        m_next = StateManager::build<StatePlayMenu>(m_machine, m_window, this->info, false);
    }

    if (key == InputManager::getKeyAction("SCORE") && status) {
        m_tabOverlay.setVisible(true);
    }
    if (key == InputManager::getKeyAction("SCORE") && !status) {
        m_tabOverlay.setVisible(false);
    }
    if (key == InputManager::getKeyAction("CHAT") && status) {
        if (m_gui.chat.getStatus() == CHAT_STATUS::CLOSED) {
            m_gui.chat.openChat();
        } else if (m_gui.chat.getStatus() == CHAT_STATUS::OPEN) {
            m_gui.chat.send();
            m_network->sendMessage(m_gui.chat.getMessage());
            m_gui.chat.closeChat();
        } else if (m_gui.chat.getStatus() == CHAT_STATUS::SENT) {
        }
    }
    if (key == sf::Keyboard::BackSpace && status) {
        m_gui.chat.removeLastChar();
    }
    if (key == sf::Keyboard::Escape && status) {
        m_gui.chat.closeChat();
    }
}

void StatePlay::handleTextInput(sf::Uint32 code) {
    // 127 shift-backspace
    if (code < 127 && code > 27) {
        m_gui.chat.addText(code);
    }
}

void StatePlay::updateBullets() {
    unsigned int counter = 0;
    for (auto& b : m_bullets) {
        if (b.isExpired()) {
            m_bullets.erase(m_bullets.begin() + counter);
        }
        ++counter;
    }
}

void StatePlay::updateHealthkits() {
    unsigned int counter = 0;
    for (auto& h : m_healthkits) {
        if (h.isExpired()) {
            m_healthkits.erase(m_healthkits.begin() + counter);
        }
        ++counter;
    }
}

void StatePlay::updateTeleports() {
    unsigned int counter = 0;
    for (auto& t : m_teleports) {
        if (t.isExpired()) {
            m_teleports.erase(m_teleports.begin() + counter);
        }
        ++counter;
    }
}
