#pragma once
#include "core/GameObject.h"

/*
    Represents the player character.
    Stores health, score and combo multiplier.
*/

class Player : public GameObject {
public:
    Player();

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void takeDamage(int amount);
    void addScore(int points);

private:
    int health;
    int score;
    float comboMultiplier;

    sf::RectangleShape body;
};