#pragma once

#include <SFML/Graphics.hpp>

#include "../Managers/AssetManager.h"
#include "BaseEntity.h"

class Teleport : public BaseEntity {
   public:
    Teleport(int id, float pos_x, float pos_y);
};
