#include "Player.h"

Player::Player(std::string name) : BasePlayer(-1, 1, name), m_playerAnimator(&m_sprite) {
    m_speed = 0.3f;

    this->walk.setBuffer(AssetManager::GetSoundBuffer("res/sound/step_short_fast.wav"));
    this->walk.setVolume(35.f);

    this->shoot.setBuffer(AssetManager::GetSoundBuffer("res/sound/gun_shot.wav"));
    this->shoot.setVolume(35.f);

    this->heal.setBuffer(AssetManager::GetSoundBuffer("res/sound/heal.wav"));
    this->heal.setVolume(35.f);

    this->teleport.setBuffer(AssetManager::GetSoundBuffer("res/sound/teleport.wav"));
    this->teleport.setVolume(35.f);

    moving = false;

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

    // 64 offset
    // auto& idle = m_playerAnimator.CreateAnimation("idle", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_down = m_playerAnimator.CreateAnimation("down", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_left = m_playerAnimator.CreateAnimation("left", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_right = m_playerAnimator.CreateAnimation("right", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_up = m_playerAnimator.CreateAnimation("up", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_down_left = m_playerAnimator.CreateAnimation("down_left", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_down_right = m_playerAnimator.CreateAnimation("down_right", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_up_left = m_playerAnimator.CreateAnimation("up_left", "res/img/Player.png", sf::seconds(0.6), true);
    // auto& anim_up_right = m_playerAnimator.CreateAnimation("up_right", "res/img/Player.png", sf::seconds(0.6), true);

    // idle.AddFrames({320, 192}, {64, 64}, 1);
    // anim_down.AddFrames({0, 640}, {64, 64}, 9);
    // anim_left.AddFrames({0, 64}, {64, 64}, 9);
    // anim_right.AddFrames({0, 128}, {64, 64},9);
    // anim_up.AddFrames({0, 192}, {64, 64}, 9);
    // anim_down_left.AddFrames({0, 256}, {64, 64}, 9);
    // anim_down_right.AddFrames({0, 320}, {64, 64}, 9);
    // anim_up_left.AddFrames({0, 364}, {64, 64}, 9);
    // anim_up_right.AddFrames({0, 428}, {64, 64}, 9);

    m_playerAnimator.SwitchAnimation("idle");

    m_sprite.setScale(2, 2);

    alive = true;
    allowed_to_shoot = false;

    hp = 100;
    attackCooldown = 20.f;
    attackCooldownMax = 20.f;
    direction = DIRECTIONS::DOWN;
    spectator = false;
    won = false;
}

void Player::setShadowPosition(sf::Vector2f pos) {
    m_shadow.setPosition(pos);
}

void Player::setVelocity(sf::Vector2f vel) {
    m_velocity = vel;
    if (vel.y > 0 && vel.x == 0) {
        m_playerAnimator.SwitchAnimation("down");
        direction = DIRECTIONS::DOWN;
    } else if (vel.x < 0 && vel.y == 0) {
        m_playerAnimator.SwitchAnimation("left");
        direction = DIRECTIONS::LEFT;
    } else if (vel.x > 0 && vel.y == 0) {
        m_playerAnimator.SwitchAnimation("right");
        direction = DIRECTIONS::RIGHT;
    } else if (vel.y < 0 && vel.x == 0) {
        m_playerAnimator.SwitchAnimation("up");
        direction = DIRECTIONS::UP;
    } else if (vel.x < 0 && vel.y > 0) {
        m_playerAnimator.SwitchAnimation("down_left");
        direction = DIRECTIONS::LEFT;
    } else if (vel.x > 0 && vel.y > 0) {
        m_playerAnimator.SwitchAnimation("down_right");
        direction = DIRECTIONS::RIGHT;
    } else if (vel.x < 0 && vel.y < 0) {
        m_playerAnimator.SwitchAnimation("up_left");
        direction = DIRECTIONS::LEFT;
    } else if (vel.x > 0 && vel.y < 0) {
        m_playerAnimator.SwitchAnimation("up_right");
        direction = DIRECTIONS::RIGHT;
    } else {
        m_playerAnimator.SwitchAnimation("idle");
    }

    if (vel.x || vel.y) {
        this->moving = true;
    } else {
        this->moving = false;
    }

    if (this->moving && !won) {
        if (!this->walk.getLoop()) {
            this->walk.setLoop(true);
            this->walk.play();
        }
    } else {
        this->walk.setLoop(false);
        this->walk.pause();
    }
}

sf::Vector2f Player::getVelocity() {
    return m_velocity;
}

float Player::getPlayerSpeed() {
    return m_speed;
}

sf::Sprite& Player::getSprite() {
    return m_sprite;
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}

void Player::update(float dt) {
    m_playerAnimator.Update(sf::milliseconds(dt));
    m_sprite.move(sf::Vector2f(m_velocity.x * m_speed * dt, m_velocity.y * m_speed * dt));
}

void Player::loseHp(const int value) {
    this->hp -= value;
    if (this->hp < 0)
        this->hp = 0;
}

void Player::setShoot(bool status) {
    this->allowed_to_shoot = status;
}

void Player::setAlive(bool status) {
    this->alive = status;
}

bool Player::canAttack() {
    if (this->attackCooldown >= this->attackCooldownMax && allowed_to_shoot && !spectator) {
        this->attackCooldown = 0.f;
        return true;
    }

    return false;
}

void Player::updateAttack() {
    if (this->attackCooldown < this->attackCooldownMax)
        this->attackCooldown += 0.5f;
}

void Player::shootSound() {
    this->shoot.play();
}

void Player::healSound() {
    this->heal.play();
}

void Player::teleportSound() {
    this->teleport.play();
}

int Player::getDirection() {
    return direction;
}
bool Player::isAlive() {
    return alive;
}
void Player::setSpectator(bool status) {
    spectator = status;
}
bool Player::getSpectator() {
    return spectator;
}
void Player::setWon() {
    won = true;
}
bool Player::getWon() {
    return won;
}
void Player::setSpeed(float speed) {
    m_speed = speed;
}
