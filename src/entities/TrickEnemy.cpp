#include "entities/TrickEnemy.h"
#include <iostream>


sf::Texture TrickEnemy::texture;

TrickEnemy::TrickEnemy(int level, std::size_t phraseLength)
    : currentLevel(level),
      currentPhraseLength(phraseLength),
      sprite(texture)   
{
    ENtype="TRICK";
    if (texture.getSize().x == 0)
    {
        if (!texture.loadFromFile("assets/textures/trick_gob.png"))
        {
            std::cerr << "Failed to load trick_gob.png\n";
        }
        texture.setSmooth(false);
    }
    sprite = sf::Sprite(texture);  
    sprite.setTexture(texture);

    sprite.setScale({10.f, 10.f});
    sprite.setPosition({1280.f * 0.68f, 720.f * 0.60f});

    maxTime = 3.f + (0.35f * phraseLength) - (0.15f * level);
    if(maxTime<4.f){maxTime=4.f;}
    remainingTime = maxTime;
}

void TrickEnemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void TrickEnemy::onDefeat()
{
}