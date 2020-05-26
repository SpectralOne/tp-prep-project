#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <algorithm>
#include <vector>

enum TILES {
    UNWALKABLE = 30
};

class TileMap : public sf::Drawable, public sf::Transformable {
   public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
    sf::Vector2i getSize();

    std::vector<bool> walkableVector;
    std::vector<bool> onlyUnwalkable;
    std::vector<sf::Vector2f> tilePosition;
    std::vector<sf::Vector2f> onlyUnwalkablePosition;
    std::vector<sf::Vector2i> tileIndex;

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void draw(sf::RenderTarget& target) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    bool walkable;
    sf::Vector2i m_size;
    sf::Vector2i m_index;
};
