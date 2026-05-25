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
    int getHealth() const;
    void takeDamage(int amount);
    void addScore(int points);
    int getScore() const;
    float getCombo() const;

private:
    int health;
    int score;
    float comboMultiplier;

    sf::RectangleShape body;
};