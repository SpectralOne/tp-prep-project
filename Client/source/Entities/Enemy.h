#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../Managers/Animator.h"
#include "../GUI/Label.h"
#include "BasePlayer.h"

class Enemy : public BasePlayer {
   public:
    Enemy(int id, unsigned short level, std::string name);
    void draw(sf::RenderWindow& window);
    void move(sf::Vector2f vel);
    void update(float dt);

    void setTargetPosition(sf::Vector2f pos);
    void interpolate(float dt);
    sf::Sprite& getSprite();

    Animator m_playerAnimator;

   private:
    float m_currentTime;
    float m_speed;
    sf::Vector2f m_target_position;
    sf::Vector2f m_velocity;
    Label m_name;
};
