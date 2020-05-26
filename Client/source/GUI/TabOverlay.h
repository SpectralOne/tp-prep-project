#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "../Entities/BasePlayer.h"
#include "Label.h"

struct OverlayLine {
    OverlayLine(BasePlayer& player, int i) : 
            level(std::to_string(player.getLevel()), {10, i * 30.0f}, 20),
            name(player.getName(), {200, i * 30.0f}, 20),
            kills(std::to_string(player.getKills()), {300, i * 30.0f}, 20),
            deaths(std::to_string(player.getDeaths()), {400, i * 30.0f}, 20),
            ping(std::to_string(player.getPing()), {4400, i * 30.0f}, 20) {
        id = player.getId();
        color = i % 2 == 0 ? sf::Color::Red : sf::Color::Blue;
    }

    OverlayLine(const sf::String& _level, const sf::String& _name, const sf::String& _kills, const sf::String& _deaths, int i) : 
            level(_level, {10, i * 30.0f}, 20),
            name(_name, {200, i * 30.0f}, 20),
            kills(_kills, {360, i * 30.0f}, 20),
            deaths(_deaths, {400, i * 30.0f}, 20),
            ping("0", {440, i * 30.0f}, 20) {
        color = i % 2 == 0 ? sf::Color::Red : sf::Color::Blue;
    }

    OverlayLine() : level("Lvl", {0, 30}, 20),
                    name("Name", {160, 30}, 20),
                    kills("Kills", {360, 30}, 20),
                    deaths("Death", {400, 30}, 20),
                    ping("Ping", {440, 30}, 20)

    {
        color = sf::Color::Cyan;
    }

    void draw(sf::RenderTarget& window) {
        level.draw(window);
        name.draw(window);
        kills.draw(window);
        deaths.draw(window);
        ping.draw(window);
    }

    void setPosition(sf::Vector2f position, float i) {
        level.setPosition({position.x, position.y + (i * 40)});
        name.setPosition({position.x + 160, position.y + (i * 40)});
        kills.setPosition({position.x + 400, position.y + (i * 40)});
        deaths.setPosition({position.x + 500, position.y + (i * 40)});
        ping.setPosition({position.x + 600, position.y + (i * 40)});
    }
    int id;
    Label level;
    Label name;
    Label kills;
    Label deaths;
    Label ping;
    sf::Color color;
};

class TabOverlay {
   public:
    TabOverlay(sf::Vector2f position);
    ~TabOverlay();

    void update();
    void draw(sf::RenderTarget& window);
    void setPosition(sf::Vector2f position);
    void setVisible(bool visible);

    void addPlayer(BasePlayer& player);
    void updatePlayer(BasePlayer& player);
    void removePlayer(int id);

   private:
    sf::RectangleShape m_background;
    Label m_mapName;
    std::vector<OverlayLine> m_lines;
    bool m_visible;
    sf::Vector2f m_position;
};
