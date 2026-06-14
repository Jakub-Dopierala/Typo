#include "entities/TankEnemy.h"
#include <iostream>


sf::Texture TankEnemy::texture;

TankEnemy::TankEnemy(int level, std::size_t phraseLength)
    : currentLevel(level),
      currentPhraseLength(phraseLength),
      sprite(texture)   
{
    ENtype="TANK";
    if (texture.getSize().x == 0)
    {
        if (!texture.loadFromFile("assets/textures/fast_gob.png"))
        {
            std::cerr << "Failed to load fast_gob.png\n";
        }
        texture.setSmooth(false);

    }
    sprite = sf::Sprite(texture);  
    sprite.setTexture(texture);

    sprite.setScale({10.f, 10.f});
    sprite.setPosition({1280.f * 0.68f, 720.f * 0.60f});

    maxTime = 3.f + (0.25f * phraseLength) - (0.25f * level);
    remainingTime = maxTime;
}

void TankEnemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void TankEnemy::onDefeat()
{
    // TODO
}