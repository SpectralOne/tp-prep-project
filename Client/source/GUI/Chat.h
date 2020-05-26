#pragma once

#include <SFML/Graphics.hpp>

#include "../Entities/Player.h"
#include "TextBoxScrollable.h"
#include "TextEntry.h"

enum CHAT_STATUS {
    OPEN,
    SENT,
    CLOSED
};

class Chat {
   public:
    Chat();
    void draw(sf::RenderTarget& window);
    void update(float dt);
    void setPosition(sf::Vector2f position);
    void setPlayer(Player* player);

    void openChat();
    void send();
    void closeChat();
    void addPlayerMessage(const std::string& text);
    void addMessage(const std::string& text);

    void addText(sf::Uint32 code);
    void removeLastChar();
    CHAT_STATUS getStatus();
    void setStatus(CHAT_STATUS status);
    std::string getMessage();

   private:
    TextBoxScrollable m_chatBox;
    TextEntry m_textEntry;

    CHAT_STATUS m_status;
    Player* m_player;
};
