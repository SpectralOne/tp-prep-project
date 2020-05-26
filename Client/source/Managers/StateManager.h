#pragma once

#include <SFML/System/Time.hpp>
#include <memory>
#include <stack>
#include <string>

#include "../Managers/InputManager.h"
#include "../States/State.h"

class State;

namespace sf {
class RenderWindow;
class TcpSocket;

}  // namespace sf

class StateManager {
   public:
    StateManager();

    void run(std::unique_ptr<State> state);

    void nextState();
    void lastState();

    void update(sf::Time dt);
    void draw();

    bool running();
    void quit();

    template <typename T>
    static std::unique_ptr<T> build(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);

    template <typename T>
    static std::unique_ptr<T> rebuild(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace = true);

   private:
    //The stack of states
    std::stack<std::unique_ptr<State>> m_states;

    bool m_resume;
    bool m_running;

    std::string player_name;
    std::string cmd;
    uint32_t s_id;
};

template <typename T>
std::unique_ptr<T> StateManager::build(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) {
    // Reset the input when a new state is created
    InputManager::reset();
    return std::unique_ptr<T>(std::make_unique<T>(machine, window, info, replace));
}

template <typename T>
std::unique_ptr<T> StateManager::rebuild(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) {
    // Reset the input when a new state is created
    InputManager::reset();
    return std::unique_ptr<T>(std::make_unique<T>(machine, window, info, replace));
}
