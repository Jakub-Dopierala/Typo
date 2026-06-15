#include "entities/FastEnemy.h"
#include <iostream>


sf::Texture FastEnemy::texture;

FastEnemy::FastEnemy(int level, std::size_t phraseLength)
    : currentLevel(level),
      currentPhraseLength(phraseLength),
      sprite(texture)   
{
    ENtype="FAST";
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

    maxTime = 3.f + (0.4f * phraseLength) - (0.25f * level);
    if(maxTime<4.f){maxTime=4.f;}
    remainingTime = maxTime;
}

void FastEnemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void FastEnemy::onDefeat()
{
}