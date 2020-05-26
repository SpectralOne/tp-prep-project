#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "AssetManager.h"

class Animator {
   public:
    struct Animation {
        std::string m_Name;
        std::string m_TextureName;
        std::vector<sf::IntRect> m_Frames;
        sf::Time m_Duration;
        bool m_Lopping;

        Animation(std::string const& name, std::string const& textureName, sf::Time const& duration, bool looping) : m_Name(name),
                                                                                                                     m_TextureName(textureName),
                                                                                                                     m_Duration(duration),
                                                                                                                     m_Lopping(looping) { }
        void AddFrames(sf::Vector2i const& startFrom, sf::Vector2i const& frameSize, unsigned int frames) {
            sf::Vector2i current = startFrom;
            for (unsigned int i = 0; i < frames; i++) {
                m_Frames.push_back(sf::IntRect(current.x, current.y, frameSize.x, frameSize.y));
                current.x += frameSize.x;
            }
        }
    };

    Animator(sf::Sprite* sprite);
    Animator::Animation& CreateAnimation(std::string const& name, std::string const& textureName, sf::Time const& duration, bool loop = false);
    void Update(sf::Time const& dt);
    bool SwitchAnimation(std::string const& name);
    std::string GetCurrentAnimationName() const;
    std::list<Animator::Animation> m_Animations;
    sf::Sprite* m_Sprite;

   private:
    Animator::Animation* FindAnimation(std::string const& name);
    void SwitchAnimation(Animator::Animation* animation);
    sf::Time m_CurrentTime;
    Animator::Animation* m_CurrentAnimation;
};
