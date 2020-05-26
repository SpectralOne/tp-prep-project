#pragma once

#include <SFML/Network.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <queue>

#include "../Entities/Bullet.h"
#include "../Entities/Enemy.h"
#include "../Entities/Player.h"
// #include "../GUI/Chat.h"
#include "../GUI/GUI.h"
#include "../GUI/TabOverlay.h"

struct InputData {
    sf::Uint64 m_id;
    float m_dt;
    sf::Vector2f m_position;
    sf::Vector2f m_direction;
};

struct ServerData {
    sf::Uint64 m_id;
    sf::Vector2f m_position;
};

// SYNC WITH SERV ENUM
enum SIGNAL_SEND {
    PING,
    CLIENT_NAME,
    CLIENT_VELOCITY,
    CLIENT_DISCONNECT,
    PLAYER_LIST,
    TEXT_MESSAGE,
    BULLETS_REC
};

enum SIGNAL_RECEIVE {
    PING_REC = 0,
    SERVER_FULL = 51,
    CLIENT_ID,
    PLAYER_LIST_REC,
    PLAYER_CONNECTED,
    PLAYER_DISCONNECTED,
    CLIENT_POSITION,
    ENEMY_POSITION,
    TEXT_MESSAGE_REC,
    BULLETS_POSITION,
    DIED,
    KILLED,
    WON,
    GAME_WAIT,
    GAME_CD,
    GAME_START
};

class Network {
   public:
    Network(sf::IpAddress ip, unsigned short port);
    void setPlayer(Player* player);
    void setEnemies(std::vector<Enemy>* enemies);
    void setBullets(std::vector<Bullet>* bullets);
    void setTabOverlay(TabOverlay* overlay);
    void setChat(Chat* chat);
    void setGM(GUI* g) { gm = g; }

    std::string getMessage() { return m_textMessage; }

    void sendPing();
    void sendName();
    void sendDirection();
    void sendBullets();
    void sendDisconnect();
    void sendPlayerListRequest();
    void sendMessage(const std::string& text);

    void receive();
    void process();
    void setInputSeq(float dt);
    bool isConnected() { return m_connected; }

   private:
    sf::TcpSocket m_socket;
    bool m_connected;

    std::vector<InputData> m_input_sequence_list;
    ServerData m_server_real_pos;
    sf::Uint64 m_current_input_id;

    sf::Int64 m_ping_ms;
    sf::Int64 m_client_time_sent;
    sf::Int64 m_offset;

    sf::Clock m_clock;
    Player* m_player;
    GUI *gm;
    std::vector<Enemy>* m_enemies;
    std::vector<Bullet>* m_bullets;
    TabOverlay* m_tabOVerlay;
    Chat* m_chat;

    std::string m_textMessage;
};
