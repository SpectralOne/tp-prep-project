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
#include <thread>
#include <sstream>
#include <random>
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
    GAME_START
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

typedef struct
{
    sf::Vector2f pos;
    sf::Vector2f dir;
    int code;
    float speed;
    int id;
    int bid;
    bool expired;
} bullet_t;

class Server {
   public:
    Server(unsigned short port);
    ~Server();

    void receive();
    void update();

    void sendAll(sf::Packet p);

    void updateTick();
    void updateBullets();
    void removeExpiredBullets();
    void updateDeadPlayers();
    void updateGame();
    void updateWorld(sf::Time dt);

    void shufflePlayers();
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

    std::vector<Player> *m_playerList = nullptr;
    std::vector<bullet_t> m_bullets;
    int curBuletsCount;
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
