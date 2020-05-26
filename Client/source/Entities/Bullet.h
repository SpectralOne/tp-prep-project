#pragma once

#include <SFML/Graphics.hpp>

#include "../Managers/AssetManager.h"

enum BDIRECTIONS {
    BUP,
    BDOWN,
    BLEFT,
    BRIGHT
};

class Bullet {
   public:
    Bullet(int id, float pos_x, float pos_y, int direction);
    const sf::FloatRect getBounds() const;

    void update(float dt);
    void render(sf::RenderWindow& window);
	sf::Vector2f getPosition();
	sf::Sprite& getSprite();
    sf::Vector2f getDirection();
    void setTargetPosition(sf::Vector2f pos);
    void interpolate(float dt);
    void rotate(float angle);
    void setExpired();
    bool isExpired();
    int getId();

   private:
    sf::Sprite shape;
    sf::Vector2f direction;
    sf::Vector2f target;
    float movementSpeed;
    bool expired;
    int id;
};
