#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "lvl.hpp"

class LevelManager {
   public:
    LevelManager();
    const std::vector<sf::Vector2f>& getUnwalkable() { return onlyUnwalkablePosition; }

   private:
    std::vector<sf::Vector2f> onlyUnwalkablePosition;
};
