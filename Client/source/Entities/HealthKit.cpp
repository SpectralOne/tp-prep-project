#include "HealthKit.h"

HealthKit::HealthKit(int id, float pos_x, float pos_y) : 
    BaseEntity(id, pos_x, pos_y) {
    shape.setTexture(AssetManager::GetTexture("res/img/healthkit.png"));
    shape.setPosition(pos_x, pos_y);
    this->id = id;
    this->expired = false;
    this->target.x = pos_x;
    this->target.y = pos_y;
}
