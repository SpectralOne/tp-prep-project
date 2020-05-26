#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <queue>
#include <string>

enum DIRECTIONS {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player {
   public:
    Player(std::unique_ptr<sf::TcpSocket>* socket, sf::Vector2f position, int id);

    void setPosition(sf::Vector2f position);
    void setVelocity(sf::Vector2f velocity);
    void setName(const std::string& name);
    void setTimeout(sf::Time time);
    void setConnected(bool status);
    void setPing(unsigned short ping);

    sf::Vector2f getPosition();
    sf::Vector2f getVelocity();
    sf::TcpSocket* getSocket();
    sf::Time getTimeout();
    std::string getName();
    bool isConnected();
    unsigned short getPing();
    int getId();
    void getDamage();
    bool isAlive() { return alive; }
    int getKills() { return kills; }
    void setKills(int val) { kills += val; }

    void update(float dt);

    void setLastInputProcessed(sf::Uint64 last_input_proc);
    sf::Uint64 getLastInputProcessed();

    void addDirection(sf::Vector2f dir);
    std::queue<sf::Vector2f>* getListOfDirection();

   private:
    sf::Vector2f m_position;
    sf::Vector2f m_half_size;
    sf::Vector2f m_velocity;
    sf::Time m_timeout;
    std::string m_name;
    std::unique_ptr<sf::TcpSocket> m_socket = nullptr;
    bool m_connected;
    int m_id;
    float m_speed;
    sf::Uint64 m_last_input_processed;
    unsigned short m_ping;
    int hp;
    bool alive;
    int kills;

    std::queue<sf::Vector2f> m_list_of_direction;
};
