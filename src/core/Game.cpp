#include "core/Game.h"
#include "entities/FastEnemy.h"
#include <algorithm>

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Typing Game")
{
    auto playerObject = std::make_unique<Player>();

    player = playerObject.get();

    objects.push_back(std::move(playerObject));

    spawnEnemy();

    currentLevel = 1;

    gameOver = false;
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

        // Escape key closes the game
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                window.close();
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

    if (gameOver)
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
            gameOver = true;
        }
    }
}

void Game::render()
{
    window.clear(sf::Color(180, 180, 180));


    for (auto& object : objects)
    {
        object->draw(window);
    }

    typingText.draw(window);

    sf::RectangleShape timerBar;

    timerBar.setSize({timer * 40.f, 30.f});

    timerBar.setPosition({300.f, 100.f});

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