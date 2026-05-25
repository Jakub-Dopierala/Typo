#include "entities/Player.h"

Player::Player()
    : health(3), score(0), comboMultiplier(1.0f)
{
    body.setSize(sf::Vector2f(80.f, 80.f));
    body.setFillColor(sf::Color::Blue);

    position = {1280.f / 3.f, 720.f * 0.75f};
    body.setPosition(position);
}

void Player::update(float dt)
{
    // Player logic will go here later
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

int Player::getHealth() const
{
    return health;
}

void Player::takeDamage(int amount)
{
    health -= amount;
}

void Player::addScore(int points)
{
    score += points;
}

int Player::getScore() const
{
    return score;
}

float Player::getCombo() const
{
    return comboMultiplier;
}