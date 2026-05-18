#include "entities/FastEnemy.h"
#include <iostream>

FastEnemy::FastEnemy()
{
    maxTime = 5.f;
    remainingTime = maxTime;

    body.setFillColor(sf::Color::Yellow);
}

void FastEnemy::onDefeat()
{
    std::cout << "Fast enemy defeated!\n";
}