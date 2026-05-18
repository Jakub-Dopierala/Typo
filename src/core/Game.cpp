#include "core/Game.h"
#include "entities/FastEnemy.h"

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Typing Game")
{
    auto playerObject = std::make_unique<Player>();

    player = playerObject.get();

    objects.push_back(std::move(playerObject));

    spawnEnemy();
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
    }
}

void Game::update(float dt)
{

    for (auto& object : objects)
    {
        object->update(dt);
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

    window.display();
}

void Game::spawnEnemy()
{
    objects.push_back(std::make_unique<FastEnemy>());
}