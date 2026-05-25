#include "entities/FastEnemy.h"
#include <iostream>

FastEnemy::FastEnemy(int level, std::size_t phraseLength)
    : currentLevel(level),
      currentPhraseLength(phraseLength)
{
    int length = static_cast<int>(phraseLength);
    maxTime = 3.f+(0.25f*length)-(0.15f*level);
    remainingTime = maxTime;

    body.setFillColor(sf::Color::Yellow);
}

void FastEnemy::onDefeat()
{
    std::cout << "Fast enemy defeated!\n";
}