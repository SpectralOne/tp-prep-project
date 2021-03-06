#include "TileMap.h"

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height) {
    m_size = {(int)width, (int)height};
    // Load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    //resize the vertex array to get the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j) {

            //get the cureent tile number
            int tileNumber = tiles[i + j * width];
            if (tileNumber >= TILES::UNWALKABLE) {
                walkable = false;
                onlyUnwalkable.push_back(walkable);
                onlyUnwalkablePosition.push_back(sf::Vector2f(i * tileSize.x, j * tileSize.y));
            } else {
                walkable = true;
			}
            walkableVector.push_back(walkable);
            tileIndex.push_back(sf::Vector2i(i, j));

            //  find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            //  get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            //  define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            //  add it to the vector for collision
            tilePosition.push_back(sf::Vector2f(i * tileSize.x, j * tileSize.y));

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    return true;
}

sf::Vector2i TileMap::getSize() {
    return m_size;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

void TileMap::draw(sf::RenderTarget& target) const {
    target.draw(m_vertices);
}
