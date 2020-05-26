#include "Enemy.h"

Enemy::Enemy(int id, unsigned short level, std::string name) : BasePlayer(id, level, name), m_playerAnimator(&m_sprite) {
    m_speed = 0.3f;

    // Creating the animations
    auto& idle = m_playerAnimator.CreateAnimation("idle", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_down = m_playerAnimator.CreateAnimation("down", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_left = m_playerAnimator.CreateAnimation("left", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_right = m_playerAnimator.CreateAnimation("right", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_up = m_playerAnimator.CreateAnimation("up", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_down_left = m_playerAnimator.CreateAnimation("down_left", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_down_right = m_playerAnimator.CreateAnimation("down_right", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_up_left = m_playerAnimator.CreateAnimation("up_left", "res/img/player_sheet.png", sf::seconds(0.6), true);
    auto& anim_up_right = m_playerAnimator.CreateAnimation("up_right", "res/img/player_sheet.png", sf::seconds(0.6), true);

    idle.AddFrames({128, 0}, {32, 32}, 1);
    anim_down.AddFrames({0, 0}, {32, 32}, 6);
    anim_left.AddFrames({0, 32}, {32, 32}, 6);
    anim_right.AddFrames({0, 64}, {32, 32}, 6);
    anim_up.AddFrames({0, 96}, {32, 32}, 6);
    anim_down_left.AddFrames({0, 128}, {32, 32}, 6);
    anim_down_right.AddFrames({0, 160}, {32, 32}, 6);
    anim_up_left.AddFrames({0, 192}, {32, 32}, 6);
    anim_up_right.AddFrames({0, 224}, {32, 32}, 6);

    m_playerAnimator.SwitchAnimation("idle");

    m_sprite.setScale(2, 2);

    m_name.setString(name);
    m_name.setPosition({200, 200});
    m_name.setTextSize(20);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
    m_name.draw(window);
}

void Enemy::move(sf::Vector2f vel) {
    m_currentTime = 0;
    m_velocity = vel;

    if (m_velocity.y > 0 && m_velocity.x == 0 && m_playerAnimator.GetCurrentAnimationName() != "down")
        m_playerAnimator.SwitchAnimation("down");
    else if (m_velocity.x < 0 && m_velocity.y == 0 && m_playerAnimator.GetCurrentAnimationName() != "left")
        m_playerAnimator.SwitchAnimation("left");
    else if (m_velocity.x > 0 && m_velocity.y == 0 && m_playerAnimator.GetCurrentAnimationName() != "right")
        m_playerAnimator.SwitchAnimation("right");
    else if (m_velocity.y < 0 && m_velocity.x == 0 && m_playerAnimator.GetCurrentAnimationName() != "up")
        m_playerAnimator.SwitchAnimation("up");
    else if (m_velocity.x < 0 && m_velocity.y > 0 && m_playerAnimator.GetCurrentAnimationName() != "down_left")
        m_playerAnimator.SwitchAnimation("down_left");
    else if (m_velocity.x > 0 && m_velocity.y > 0 && m_playerAnimator.GetCurrentAnimationName() != "down_right")
        m_playerAnimator.SwitchAnimation("down_right");
    else if (m_velocity.x < 0 && m_velocity.y < 0 && m_playerAnimator.GetCurrentAnimationName() != "up_left")
        m_playerAnimator.SwitchAnimation("up_left");
    else if (m_velocity.x > 0 && m_velocity.y < 0 && m_playerAnimator.GetCurrentAnimationName() != "up_right")
        m_playerAnimator.SwitchAnimation("up_right");
}

void Enemy::setTargetPosition(sf::Vector2f pos) {
    m_target_position = pos;
}

void Enemy::interpolate(float dt) {
    if (m_sprite.getPosition() != m_target_position) {
        float x_diff = m_sprite.getPosition().x - m_target_position.x;
        float y_diff = m_sprite.getPosition().y - m_target_position.y;

        move(sf::Vector2f(-x_diff, -y_diff));

        // X position interpolation
        if (x_diff > dt * m_speed) {
            m_sprite.setPosition(m_sprite.getPosition().x - m_speed * dt, m_sprite.getPosition().y);
        } else if (x_diff < -dt * m_speed) {
            m_sprite.setPosition(m_sprite.getPosition().x + m_speed * dt, m_sprite.getPosition().y);
        } else {
            m_sprite.setPosition(m_target_position.x, m_sprite.getPosition().y);
        }

        // Y position interpolation
        if (y_diff > dt * m_speed) {
            m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - m_speed * dt);
        } else if (y_diff < -dt * m_speed) {
            m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y + m_speed * dt);
        } else {
            m_sprite.setPosition(m_sprite.getPosition().x, m_target_position.y);
        }

        //  FIXME: correct position (as for main character)
        m_name.setPosition({m_sprite.getPosition().x + 31, m_sprite.getPosition().y - 21});
    }
}

void Enemy::update(float dt) {
    m_currentTime += dt;
    if (m_currentTime > 60)
        m_playerAnimator.SwitchAnimation("idle");

    m_playerAnimator.Update(sf::milliseconds(dt));
}

sf::Sprite& Enemy::getSprite() {
    return m_sprite;
}
