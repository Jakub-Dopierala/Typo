#pragma once
#include <SFML/Graphics.hpp>

/*
    Abstract base class for every object that exists in the game world.
    All derived objects must implement update() and draw().
*/

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

protected:
    sf::Vector2f position;
};