#include "Chat.h"

Chat::Chat() : m_chatBox({0, 0}, {400, 200}) {
    m_status = CHAT_STATUS::CLOSED;
}

void Chat::draw(sf::RenderTarget& window) {
    m_chatBox.draw(window);
    m_textEntry.draw(window);
}

void Chat::update(float dt) {
    m_textEntry.update(dt);
    m_chatBox.update(dt);
}

void Chat::setPosition(sf::Vector2f position) {
    m_chatBox.setPosition({position.x - 620, position.y + 95});
    m_textEntry.setPosition({position.x - 620, position.y + 300});
}

void Chat::setPlayer(Player* player) {
    m_player = player;
}

void Chat::openChat() {
    m_status = CHAT_STATUS::OPEN;
    m_textEntry.setActive(true);
}

void Chat::send() {
    std::string message = m_textEntry.getString();
    if (message.size() > 0) {
        addPlayerMessage(message);
    }
    m_status = CHAT_STATUS::SENT;
}

void Chat::closeChat() {
    m_textEntry.clear();
    m_textEntry.setActive(false);
    m_status = CHAT_STATUS::CLOSED;
}

void Chat::addPlayerMessage(const std::string& text) {
    m_chatBox.addString("[" + m_player->getName() + "]: " + text, sf::Color::White);
    m_textEntry.setActive(false);
    m_status = CHAT_STATUS::SENT;
}

void Chat::addMessage(const std::string& text) {
    m_chatBox.addString(text, sf::Color::White);
}

void Chat::addText(sf::Uint32 code) {
    if (m_status == CHAT_STATUS::OPEN) {
        m_textEntry.addChar(static_cast<char>(code));
    }
}

void Chat::removeLastChar() {
    m_textEntry.removeChar();
}

CHAT_STATUS Chat::getStatus() {
    return m_status;
}
void Chat::setStatus(CHAT_STATUS status) {
    m_status = status;
}
std::string Chat::getMessage() {
    return m_textEntry.getString();
}
