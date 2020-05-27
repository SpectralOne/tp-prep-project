#pragma once

#include <SFML/Graphics.hpp>

#include "../Managers/AssetManager.h"

class BaseEntity {
   public:
    BaseEntity(int id, float pos_x, float pos_y);
    const sf::FloatRect getBounds() const;

    void render(sf::RenderWindow& window);
	sf::Vector2f getPosition();
	sf::Sprite& getSprite();
    void setTargetPosition(sf::Vector2f pos);
    void rotate(float angle);
    void setExpired();
    bool isExpired();
    int getId();

   protected:
    sf::Sprite shape;
    sf::Vector2f target;
    bool expired;
    int id;
};
