#include "Network.h"

Network::Network(sf::IpAddress ip, unsigned short port) {
    m_player = nullptr;
    m_enemies = nullptr;
    m_connected = false;
    m_client_time_sent = 0;
    m_current_input_id = 0;

    if (m_socket.connect(ip, port, sf::seconds(15)) != sf::Socket::Done) {
        std::cout << "Error connecting to server" << std::endl;
    } else {
        m_socket.setBlocking(false);
        m_connected = true;
        std::cout << "Connected" << std::endl;
    }
}

void Network::setPlayer(Player* player) {
    m_player = player;
}

void Network::setEnemies(std::vector<Enemy>* enemies) {
    m_enemies = enemies;
}

void Network::setBullets(std::vector<Bullet>* bullets) {
    this->m_bullets = bullets;
}

void Network::setTabOverlay(TabOverlay* overlay) {
    m_tabOVerlay = overlay;
}

void Network::setChat(Chat* chat) {
    m_chat = chat;
}

void Network::sendPing() {
    auto chrono_clock_now = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::milliseconds chrono_time_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(chrono_clock_now);
    m_client_time_sent = chrono_time_milliseconds.count();

    sf::Packet p;
    p << SIGNAL_SEND::PING;
    p << m_player->getId();
    p << m_player->getPing();
    if (m_socket.send(p) != sf::Socket::Done)
        std::cout << "Failed to send ping" << std::endl;
}

void Network::sendName() {
    sf::Packet temp;
    temp << SIGNAL_SEND::CLIENT_NAME;
    temp << m_player->getId();
    temp << m_player->getName();

    if (m_socket.send(temp) != sf::Socket::Done)
        std::cout << "Error sending my name" << std::endl;
}

void Network::sendBullets() {
    sf::Packet temp;
    temp << SIGNAL_SEND::BULLETS_REC;
    temp << m_player->getId();
    temp << m_player->getPosition().x;
    temp << m_player->getPosition().y;
    temp << m_player->getDirection();
    if (m_socket.send(temp) != sf::Socket::Done)
        std::cout << "Error sending data to server" << std::endl;
}

void Network::sendDirection() {
    if (!m_player->isAlive()) {
        return;
    }
    sf::Packet temp;
    temp << SIGNAL_SEND::CLIENT_VELOCITY;
    temp << m_player->getId();
    temp << m_current_input_id++;
    temp << m_player->getVelocity().x;
    temp << m_player->getVelocity().y;

    if (m_socket.send(temp) != sf::Socket::Done)
        std::cout << "Error sending data to server" << std::endl;
}

void Network::sendDisconnect() {
    sf::Packet temp;
    temp << SIGNAL_SEND::CLIENT_DISCONNECT;
    temp << m_player->getId();

    if (m_socket.send(temp) != sf::Socket::Done)
        std::cout << "Error sending disconnect command to server" << std::endl;
    else
        m_socket.disconnect();
    std::cout << "Disconnected" << std::endl;
}

void Network::sendPlayerListRequest() {
    sf::Packet temp;
    temp << SIGNAL_SEND::PLAYER_LIST;
    temp << m_player->getId();

    if (m_socket.send(temp) != sf::Socket::Done)
        std::cout << "Error sending getPlayerList to server" << std::endl;
}

void Network::sendMessage(const std::string& text) {
    sf::Packet temp;
    temp << SIGNAL_SEND::TEXT_MESSAGE;
    temp << m_player->getId();
    temp << text;

    if (m_socket.send(temp) != sf::Socket::Done)
        std::cout << "Error sending text message to server" << std::endl;
}

void Network::receive() {
    sf::Packet received_packet;
    int signal;
    if (m_socket.receive(received_packet) == sf::Socket::Done) {
        received_packet >> signal;

        switch (signal) {
            case SIGNAL_RECEIVE::PING_REC: {
                sf::Int64 server_time;
                received_packet >> server_time;

                auto chrono_clock_now = std::chrono::system_clock::now().time_since_epoch();
                std::chrono::milliseconds chrono_time_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(chrono_clock_now);
                sf::Int64 time_now = chrono_time_milliseconds.count();

                m_ping_ms = time_now - m_client_time_sent;
                m_offset = server_time - m_client_time_sent;
                m_player->setPing(m_ping_ms);
            } break;
            case SIGNAL_RECEIVE::SERVER_FULL: {
                std::cout << "Server is full" << std::endl;
            } break;
            case SIGNAL_RECEIVE::CLIENT_ID: {
                int id;
                received_packet >> id;
                m_player->setId(id);

                sendName();
                sendPlayerListRequest();
            } break;
            case SIGNAL_RECEIVE::PLAYER_LIST_REC: {
                int enemy_count;
                received_packet >> enemy_count;

                for (int i = 0; i < enemy_count; ++i) {
                    int enemy_id;
                    std::string enemy_name;
                    sf::Vector2f enemy_position;
                    received_packet >> enemy_id;
                    received_packet >> enemy_name;
                    received_packet >> enemy_position.x;
                    received_packet >> enemy_position.y;

                    bool createEnemy = true;
                    for (auto& enemy : *m_enemies) {
                        if (enemy.getId() == enemy_id)
                            createEnemy = false;
                    }

                    if (enemy_id != m_player->getId() && createEnemy) {
                        std::cout << "New user connected with id " << enemy_id << std::endl;
                        m_enemies->push_back(Enemy(enemy_id, 1, enemy_name));
                        m_enemies->back().setPosition(enemy_position);
                        m_tabOVerlay->addPlayer(m_enemies->back());
                    }
                }

                std::cout << "Number of players connected: " << m_enemies->size() << std::endl;
            } break;
            case SIGNAL_RECEIVE::PLAYER_CONNECTED: {
            } break;
            case SIGNAL_RECEIVE::PLAYER_DISCONNECTED: {
                int id;
                received_packet >> id;

                std::cout << "Delete this player: " << id << std::endl;
                // Remove the player form the player list
                for (unsigned int i = 0; i < m_enemies->size(); ++i) {
                    if (m_enemies->at(i).getId() == id) {
                        m_enemies->erase(m_enemies->begin() + i);
                        m_tabOVerlay->removePlayer(id);
                    }
                }

            } break;
            case SIGNAL_RECEIVE::CLIENT_POSITION: {
                if (m_player->isAlive()) {
                    sf::Uint64 input_id;
                    sf::Vector2f pos;

                    received_packet >> input_id;
                    received_packet >> pos.x;
                    received_packet >> pos.y;

                    m_server_real_pos.m_id = input_id;
                    m_server_real_pos.m_position = pos;

                    process();
                }
            } break;
            case SIGNAL_RECEIVE::ENEMY_POSITION: {
                int num;
                received_packet >> num;

                for (int i = 0; i < num; ++i) {
                    int e_id;
                    sf::Vector2f tmp;
                    unsigned short ping;

                    received_packet >> e_id;
                    received_packet >> ping;
                    received_packet >> tmp.x;
                    received_packet >> tmp.y;
                    bool status;
                    received_packet >> status;
                    int kills;
                    received_packet >> kills;
                    for (auto& enm : *m_enemies) {
                        if (enm.getId() == e_id) {
                            enm.setTargetPosition(tmp);
                            enm.setPing(ping);
                            if (status == false) {
                                enm.setDeaths(1);
                            }
                            enm.setKills(kills);
                        }
                    }
                }
            } break;
            case SIGNAL_RECEIVE::TEXT_MESSAGE_REC: {
                int id;
                received_packet >> id;
                received_packet >> m_textMessage;
                for (auto player : *m_enemies) {
                    if (player.getId() == id)
                        m_chat->addMessage("<" + player.getName() + ">: " + m_textMessage);
                }
            } break;
            case SIGNAL_RECEIVE::BULLETS_POSITION: {
                int num;
                received_packet >> num;
                for (int i = 0; i < num; ++i) {
                    int id;
                    int code;
                    sf::Vector2f pos;
                    bool expired;

                    received_packet >> id;
                    received_packet >> pos.x >> pos.y;
                    received_packet >> code;
                    received_packet >> expired;

                    bool create_bullet = true;
                    for (auto& b : *m_bullets) {
                        if (b.getId() == id) {
                            b.setTargetPosition(pos);
                            if (expired) {
                                b.setExpired();
                            }
                            create_bullet = false;
                        }
                    }
                    if (create_bullet) {
                        m_bullets->push_back(Bullet(id, pos.x, pos.y, code));
                        m_player->shootSound();
                    }
                }
            } break;
            case SIGNAL_RECEIVE::DIED: {
                m_player->setDeaths(1);
                m_player->setShoot(false);
                m_player->setAlive(false);
            } break;
            case SIGNAL_RECEIVE::KILLED: {
                m_player->setKills(m_player->getKills() + 1);
            } break;
            case SIGNAL_RECEIVE::WON: {
                m_player->setWon();
            } break;
            case SIGNAL_RECEIVE::GAME_WAIT: {
                std::string msg;
                received_packet >> msg;
                gm->setText(msg);
            } break;
            case SIGNAL_RECEIVE::GAME_CD: {
                std::string msg;
                received_packet >> msg;
                gm->setText(msg);
                // display msg
            } break;
            case SIGNAL_RECEIVE::GAME_START: { // random teleport
                m_player->setShoot(true);
                gm->setText("Kill them all");
            } break;
            default:
                std::cout << "Received unknown signal from server: " << signal << std::endl;
                break;
        }
    }
}

void Network::process() {
    int i = 0;
    while (i < m_input_sequence_list.size()) {
        if (m_input_sequence_list.at(i).m_id <= m_server_real_pos.m_id) {
            m_input_sequence_list.erase(m_input_sequence_list.begin());
        } else {
            m_player->setPosition(m_server_real_pos.m_position);
            ++i;
        }
    }
}

void Network::setInputSeq(float dt) {
    InputData x;
    x.m_id = m_current_input_id;
    x.m_dt = dt;
    x.m_position = m_player->getPosition();
    x.m_direction = m_player->getVelocity();
    m_input_sequence_list.push_back(x);
}
