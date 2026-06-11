#include "core/Game.h"
#include "entities/FastEnemy.h"
#include <algorithm>
#include <iostream>

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Typing Game")
{   

    state = GameState::MainMenu;
    auto playerObject = std::make_unique<Player>();

    player = playerObject.get();

    objects.push_back(std::move(playerObject));

    spawnEnemy();

    currentLevel = 1;


    if (!uiFont.openFromFile("assets/fonts/pixel.ttf"))
    {
        std::cout << "ERROR: Could not load font!\n";
    }

    menuSelection = 0;

    mainMenuOptions =
    {
        "PLAY",
        "PVP",
        "SCOREBOARD",
        "QUIT"
    };

    pauseMenuOptions =
    {
        "RESUME",
        "MAIN MENU"
    };

    gameOverOptions =
    {
        "RESTART",
        "MAIN MENU"
    };
}

void Game::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        // Window close event
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        // Escape key PAUSES the game
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                if (state == GameState::Playing)
                {
                    state = GameState::Paused;
                    menuSelection = 0;
                }
                else if (state == GameState::Paused)
                {
                    state = GameState::Playing;
                }
            }

            if (state != GameState::Playing)
            {
                if (keyPressed->code == sf::Keyboard::Key::Up)
                    menuSelection--;

                if (keyPressed->code == sf::Keyboard::Key::Down)
                    menuSelection++;

                if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    if (state == GameState::MainMenu)
                    {
                        switch (menuSelection)
                        {
                            case 0:
                                resetGame();
                                state = GameState::Playing;
                                break;

                            case 1:
                                break;

                            case 2:
                                break;

                            case 3:
                                window.close();
                                break;
                        }
                    }

                    if (state == GameState::Paused)
                    {
                        switch (menuSelection)
                        {
                            case 0:
                                state = GameState::Playing;
                                break;

                            case 1:
                                state = GameState::MainMenu;
                                menuSelection = 0;
                                break;
                        }
                    }

                    if (state == GameState::GameOver)
                    {
                        switch (menuSelection)
                        {
                            case 0:
                                resetGame();
                                state = GameState::Playing;
                                break;

                            case 1:
                                state = GameState::MainMenu;
                                menuSelection = 0;
                                break;
                        }
                    }
                }

                int optionCount = 0;

                switch (state)
                {
                    case GameState::MainMenu:
                        optionCount = mainMenuOptions.size();
                        break;

                    case GameState::Paused:
                        optionCount = pauseMenuOptions.size();
                        break;

                    case GameState::GameOver:
                        optionCount = gameOverOptions.size();
                        break;

                    default:
                        break;
                }

                if (optionCount > 0)
                {
                    if (menuSelection < 0)
                        menuSelection = optionCount - 1;

                    if (menuSelection >= optionCount)
                        menuSelection = 0;
                }
            }
        }

        if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())   
        {
            if (!wordCompleted)
            {
                char typed = static_cast<char>(textEntered->unicode);

                if (typed == '\b')
                {
                    typingText.removeLastCharacter();
                }
                else if (typed >= 32 && typed <= 126)
                {
                    typingText.processInput(typed);
                }
            }
        }
    }
}

void Game::update(float dt)
{

    if(state != GameState::Playing)
    {
        return;
    }

    if(!wordCompleted){timer -= dt;}

    for (auto& object : objects)
    {
        object->update(dt);
    }


    // Word just completed
    if (typingText.isComplete() && !wordCompleted)
    {
        wordCompleted = true;

        completionTimer = 1.f; // linger for 1 second
    }
    // Waiting before changing word
    if (wordCompleted)
    {
        completionTimer -= dt;

        if (completionTimer <= 0.f)
        {
            // Enemy defeated
            currentLevel++;
            currentEnemy->onDefeat();

            // Remove enemy from objects
            removeCurrentEnemy();

            // Spawn new enemy
            spawnEnemy();

            wordCompleted = false;
        }
    }
    // Timer reached zero
    if (timer <= 0.f)
    {
        player->takeDamage(1);

        // Restart timer for SAME enemy
        timer = currentEnemy->getMaxTime();

        if (player->getHealth() <= 0)
        {
            state = GameState::GameOver;
            menuSelection = 0;
        }
    }
}

void Game::render()
{
    window.clear(sf::Color(180, 180, 180));

    if(state == GameState::MainMenu)
    {
        drawMenu(mainMenuOptions);

        window.display();

        return;
    }

    if(state == GameState::Paused)
    {
    

        drawMenu(pauseMenuOptions);

        window.display();

        return;
    }

    if(state == GameState::GameOver)
    {
        drawMenu(gameOverOptions);

        window.display();

        return;
    }

    for (auto& object : objects)
    {
        object->draw(window);
    }

    typingText.draw(window);
    


    // HEART BACKDROP

    sf::RectangleShape heartBackdrop;

    heartBackdrop.setSize({240.f, 60.f});

    heartBackdrop.setPosition({200.f, 600.f});

    heartBackdrop.setFillColor(sf::Color(50, 50, 50));

    heartBackdrop.setOutlineColor(sf::Color::Black);

    heartBackdrop.setOutlineThickness(4.f);

    window.draw(heartBackdrop);

    int hp = player->getHealth();

    float availableWidth = 200.f;

    float heartSize = availableWidth / hp;

    if (heartSize > 40.f)
    {
        heartSize = 40.f;
    }

    float totalWidth = hp * heartSize;

    float startX = 200.f + (240.f - totalWidth) / 2.f;

    for (int i = 0; i < hp; i++)
    {
        sf::RectangleShape heart;

        heart.setSize({heartSize - 6.f, heartSize - 6.f});

        heart.setFillColor(sf::Color::Red);

        heart.setPosition({
            startX + i * heartSize,
            600.f + (60.f - heartSize) / 2.f
        });

        window.draw(heart);
    }


    // ENEMY TYPE BOX (now same size + symmetric with hearts)

    sf::RectangleShape enemyBackdrop;

    enemyBackdrop.setSize({240.f, 60.f});

    enemyBackdrop.setPosition({840.f, 600.f});

    enemyBackdrop.setFillColor(sf::Color(50, 50, 50));

    enemyBackdrop.setOutlineColor(sf::Color::Black);

    enemyBackdrop.setOutlineThickness(4.f);

    window.draw(enemyBackdrop);

    sf::Text enemyText(uiFont);

    enemyText.setString("FAST");

    enemyText.setCharacterSize(24);

    enemyText.setFillColor(sf::Color::White);

    enemyText.setOutlineColor(sf::Color::Black);

    enemyText.setOutlineThickness(2.f);

    sf::FloatRect bounds = enemyText.getLocalBounds();

    enemyText.setPosition({
        840.f + (240.f - bounds.size.x) / 2.f,
        600.f + (60.f - bounds.size.y) / 2.f - bounds.position.y
    });

    window.draw(enemyText);


    // LEVEL

    sf::RectangleShape levelBox;

    levelBox.setSize({180.f, 60.f});

    levelBox.setPosition({40.f, 40.f});

    levelBox.setFillColor(sf::Color(50, 50, 50));

    levelBox.setOutlineColor(sf::Color::Black);

    levelBox.setOutlineThickness(4.f);

    window.draw(levelBox);

    sf::Text levelText(uiFont);

    levelText.setString("LVL " + std::to_string(currentLevel));

    levelText.setCharacterSize(24);

    sf::FloatRect levelBounds = levelText.getLocalBounds();

    levelText.setPosition({
        40.f + (180.f - levelBounds.size.x) / 2.f,
        40.f + (60.f - levelBounds.size.y) / 2.f - levelBounds.position.y
    });

    window.draw(levelText);


    // SCORE (wider + better centering)

    sf::RectangleShape scoreBox;

    scoreBox.setSize({260.f, 60.f});

    scoreBox.setPosition({980.f, 40.f});

    scoreBox.setFillColor(sf::Color(50, 50, 50));

    scoreBox.setOutlineColor(sf::Color::Black);

    scoreBox.setOutlineThickness(4.f);

    window.draw(scoreBox);

    sf::Text scoreText(uiFont);

    scoreText.setString("SCORE 0");

    scoreText.setCharacterSize(24);

    sf::FloatRect scoreBounds = scoreText.getLocalBounds();

    scoreText.setPosition({
        980.f + (260.f - scoreBounds.size.x) / 2.f,
        40.f + (60.f - scoreBounds.size.y) / 2.f - scoreBounds.position.y
    });

    window.draw(scoreText);


    // COMBO (wider + better centering)

    sf::RectangleShape comboBox;

    comboBox.setSize({260.f, 60.f});

    comboBox.setPosition({980.f, 120.f});

    comboBox.setFillColor(sf::Color(50, 50, 50));

    comboBox.setOutlineColor(sf::Color::Black);

    comboBox.setOutlineThickness(4.f);

    window.draw(comboBox);

    sf::Text comboText(uiFont);

    comboText.setString("COMBO x1");

    comboText.setCharacterSize(24);

    sf::FloatRect comboBounds = comboText.getLocalBounds();

    comboText.setPosition({
        980.f + (260.f - comboBounds.size.x) / 2.f,
        120.f + (60.f - comboBounds.size.y) / 2.f - comboBounds.position.y
    });

    window.draw(comboText);


    // TIMER

    sf::RectangleShape timerBackdrop;

    timerBackdrop.setSize({420.f, 40.f});

    timerBackdrop.setPosition({430.f, 120.f}); // moved down to align better

    timerBackdrop.setFillColor(sf::Color(50, 50, 50));

    timerBackdrop.setOutlineColor(sf::Color::Black);

    timerBackdrop.setOutlineThickness(4.f);

    window.draw(timerBackdrop);


    // TIMER BAR (clamped to backdrop)

    sf::RectangleShape timerBar;

    float maxBarWidth = 420.f;

    float barWidth = timer * 40.f;  // restored original scaling feel

    if (barWidth > maxBarWidth) barWidth = maxBarWidth;
    if (barWidth < 0.f) barWidth = 0.f;

    timerBar.setSize({barWidth, 30.f});

    if (barWidth > maxBarWidth) barWidth = maxBarWidth;
    if (barWidth < 0.f) barWidth = 0.f;

    timerBar.setSize({barWidth, 30.f});

    timerBar.setPosition({430.f, 125.f});

    timerBar.setFillColor(sf::Color::Red);

    window.draw(timerBar);

    window.display();
}

void Game::spawnEnemy()
{
    std::string phrase = generator.generateFastSentence(currentLevel);
    auto enemy = std::make_unique<FastEnemy>(
        currentLevel,
        phrase.length()
    );

    currentEnemy = enemy.get();

    
    currentEnemy->setPhrase(phrase);

    typingText.setText(phrase);

    timer = currentEnemy->getMaxTime();
    objects.push_back(std::move(enemy));
}

void Game::removeCurrentEnemy()
{
    objects.erase(
        std::remove_if(
            objects.begin(),
            objects.end(),
            [this](const auto& object)
            {
                return object.get() == currentEnemy;
            }
        ),
        objects.end()
    );
}

void Game::resetGame()
{
    objects.clear();

    auto playerObject = std::make_unique<Player>();

    player = playerObject.get();

    objects.push_back(std::move(playerObject));

    currentLevel = 1;


    wordCompleted = false;

    spawnEnemy();
}

void Game::drawMenu(const std::vector<std::string>& options)
{
    float startY = 250.f;

    for(size_t i = 0; i < options.size(); i++)
    {
        sf::Text text(uiFont);

        text.setString(options[i]);

        text.setCharacterSize(40);

        text.setPosition(
        {
            500.f,
            startY + i * 80.f
        });

        if(i == menuSelection)
        {
            text.setFillColor(sf::Color::Yellow);
        }
        else
        {
            text.setFillColor(sf::Color::White);
        }

        text.setOutlineColor(sf::Color::Black);

        text.setOutlineThickness(3.f);

        window.draw(text);
    }
}