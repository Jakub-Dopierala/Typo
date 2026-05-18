#include "entities/Player.h"

Player::Player()
    : health(3), score(0), comboMultiplier(1.0f)
{
    body.setSize(sf::Vector2f(80.f, 80.f));
    body.setFillColor(sf::Color::Blue);

    position = {100.f, 400.f};
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

void Player::takeDamage(int amount)
{
    health -= amount;
}

void Player::addScore(int points)
{
    score += points;
}