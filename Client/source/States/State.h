#pragma once
#include <iostream>
#include <memory>

#include <SFML/Network.hpp>

class StateManager;

namespace sf {
class RenderWindow;
class Time;
}  // namespace sf

typedef struct {
    std::string pName;
    std::string ip;
    unsigned short port;
    bool isCon;
} pInfo_t;

class State {
   public:
    State(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);
    virtual ~State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    pInfo_t& operator=(const pInfo_t info);

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void update(sf::Time dt) = 0;
    virtual void draw() = 0;

    std::unique_ptr<State> next();

    void setPlayerName(std::string name);
    bool isReplacing();
    bool isClearing();

   protected:
    StateManager& m_machine;
    sf::RenderWindow& m_window;

    bool m_replacing;

    std::unique_ptr<State> m_next;
    bool m_clear;
    pInfo_t info;
};
