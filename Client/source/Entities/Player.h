#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Managers/Animator.h"
#include "BasePlayer.h"

enum DIRECTIONS {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player : public BasePlayer {
   public:
    Player(std::string name);

    void setShadowPosition(sf::Vector2f pos);
    void setVelocity(sf::Vector2f vel);

    sf::Vector2f getVelocity();
    float getPlayerSpeed();

    sf::Sprite& getSprite();
    void draw(sf::RenderWindow& window);
    void update(float dt);

    void loseHp(const int value);
    bool canAttack();
    void updateAttack();
    int getDirection();
    void setShoot(bool status);
    void setAlive(bool status);
    bool isAlive();
    void setSpectator(bool status);
    bool getSpectator();
    void setWon();
    bool getWon();
    void setSpeed(float speed);

    void shootSound();

   private:
    sf::Sprite m_shadow;

    sf::Vector2f m_velocity;
    float m_speed;

    Animator m_playerAnimator;

    bool moving;
    sf::Sound walkSound;
    sf::Sound shoot;
    bool s;

    bool alive;
    bool allowed_to_shoot;

    int hp;
    float attackCooldown;
    float attackCooldownMax;
    int direction;
    bool spectator;
    bool won;
};
