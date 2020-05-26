#include "Bullet.h"

Bullet::Bullet(int id, float pos_x, float pos_y, int direction) {
    this->shape.setTexture(AssetManager::GetTexture("res/img/3310.png"));
    this->shape.setPosition(pos_x, pos_y);
    this->movementSpeed = 20.f;
    this->id = id;
    this->expired = false;
    this->target.x = pos_x;
    this->target.y = pos_y;

    if (direction == BDIRECTIONS::BLEFT) {  //left
        this->shape.rotate(270.f);
    }
    if (direction == BDIRECTIONS::BRIGHT) {  //right
        this->shape.rotate(90.f);
    }
    if (direction == BDIRECTIONS::BDOWN) {  //down
        this->shape.rotate(180.f);
    }
}

const sf::FloatRect Bullet::getBounds() const {
    return this->shape.getGlobalBounds();
}

void Bullet::rotate(float angle) {
    this->shape.rotate(angle);
}

int Bullet::getId() {
    return this->id;
}

void Bullet::update(float dt) {
    interpolate(dt);
}

void Bullet::setTargetPosition(sf::Vector2f pos) {
    target = pos;
}

void Bullet::interpolate(float dt) {
    if (shape.getPosition() != target) {
        float x_diff = shape.getPosition().x - target.x;
        float y_diff = shape.getPosition().y - target.y;

        // X position interpolation
        if (x_diff > dt * movementSpeed) {
            shape.setPosition(shape.getPosition().x - movementSpeed * dt, shape.getPosition().y);
        } else if (x_diff < -dt * movementSpeed) {
            shape.setPosition(shape.getPosition().x + movementSpeed * dt, shape.getPosition().y);
        } else {
            shape.setPosition(target.x, shape.getPosition().y);
        }

        // Y position interpolation
        if (y_diff > dt * movementSpeed) {
            shape.setPosition(shape.getPosition().x, shape.getPosition().y - movementSpeed * dt);
        } else if (y_diff < -dt * movementSpeed) {
            shape.setPosition(shape.getPosition().x, shape.getPosition().y + movementSpeed * dt);
        } else {
            shape.setPosition(shape.getPosition().x, target.y);
        }
    }
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(this->shape);
}

sf::Vector2f Bullet::getPosition() {
    return shape.getPosition();
}
sf::Sprite& Bullet::getSprite() {
    return shape;
}
sf::Vector2f Bullet::getDirection() {
    return this->direction;
}

void Bullet::setExpired() {
    this->expired = true;
}
bool Bullet::isExpired() {
    return this->expired;
}