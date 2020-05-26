#pragma once
#include <iostream>

#include "TileMap.h"
#include "lvl.hpp"

class Level {
   public:
    Level();
    void draw(sf::RenderWindow& window);
    TileMap& getMap() { return m_map; }

   private:
    TileMap m_map;
};
