#include "BaseEntity.h"

BaseEntity::BaseEntity(int id, float pos_x, float pos_y) {
    this->id = id;
    this->expired = false;
    this->target.x = pos_x;
    this->target.y = pos_y;
}

void BaseEntity::setTargetPosition(sf::Vector2f pos) {
    target = pos;
}

const sf::FloatRect BaseEntity::getBounds() const {
    return this->shape.getGlobalBounds();
}

void BaseEntity::rotate(float angle) {
    this->shape.rotate(angle);
}

int BaseEntity::getId() {
    return this->id;
}

void BaseEntity::render(sf::RenderWindow& window) {
    window.draw(this->shape);
}

sf::Vector2f BaseEntity::getPosition() {
    return shape.getPosition();
}
sf::Sprite& BaseEntity::getSprite() {
    return shape;
}

void BaseEntity::setExpired() {
    this->expired = true;
}
bool BaseEntity::isExpired() {
    return this->expired;
}