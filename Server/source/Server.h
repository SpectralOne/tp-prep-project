#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Network.hpp>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>
#include <thread>
// #include <format>

#include "LevelManager.h"
#include "Player.h"

enum SIGNAL_SEND {
    PING = 0,
    SERVER_FULL = 51,
    CLIENT_ID,
    PLAYER_LIST,
    PLAYER_CONNECTED,
    PLAYER_DISCONNECTED,
    CLIENT_POSITION,
    ENEMY_POSITION,
    TEXT_MESSAGE,
    BULLET_POSITION,
    DIED,
    KILLED,
    WON,
    GAME_WAIT,
    GAME_CD,
    GAME_START,
    HEALTHKITS,
    TELEPORTS,
    HEALED,
    TELEPORTED
};

enum SIGNAL_RECEIVE {
    PING_REC,
    CLIENT_NAME,
    CLIENT_VELOCITY,
    CLIENT_DISCONNECTED,
    PLAYER_LIST_REQUEST,
    TEXT_MESSAGE_REC,
    BULLET_REC
};

typedef struct {
    sf::Vector2f pos;
    sf::Vector2f dir;
    int code;
    float speed;
    int id;
    int bid;
    bool expired;
} bullet_t;

typedef struct {
    sf::Vector2f pos;
    int id;
    bool expired;
} healthkit_t;

typedef struct {
    sf::Vector2f pos;
    int id;
    bool expired;
} teleport_t;

class Server {
   public:
    Server(unsigned short port);
    ~Server();

    void receive();
    void update();

    void sendAll(sf::Packet p);

    void updateTick();
    void updateBullets();
    void updateHealthkits();
    void updateTeleports();
    void removeExpired();
    void updateDeadPlayers();
    void updateGame();
    void updateWorld(sf::Time dt);

    sf::Vector2f getRandom();
    void shufflePlayers();
    void setHealthkits();
    void setTeleports();

    bool intersectsCollision(float x, float y);
    void setPlayerList(std::vector<Player> *players);

   private:
    bool m_running;

    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;

    unsigned short m_port;
    unsigned short m_maxPlayers;
    bool game;

    int m_playersConnected;
    int m_currentPlayerId;
    int curBulletId;
    int curHealthkitId;
    int curTeleportId;

    std::vector<Player> *m_playerList = nullptr;
    std::vector<bullet_t> m_bullets;
    std::vector<healthkit_t> m_healthkits;
    std::vector<teleport_t> m_teleports;
    int curBuletsCount;
    int curHealthkitsCount;
    int curTeleportsCount;
    std::queue<sf::Packet> m_receivedPackets;

    char m_tmp[1400];

    std::thread t0;
    std::mutex m_mutex;

    sf::Clock servClock;
    sf::Time elapsed;
    float secSince;
    bool game_end;
    // Map
    LevelManager m_currentLevel;
};
