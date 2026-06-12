#pragma once
#include "core/GameObject.h"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

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

    void registerCorrectLetter();
    void registerMistake();
    void resetStreak();
    void addScore(
        int sentenceLength,
        int mistakesInSentence,
        float timeLeftPercent
    );
    void addScore(
        int sentenceLength,
        int mistakesInSentence);

private:
    int health;
    int score;
    float comboMultiplier;
    int consecutiveCorrect;

    sf::RectangleShape body;

    static sf::Texture texture;
    sf::Sprite sprite;
};