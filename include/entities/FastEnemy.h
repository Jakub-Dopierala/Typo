#pragma once
#include "entities/Enemy.h"
#include <cstddef>

class FastEnemy : public Enemy {
public:
    FastEnemy(int level, std::size_t phraseLength);

    void onDefeat() override;

private:
    int currentLevel;
    std::size_t currentPhraseLength;
};