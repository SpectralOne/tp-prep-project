#include "Teleport.h"

Teleport::Teleport(int id, float pos_x, float pos_y) : 
    BaseEntity(id, pos_x, pos_y) {
    shape.setTexture(AssetManager::GetTexture("res/img/portal.png"));
    shape.setPosition(pos_x, pos_y);
    shape.scale({0.5, 0.5});
    this->id = id;
    this->expired = false;
    this->target.x = pos_x;
    this->target.y = pos_y;
}
