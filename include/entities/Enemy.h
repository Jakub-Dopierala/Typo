#pragma once
#include "core/GameObject.h"
#include <string>

/*
    Abstract enemy class.
    Different enemy types inherit from this class.
*/

class Enemy : public GameObject {
public:
    Enemy();

    virtual void update(float dt) override;
    virtual void draw(sf::RenderWindow& window) override;

    virtual void onDefeat() = 0;

protected:
    std::string phrase;

    float maxTime;
    float remainingTime;

    sf::RectangleShape body;
};