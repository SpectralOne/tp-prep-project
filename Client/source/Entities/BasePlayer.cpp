#include "BasePlayer.h"

BasePlayer::BasePlayer(int id,
                       unsigned short level,
                       const std::string& name) : m_id(id),
                                                  m_level(level),
                                                  m_name(name),
                                                  m_kills(0),
                                                  m_deaths(0),
                                                  m_ping(0) {}

BasePlayer::BasePlayer(int id, unsigned short level,
                       const std::string& name,
                       unsigned short kills,
                       unsigned short deaths,
                       unsigned short ping) : m_id(id),
                                              m_level(level),
                                              m_name(name),
                                              m_kills(kills),
                                              m_deaths(deaths),
                                              m_ping(ping) {}

int BasePlayer::getId() {
    return m_id;
}
unsigned short BasePlayer::getLevel() {
    return m_level;
}
std::string BasePlayer::getName() {
    return m_name;
}
unsigned short BasePlayer::getKills() {
    return m_kills;
}
unsigned short BasePlayer::getDeaths() {
    return m_deaths;
}
unsigned short BasePlayer::getPing() {
    return m_ping;
}
sf::Vector2f BasePlayer::getPosition() {
    return m_sprite.getPosition();
}

void BasePlayer::setId(int id) {
    m_id = id;
}
void BasePlayer::setLevel(unsigned short level) {
    m_level = level;
}
void BasePlayer::setName(const std::string& name) {
    m_name = name;
}
void BasePlayer::setKills(unsigned short kills) {
    m_kills = kills;
}
void BasePlayer::setDeaths(unsigned short deaths) {
    m_deaths = deaths;
}
void BasePlayer::setPing(unsigned short ping) {
    m_ping = ping;
}
void BasePlayer::setPosition(sf::Vector2f position) {
    m_sprite.setPosition(position);
}