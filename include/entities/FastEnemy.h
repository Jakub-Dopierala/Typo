#pragma once
#include "entities/Enemy.h"

/*
    Fast enemy:
    shorter phrases but less time to type.
*/

class FastEnemy : public Enemy {
public:
    FastEnemy();

    void onDefeat() override;
};