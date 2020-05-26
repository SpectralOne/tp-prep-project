#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class BasePlayer {
   public:
    BasePlayer() {}
    BasePlayer(int id, unsigned short level, const std::string& name);
    BasePlayer(int id, unsigned short level, const std::string& name, unsigned short kills, unsigned short deaths, unsigned short ping);

    int getId();
    unsigned short getLevel();
    std::string getName();
    unsigned short getKills();
    unsigned short getDeaths();
    unsigned short getPing();
    sf::Vector2f getPosition();

    void setId(int id);
    void setLevel(unsigned short level);
    void setName(const std::string& name);
    void setKills(unsigned short kills);
    void setDeaths(unsigned short deaths);
    void setPing(unsigned short ping);
    void setPosition(sf::Vector2f position);

   protected:
    int m_id;
    unsigned short m_level;
    std::string m_name;
    unsigned short m_kills;
    unsigned short m_deaths;
    unsigned short m_ping;

    sf::Texture m_texture;
    sf::Sprite m_sprite;
};
