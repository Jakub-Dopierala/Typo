#pragma once
#include "entities/Enemy.h"
#include <SFML/Graphics.hpp>
#include <cstddef>

class TrickEnemy : public Enemy {
public:
    TrickEnemy(int level, std::size_t phraseLength);

    void onDefeat() override;
    void draw(sf::RenderWindow& window) override;

private:
    int currentLevel;
    std::size_t currentPhraseLength;

    static sf::Texture texture;
    sf::Sprite sprite;
};