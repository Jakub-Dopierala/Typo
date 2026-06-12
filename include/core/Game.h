#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <SFML/Window.hpp>
#include "core/GameObject.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "systems/TypingText.h"
#include "systems/SentenceGenerator.h"
#include <cmath>
#include <sstream>
#include <iomanip>

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

    int sentenceMistakes;

    int currentLevel;

    void resetGame();
    void drawMenu(const std::vector<std::string>& options);

    enum class GameState
    {
        MainMenu,
        Playing,
        Paused,
        GameOver,
        Scoreboard
    };

    GameState state;

    int menuSelection;

    std::vector<std::string> mainMenuOptions;
    std::vector<std::string> pauseMenuOptions;
    std::vector<std::string> gameOverOptions;

    void updateScoreboard(int newScore);
    void loadScoreboard();
    std::vector<int> scoreboard;

    void drawScoreboard();


private:
    sf::RenderWindow window;

    sf::Clock clock;

    std::vector<std::unique_ptr<GameObject>> objects;
    Enemy* currentEnemy;
    Player* player;


    sf::Font uiFont;
    TypingText typingText;

    SentenceGenerator generator;
};