#pragma once

#include <SFML/Graphics.hpp>

#include "../Managers/AssetManager.h"
#include "BaseEntity.h"

enum BDIRECTIONS {
    BUP,
    BDOWN,
    BLEFT,
    BRIGHT
};

class Bullet : public BaseEntity {
   public:
    Bullet(int id, float pos_x, float pos_y, int direction);

    void update(float dt);
    sf::Vector2f getDirection();
    void interpolate(float dt);

   private:
    sf::Vector2f direction;
    float movementSpeed;
};
