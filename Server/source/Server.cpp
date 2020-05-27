#include "Server.h"

Server::Server(unsigned short port) {
    m_running = true;

    sf::IpAddress ip = sf::IpAddress::getLocalAddress();

    std::cout << ip << std::endl;

    if (m_listener.listen(port) == sf::Socket::Done)
        std::cout << "Server is started on port: " << port << ". Waiting for clients.\n";
    else
        std::cout << "Error - Failed to bind the port " << port << std::endl;

    m_selector.add(m_listener);

    m_maxPlayers = 5;

    m_playersConnected = 0;
    m_currentPlayerId = 0;
    curBulletId = 0;
    curHealthkitId = 0;
    curTeleportId = 0;

    t0 = std::thread(&Server::receive, this);
    t0.detach();
    curBuletsCount = 0;
    curHealthkitsCount = 0;
    curTeleportsCount = 0;
    game = false;
    secSince = 0.f;
    game_end = false;
}

Server::~Server() {
    m_running = false;
}

void Server::setPlayerList(std::vector<Player>* players) {
    m_playerList = players;
}

void Server::receive() {
    while (m_running) {
        if (m_selector.wait()) {
            if (m_selector.isReady(m_listener)) {
                std::unique_ptr<sf::TcpSocket> tempSocket = std::make_unique<sf::TcpSocket>();
                if (m_listener.accept(*tempSocket) == sf::Socket::Done) {
                    if (m_playersConnected < m_maxPlayers) {
                        std::lock_guard<std::mutex> guard(m_mutex);
                        m_playerList->emplace_back(Player(&tempSocket, sf::Vector2f(200, 200), m_currentPlayerId));
                        m_selector.add(*m_playerList->back().getSocket());
                        ++m_playersConnected;

                        sf::Packet outPacket;
                        outPacket << SIGNAL_SEND::CLIENT_ID;
                        outPacket << m_currentPlayerId;

                        //Send client id
                        if (m_playerList->back().getSocket()->send(outPacket) != sf::Socket::Done) {
                            std::cout << "Error sending player index" << std::endl;
                        }

                        ++m_currentPlayerId;
                    } else {
                        sf::Packet outPacket;
                        outPacket << SIGNAL_SEND::SERVER_FULL;

                        if (tempSocket->send(outPacket) != sf::Socket::Done)
                            std::cout << "Error sending server is full message!" << std::endl;

                        std::cout << "User tried to connect but the server was full!" << std::endl;
                    }
                }
            } else {
                std::lock_guard<std::mutex> guard2(m_mutex);
                for (auto& player : *m_playerList) {
                    if (m_selector.isReady(*player.getSocket())) {
                        sf::Packet received;
                        if (player.getSocket()->receive(received) == sf::Socket::Done) {
                            m_receivedPackets.push(received);
                        }
                    }
                }
            }
        }
    }
}

void Server::update() {
    if (m_receivedPackets.size() > 0) {
        std::lock_guard<std::mutex> guard(m_mutex);
        sf::Packet received = m_receivedPackets.front();

        int signal, id;
        received >> signal;
        received >> id;

        Player* currentPlayer = nullptr;
        for (auto& itr : *m_playerList) {
            if (itr.getId() == id) {
                currentPlayer = &itr;
            }
        }

        switch (signal) {
            case SIGNAL_RECEIVE::PING_REC: {
                unsigned short ping;
                received >> ping;
                currentPlayer->setPing(ping);

                auto chrono_clock_now = std::chrono::system_clock::now().time_since_epoch();
                std::chrono::milliseconds chrono_time_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(chrono_clock_now);
                sf::Int64 time_now = chrono_time_milliseconds.count();

                sf::Packet p;
                p << SIGNAL_SEND::PING;
                p << time_now;
                if (currentPlayer->getSocket()->send(p) != sf::Socket::Done)
                    std::cout << "Error - Failed to send pong to client" << std::endl;
                break;
            }

            case SIGNAL_RECEIVE::CLIENT_NAME: {
                std::string nameHolder;
                received >> nameHolder;
                currentPlayer->setName(nameHolder);
                std::cout << "New client connected - "
                          << " ID: " << id << " Name: " << nameHolder << std::endl;
                break;
            }

            case SIGNAL_RECEIVE::CLIENT_VELOCITY: {
                // Save the velocity
                sf::Vector2f vel;
                sf::Uint64 input_id;

                received >> input_id;
                received >> vel.x >> vel.y;

                currentPlayer->setLastInputProcessed(input_id);
                currentPlayer->setVelocity(vel);

                break;
            }

            case SIGNAL_RECEIVE::CLIENT_DISCONNECTED: {
                // Tell other users one client disconnected
                for (auto& player : *m_playerList) {
                    sf::Packet out;
                    out << SIGNAL_SEND::PLAYER_DISCONNECTED;
                    out << id;
                    if (id == player.getId())
                        continue;
                    if (player.getSocket()->send(out) != sf::Socket::Done) {
                        std::cout << "Error - Failed to send a disconnected packet" << std::endl;
                    }
                }

                for (auto& itr : *m_playerList) {
                    if (itr.getId() == id) {
                        std::cout << "Client disconnected! - "
                                  << "ID: " << itr.getId() << " Name: " << itr.getName() << std::endl;
                    }
                }

                for (unsigned int i = 0; i < m_playerList->size(); ++i) {
                    if (m_playerList->at(i).getId() == id) {
                        m_playerList->at(i).setConnected(false);
                    }
                }

                --m_playersConnected;
                std::cout << "Number of players connected: " << m_playersConnected << std::endl;
                break;
            }

            case SIGNAL_RECEIVE::PLAYER_LIST_REQUEST: {
                sf::Packet namePacket;
                namePacket << SIGNAL_SEND::PLAYER_LIST;
                namePacket << m_playersConnected;

                for (auto& itr : *m_playerList) {
                    namePacket << itr.getId();
                    namePacket << itr.getName();
                    namePacket << itr.getPosition().x;
                    namePacket << itr.getPosition().y;
                }

                for (auto& player : *m_playerList) {
                    if (player.getSocket()->send(namePacket) != sf::Socket::Done) {
                        std::cout << "Error - Failed to send a name list packet" << std::endl;
                    }
                }
                break;
            }

            case SIGNAL_RECEIVE::TEXT_MESSAGE_REC: {
                std::string message;
                received >> message;

                sf::Packet packet_out;
                packet_out << SIGNAL_SEND::TEXT_MESSAGE << id << message;

                for (auto& player : *m_playerList) {
                    if (id == player.getId())
                        continue;
                    if (player.getSocket()->send(packet_out) != sf::Socket::Done) {
                        std::cout << "Error - Failed to send a text message packet" << std::endl;
                    }
                }
                break;
            }

            case SIGNAL_RECEIVE::BULLET_REC: {
                sf::Vector2f pos;
                sf::Vector2f dir;

                received >> pos.x;
                received >> pos.y;

                int code;
                received >> code;

                float dirx, diry;

                if (code == DIRECTIONS::UP) {  //up
                    dirx = 0;
                    diry = -1;
                }
                if (code == DIRECTIONS::LEFT) {  //left
                    dirx = -1;
                    diry = 0;
                }
                if (code == DIRECTIONS::RIGHT) {  //right
                    dirx = 1;
                    diry = 0;
                }
                if (code == DIRECTIONS::DOWN) {  //down
                    dirx = 0;
                    diry = 1;
                }

                dir.x = dirx;
                dir.y = diry;
                m_bullets.push_back({pos, dir, code, 20.f, id, curBulletId, false});

                updateBullets();
                ++curBuletsCount;
                ++curBulletId;
                sf::Packet p;
                p << SIGNAL_SEND::BULLET_POSITION;
                p << int(curBuletsCount);
                // std::cout << "curCnt : " << curBuletsCount << " == act " << m_bullets.size() << std::endl;
                for (auto& bullet : m_bullets) {
                    p << bullet.bid;
                    p << bullet.pos.x;
                    p << bullet.pos.y;
                    p << bullet.code;
                    p << bullet.expired;
                }
                sendAll(p);
                break;
            }
            default:
                break;
        }

        for (unsigned int i = 0; i < m_playerList->size(); ++i) {
            if (!m_playerList->at(i).isConnected()) {
                m_selector.remove(*m_playerList->at(i).getSocket());
                m_playerList->erase(m_playerList->begin() + i);
            }
        }

        m_receivedPackets.pop();
    }
}

void Server::updateTick() {
    std::lock_guard<std::mutex> guard(m_mutex);
    for (auto& player : *m_playerList) {
        sf::Packet p;
        p << SIGNAL_SEND::CLIENT_POSITION;
        p << player.getLastInputProcessed();
        p << player.getPosition().x;
        p << player.getPosition().y;

        if (player.getSocket()->send(p) != sf::Socket::Done) {
            std::cout << "error sending position" << std::endl;
            player.setConnected(false);
            std::cout << player.getName() << " player time-out\n";
            --m_playersConnected;
            std::cout << "Number of players connected: " << m_playersConnected << std::endl;
        }
    }

    for (unsigned int i = 0; i < m_playerList->size(); ++i) {
        if (!m_playerList->at(i).isConnected()) {
            m_selector.remove(*m_playerList->at(i).getSocket());
            m_playerList->erase(m_playerList->begin() + i);
        }
    }

    if (m_playerList->size() > 1) {
        for (auto& player : *m_playerList) {
            sf::Packet p;
            p << SIGNAL_SEND::ENEMY_POSITION;
            p << int(m_playerList->size() - 1);

            for (auto& enemy : *m_playerList) {
                int id = enemy.getId();

                if (id != player.getId()) {
                    p << id;
                    p << enemy.getPing();
                    if (!enemy.isAlive()) {
                        p << 0;
                        p << 0;
                    } else {
                        p << enemy.getPosition().x;
                        p << enemy.getPosition().y;
                    }
                    p << enemy.isAlive();
                    p << enemy.getKills();
                }
            }

            player.getSocket()->send(p);
        }
    }

    if (game) {
        updateBullets();
        updateHealthkits();
        updateTeleports();
        updateDeadPlayers();

        if (m_playerList->size() > 0 && curBuletsCount) {
            sf::Packet p;
            p << SIGNAL_SEND::BULLET_POSITION;
            p << int(curBuletsCount);
            // std::cout << "curCnt : " << curBuletsCount << " == act " << m_bullets.size() << std::endl;
            for (auto& bullet : m_bullets) {
                p << bullet.bid;
                p << bullet.pos.x;
                p << bullet.pos.y;
                p << bullet.code;
                p << bullet.expired;
            }
            sendAll(p);
        }

        if (m_playerList->size() > 0 && curTeleportsCount) {
            sf::Packet p;
            p << SIGNAL_SEND::TELEPORTS;
            p << int(curTeleportsCount);
            for (auto& t : m_teleports) {
                p << t.id;
                p << t.pos.x;
                p << t.pos.y;
                p << t.expired;
            }
            sendAll(p);
        }

        if (m_playerList->size() > 0 && curHealthkitsCount) {
            sf::Packet p;
            p << SIGNAL_SEND::HEALTHKITS;
            p << int(curHealthkitsCount);
            for (auto& hk : m_healthkits) {
                p << hk.id;
                p << hk.pos.x;
                p << hk.pos.y;
                p << hk.expired;
            }
            sendAll(p);
        }

        removeExpired();

        if (m_playerList->size() > 1) {
            int alive = 0;
            int id;
            for (auto& player : *m_playerList) {
                if (player.isAlive()) {
                    ++alive;
                    id = player.getId();
                }
                if (alive > 1) {
                    break;
                }
            }
            if (alive == 1) {
                for (auto& player : *m_playerList) {
                    if (player.getId() == id) {
                        sf::Packet p;
                        p << SIGNAL_SEND::WON;
                        player.getSocket()->send(p);
                    }
                }
            }
        }
    }
    updateGame();
}

void Server::updateGame() {  // std::format???
    elapsed = servClock.restart();
    if (m_playerList->size() > 0 && !game_end) {
        if (m_playerList->size() < 2 && !game) {
            std::ostringstream stringStream;
            stringStream << "Waiting for players! " << std::to_string(m_playerList->size()) << "/ 2" << std::endl;
            std::string msg = stringStream.str();

            sf::Packet p;
            p << SIGNAL_SEND::GAME_WAIT;
            p << msg;
            sendAll(p);
        }

        if (m_playerList->size() > 1 && !game) {
            std::string msg;

            secSince += elapsed.asSeconds();

            if (secSince > 1.0f) {
                msg = "Starting game ... 3";
            }
            if (secSince > 2.0f) {
                msg = "Starting game ... 2";
            }
            if (secSince > 3.0f) {
                msg = "Starting game ... 1";
            }
            if (secSince > 4.0f) {
                msg = "Kill them all!";
            }

            if (secSince > 4.5f) {
                msg = "";
                game = true;
            }

            sf::Packet p;
            p << SIGNAL_SEND::GAME_CD;
            p << msg;
            sendAll(p);
        }

        if (game) {
            sf::Packet p;
            p << SIGNAL_SEND::GAME_START;
            sendAll(p);
            game_end = true;
            shufflePlayers();
            setHealthkits();
            setTeleports();
        }
    }
}

void Server::shufflePlayers() {
    sf::Vector2f pos = getRandom();
    for (auto& player : *m_playerList) {
        player.setPosition(pos);
    }
}

void Server::setHealthkits() {
    for (int i = 0; i < m_playerList->size() * 2; ++i) {
        sf::Vector2f pos = getRandom();

        if (!intersectsCollision(pos.x, pos.y)) {
            m_healthkits.push_back({pos, curHealthkitId, false});
            ++curHealthkitId;
            ++curHealthkitsCount;
        } else {
            --i;
        }
    }
}

void Server::setTeleports() {
    for (int i = 0; i < m_playerList->size() * 2; ++i) {
        sf::Vector2f pos = getRandom();

        if (!intersectsCollision(pos.x, pos.y)) {
            m_teleports.push_back({pos, curTeleportId, false});
            ++curTeleportId;
            ++curTeleportsCount;
        } else {
            --i;
        }
    }
}

sf::Vector2f Server::getRandom() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distX(100, 1500);
    std::uniform_int_distribution<std::mt19937::result_type> distY(100, 1000);

    return {distX(rng), distY(rng)};
}
void Server::updateBullets() {
    if (m_playerList->size() > 0 && curBuletsCount) {
        unsigned int counter = 0;
        sf::FloatRect intersection;
        for (auto& bullet : m_bullets) {
            bullet.pos += bullet.dir * bullet.speed;
            bool deleted = false;
            // for (auto& player : *m_playerList) {
            for (const auto& tile : m_currentLevel.getUnwalkable()) {
                if (sf::FloatRect(bullet.pos, {64, 64}).intersects(sf::FloatRect(tile, {64, 64}), intersection) ||
                    sf::FloatRect(bullet.pos, {64, 64}).intersects(sf::FloatRect({0, 0}, {64, 64}), intersection)) {
                    m_bullets[counter].expired = true;
                    deleted = true;
                    break;
                }
            }
            if (!deleted) {
                for (auto& player : *m_playerList) {  // segfault
                    if (sf::FloatRect(player.getPosition(), {64, 64}).intersects(sf::FloatRect(bullet.pos, {64, 64}), intersection) &&
                        player.getId() != bullet.id && player.isAlive()) {
                        player.getDamage();
                        m_bullets[counter].expired = true;
                        if (!player.isAlive()) {
                            sf::Packet pac;
                            pac << SIGNAL_SEND::KILLED;

                            for (auto& p : *m_playerList) {
                                if (p.getId() == bullet.id) {
                                    p.setKills(1);
                                    p.getSocket()->send(pac);
                                }
                            }
                        }
                        break;
                    }
                }
            }
            ++counter;
        }
    }
}

void Server::updateHealthkits() {
    if (m_playerList->size() > 0 && m_healthkits.size() > 0) {
        sf::FloatRect intersection;
        unsigned int counter = 0;
        for (auto& hk : m_healthkits) {
            for (auto& player : *m_playerList) {  // segfault
                if (sf::FloatRect(player.getPosition(), {64, 64}).intersects(sf::FloatRect(hk.pos, {64, 64}), intersection) && player.isAlive()) {
                    player.getHeal();
                    m_healthkits[counter].expired = true;

                    sf::Packet p;
                    p << SIGNAL_SEND::HEALED;
                    player.getSocket()->send(p);
                }
            }
            ++counter;
        }
    }
}

void Server::updateTeleports() {
    if (m_playerList->size() > 0 && m_teleports.size() > 0) {
        sf::FloatRect intersection;
        unsigned int counter = 0;
        for (auto& t : m_teleports) {
            for (auto& player : *m_playerList) {  // segfault
                if (sf::FloatRect(player.getPosition(), {64, 64}).intersects(sf::FloatRect(t.pos, {64, 64}), intersection) && player.isAlive()) {
                    sf::Vector2f pos = getRandom();
                    player.setPosition(pos);
                    m_teleports[counter].expired = true;

                    sf::Packet p;
                    p << SIGNAL_SEND::TELEPORTED;
                    player.getSocket()->send(p);
                }
            }
            ++counter;
        }
    }
}

void Server::removeExpired() {
    std::vector<bullet_t> n;
    for (auto& b : m_bullets) {
        if (!b.expired) {
            n.push_back(b);
        } else {
            --curBuletsCount;
        }
    }
    m_bullets.clear();
    m_bullets = n;

    std::vector<healthkit_t> h;
    for (auto& hk : m_healthkits) {
        if (!hk.expired) {
            h.push_back(hk);
        } else {
            --curHealthkitsCount;
        }
    }
    m_healthkits.clear();
    m_healthkits = h;

    std::vector<teleport_t> t;
    for (auto& tp : m_teleports) {
        if (!tp.expired) {
            t.push_back(tp);
        } else {
            --curTeleportsCount;
        }
    }
    m_teleports.clear();
    m_teleports = t;
}

void Server::updateDeadPlayers() {
    for (auto& player : *m_playerList) {
        if (!player.isAlive()) {
            sf::Packet p;
            p << SIGNAL_SEND::DIED;
            player.getSocket()->send(p);
        }
    }
}

void Server::updateWorld(sf::Time dt) {
    std::lock_guard<std::mutex> guard(m_mutex);

    for (auto& player : *m_playerList) {
        player.update(dt.asMilliseconds());

        for (const auto& tile : m_currentLevel.getUnwalkable()) {
            sf::FloatRect intersection;
            if (sf::FloatRect(player.getPosition(), {64, 64}).intersects(sf::FloatRect(tile, {64, 64}), intersection)) {
                sf::Vector2f collision_normal = player.getPosition() - tile;

                sf::Vector3f mainfold;
                if (intersection.width < intersection.height && abs(intersection.width - intersection.height) > 1) {
                    mainfold.x = (collision_normal.x < 0) ? -1.0f : 1.0f;
                    mainfold.z = intersection.width;
                } else if (intersection.height < intersection.width && abs(intersection.width - intersection.height) > 1) {
                    mainfold.y = (collision_normal.y < 0) ? -1.0f : 1.0f;
                    mainfold.z = intersection.height;
                }
                player.setPosition({player.getPosition().x + mainfold.x * mainfold.z, player.getPosition().y + mainfold.y * mainfold.z});
            }
        }
    }
}

void Server::sendAll(sf::Packet p) {
    for (auto& player : *m_playerList) {
        player.getSocket()->send(p);
    }
}

bool Server::intersectsCollision(float x, float y) {
    sf::FloatRect intersection;
    for (const auto& tile : m_currentLevel.getUnwalkable()) {
        if (sf::FloatRect({x, y}, {64, 64}).intersects(sf::FloatRect(tile, {64, 64}), intersection)) {
            return true;
        }
    }
    return false;
}
