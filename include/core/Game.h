#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <SFML/Window.hpp>
#include "core/GameObject.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "systems/TypingText.h"
#include "systems/SentenceGenerator.h"

/*
    Main game class.
    Controls the game loop and manages objects.
*/

class Game {
public:
    Game();

    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void spawnEnemy();
    void removeCurrentEnemy();
    float timer;
    bool wordCompleted = false;
    float completionTimer = 0.f;

    int currentLevel;
    bool gameOver;


private:
    sf::RenderWindow window;

    sf::Clock clock;

    std::vector<std::unique_ptr<GameObject>> objects;
    Enemy* currentEnemy;
    Player* player;


    
    TypingText typingText;

    SentenceGenerator generator;
};