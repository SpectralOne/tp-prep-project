#include "State.h"

State::State(StateManager& machine, sf::RenderWindow& window, pInfo_t info, bool replace) : m_machine{machine},
                                                                                             m_window{window},
                                                                                             m_replacing{replace} {
    this->info = info;
    bool m_clear = false;
}

std::unique_ptr<State> State::next() {
    return std::move(m_next);
}

bool State::isReplacing() {
    return m_replacing;
}

bool State::isClearing() {
    return m_clear;
}

void State::setPlayerName(std::string name) {
    this->info.pName = name;
}
