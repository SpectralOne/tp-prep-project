#pragma once

#include <SFML/Graphics.hpp>

#include "../Managers/AssetManager.h"
#include "BaseEntity.h"

class HealthKit : public BaseEntity {
   public:
    HealthKit(int id, float pos_x, float pos_y);
};
