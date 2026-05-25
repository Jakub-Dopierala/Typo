#include "entities/Enemy.h"

Enemy::Enemy()
{
    body.setSize(sf::Vector2f(80.f, 80.f));
    body.setFillColor(sf::Color::Red);

    position = {1280.f * (2.f / 3.f), 720.f * 0.75f};
    body.setPosition(position);

    maxTime = 10.f;
    remainingTime = maxTime;
}

float Enemy::getMaxTime() const
{
    return maxTime;
}

const std::string& Enemy::getPhrase() const
{
    return phrase;
}

void Enemy::setPhrase(const std::string& newPhrase)
{
    phrase = newPhrase;
}

void Enemy::update(float dt)
{
    remainingTime -= dt;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(body);
}