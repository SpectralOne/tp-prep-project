#include "Player.h"

#include <iostream>

Player::Player(std::unique_ptr<sf::TcpSocket>* socket, sf::Vector2f position, int id) {
    m_position = position;
    m_half_size = sf::Vector2f(32, 32);
    m_name = "Default Name";
    m_id = id;
    m_speed = 0.3f;
    m_socket = std::move(*socket);
    m_velocity = sf::Vector2f(0, 0);
    m_timeout = sf::seconds(0);
    m_connected = true;
    m_last_input_processed = 0;
    m_ping = 0;
    hp = 100;
    alive = true;
}

void Player::setPosition(sf::Vector2f position) {
    m_position = position;
}

void Player::setVelocity(sf::Vector2f velocity) {
    m_velocity = velocity;
}

void Player::setName(const std::string& name) {
    m_name = name;
}

void Player::setTimeout(sf::Time time) {
    m_timeout = time;
}

void Player::setConnected(bool status) {
    m_connected = status;
}

void Player::setPing(unsigned short ping) {
    m_ping = ping;
}

sf::Vector2f Player::getPosition() {
    return m_position;
}

sf::Vector2f Player::getVelocity() {
    return m_velocity;
}

sf::TcpSocket* Player::getSocket() {
    return m_socket.get();
}

std::string Player::getName() {
    return m_name;
}

int Player::getId() {
    return m_id;
}

void Player::getDamage() {
    hp -= 18;
    hp > 0 ? alive = true : alive = false;
}

sf::Time Player::getTimeout() {
    return m_timeout;
}
bool Player::isConnected() {
    return m_connected;
}

void Player::update(float dt) {
    m_position.x += (m_velocity.x * m_speed * dt);
    m_position.y += (m_velocity.y * m_speed * dt);
}

void Player::addDirection(sf::Vector2f dir) {
    m_list_of_direction.push(dir);
}

std::queue<sf::Vector2f>* Player::getListOfDirection() {
    return &m_list_of_direction;
}

void Player::setLastInputProcessed(sf::Uint64 last_input_proc) {
    m_last_input_processed = last_input_proc;
}

sf::Uint64 Player::getLastInputProcessed() {
    return m_last_input_processed;
}

unsigned short Player::getPing() {
    return m_ping;
}