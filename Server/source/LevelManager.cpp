#include "LevelManager.h"

LevelManager::LevelManager() {
    int tileSize = 64;
    for (unsigned int i = 0; i < 64; ++i)
        for (unsigned int j = 0; j < 32; ++j) {
            int tileNumber = lvl[i + j * 64];
            if (tileNumber >= 30) {
                onlyUnwalkablePosition.push_back(sf::Vector2f(i * tileSize, j * tileSize));
            }
        }
}