#include "StateManager.h"

StateManager::StateManager() : m_resume(false),
                               m_running(false) {}

void StateManager::run(std::unique_ptr<State> state) {
    m_running = true;
    m_states.push(std::move(state));
}

void StateManager::nextState() {
    if (m_resume) {
        if (!m_states.empty()) {
            m_states.pop();
        }

        if (!m_states.empty()) {
            m_states.top()->resume();
        }

        m_resume = false;
    }

    if (!m_states.empty()) {
        std::unique_ptr<State> temp = m_states.top()->next();

        if (temp != nullptr) {
            if (temp->isClearing()) {
                while (m_states.size() > 1)
                    m_states.pop();
            }

            if (temp->isReplacing())
                m_states.pop();
            else
                m_states.top()->pause();

            m_states.push(std::move(temp));
        }
    }
}

void StateManager::lastState() {
    // Reset the input before returning to previous state
    InputManager::reset();
    m_resume = true;
}

void StateManager::update(sf::Time dt) {
    m_states.top()->update(dt);
}

void StateManager::draw() {
    m_states.top()->draw();
}

bool StateManager::running() {
    return m_running;
}

void StateManager::quit() {
    m_running = false;
    while (m_states.empty())
        m_states.pop();
}
