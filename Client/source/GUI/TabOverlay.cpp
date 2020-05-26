#include "TabOverlay.h"

TabOverlay::TabOverlay(sf::Vector2f position) : m_mapName("MapName", {-350, -400}) {
    m_position = position;
    m_background.setSize({800, 400});
    m_background.setPosition({m_position.x - 800 / 2, m_position.y - 400 / 2});
    m_background.setFillColor(sf::Color(26, 24, 24, 180));
    m_visible = false;

    m_lines.push_back(OverlayLine());
}

TabOverlay::~TabOverlay() {
}

void TabOverlay::update() {
}

void TabOverlay::draw(sf::RenderTarget& window) {
    if (m_visible) {
        window.draw(m_background);
        m_mapName.draw(window);
        for (auto& line : m_lines)
            line.draw(window);
    }
}

void TabOverlay::setPosition(sf::Vector2f position) {
    m_position = position;
    m_background.setPosition({m_position.x - 800 / 2 + 32, m_position.y - 400 / 2 + 32});
    m_mapName.setPosition({m_position.x + 35, m_position.y - 130});
    for (int i = 0; i < m_lines.size(); ++i) {
        m_lines[i].setPosition({position.x - 320, position.y - 60}, i);
    }
}

void TabOverlay::setVisible(bool visible) {
    m_visible = visible;
}

void TabOverlay::addPlayer(BasePlayer& player) {
    m_lines.push_back(OverlayLine(player, m_lines.size()));
}

void TabOverlay::updatePlayer(BasePlayer& player) {
    for (int i = 1; i < m_lines.size(); ++i) {
        if (m_lines.at(i).id == player.getId()) {
            m_lines.at(i).kills.setString(std::to_string(player.getKills()));
            m_lines.at(i).deaths.setString(std::to_string(player.getDeaths()));
            m_lines.at(i).ping.setString(std::to_string(player.getPing()));
        }
    }
}

void TabOverlay::removePlayer(int id) {
    std::vector<OverlayLine>::iterator iter;
    iter = std::find_if(m_lines.begin(), m_lines.end(), [id](const OverlayLine& m) -> bool { return m.id == id; });
    m_lines.erase(iter);
}