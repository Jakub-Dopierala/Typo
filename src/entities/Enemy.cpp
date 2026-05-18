#include "entities/Enemy.h"

Enemy::Enemy()
{
    body.setSize(sf::Vector2f(80.f, 80.f));
    body.setFillColor(sf::Color::Red);

    position = {600.f, 400.f};
    body.setPosition(position);

    maxTime = 10.f;
    remainingTime = maxTime;
}

void Enemy::update(float dt)
{
    remainingTime -= dt;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(body);
}