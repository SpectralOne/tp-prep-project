#pragma once
#include <assert.h>

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <map>

struct MouseButton {
    sf::Vector2i clickPosition;
    sf::Vector2i releasePosition;
    bool hold = false;
};

class InputManager {
   public:
    InputManager();
    // Mouse
    static void setButtonClick(sf::Mouse::Button button, sf::Vector2i position);
    static void setButtonRelease(sf::Mouse::Button button, sf::Vector2i position);
    static MouseButton& getButton(sf::Mouse::Button button);
    static void reset();

    // Keyboard
    static sf::Keyboard::Key getKeyAction(const std::string& key);

   private:
    void initMap();

    std::map<sf::Mouse::Button, MouseButton> m_mouseButtons;
    std::map<std::string, sf::Keyboard::Key> m_nameMap;
    std::map<std::string, sf::Keyboard::Key> m_actionMap;

    static InputManager* sInstance;
};
