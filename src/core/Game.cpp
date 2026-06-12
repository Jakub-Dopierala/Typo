#include "core/Game.h"
#include "entities/FastEnemy.h"
#include <algorithm>
#include <iostream>

Game::Game()
    : window(sf::VideoMode({1280, 720}), "Typing Game")
{   

    matchType = MatchType::Singleplayer;
    pvpPhase = PvPPhase::None;

    state = GameState::MainMenu;
    auto playerObject = std::make_unique<Player>();

    player = playerObject.get();

    objects.push_back(std::move(playerObject));

    if (matchType == MatchType::Singleplayer)
    {
        spawnEnemy();
    }

    currentLevel = 1;
    sentenceMistakes = 0;

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
                else if(state == GameState::Scoreboard)
                {
                    state = GameState::MainMenu;
                    menuSelection = 0;
                }
                else if(state == GameState::MainMenu)
                {
                    window.close();
                    break;
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
                    if (matchType == MatchType::PvP &&
                        pvpPhase == PvPPhase::SwitchPlayers)
                    {
                        resetGame();

                        pvp.completedSentences = 0;
                        typingText.setText(pvp.sentences[0]);
                        sentenceMistakes = 0;

                        pvp.timer.restart();

                        pvpPhase = PvPPhase::Player2Turn;
                        state = GameState::Playing;

                        return;
                    }
                    if (state == GameState::MainMenu)
                    {
                        switch (menuSelection)
                        {
                            case 0:
                                resetGame();
                                state = GameState::Playing;
                                break;

                            case 1:
                                startPvP();

                                state = GameState::Playing;
                                break;

                            case 2:
                                loadScoreboard();
                                state = GameState::Scoreboard;
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
                                if(matchType == MatchType::PvP){
                                    matchType = MatchType::Singleplayer;
                                    pvpPhase = PvPPhase::None;
                                    state = GameState::MainMenu;
                                    menuSelection = 0;
                                break;

                                }
                                updateScoreboard(player->getScore());
                                state = GameState::MainMenu;
                                menuSelection = 0;
                                break;
                        }
                    }

                    if (state == GameState::GameOver)
                    {
                        if (matchType == MatchType::PvP
                            && pvpPhase == PvPPhase::Results)
                        {
                            state = GameState::MainMenu;

                            matchType = MatchType::Singleplayer;

                            pvpPhase = PvPPhase::None;

                            menuSelection = 0;

                            break;
                        }
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
                    player->resetStreak();
                    typingText.removeLastCharacter();
                }
                else if (typed >= 32 && typed <= 126)
                {
                    TypingText::InputResult result = typingText.processInput(typed);
                    if (result == TypingText::InputResult::Correct)
                    {
                        player->registerCorrectLetter();
                    }
                    else if (result == TypingText::InputResult::Incorrect)
                    {
                        player->registerMistake();

                        sentenceMistakes++;
                    }
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

    if(matchType == MatchType::Singleplayer)
    {
        if(!wordCompleted)
        {
            timer -= dt;
        }
    }

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

            if(matchType == MatchType::Singleplayer)
            {
                float timePercent =
                    timer / currentEnemy->getMaxTime();

                player->addScore(
                    currentEnemy->getPhrase().length(),
                    sentenceMistakes,
                    timePercent);

                currentLevel++;

                currentEnemy->onDefeat();

                removeCurrentEnemy();

                spawnEnemy();
            }
            else
            {
                player->addScore(
                    currentEnemy->getPhrase().length(),
                    sentenceMistakes);

                handlePvPCompletion();
            }

            wordCompleted = false;
        }
    }
    // Timer reached zero
    if (matchType == MatchType::Singleplayer && timer <= 0.f)
    {
        player->takeDamage(1);

        // Restart timer for SAME enemy
        timer = currentEnemy->getMaxTime();

        if (player->getHealth() <= 0)
        {
            updateScoreboard(player->getScore());
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
        if(matchType == MatchType::PvP
        && pvpPhase == PvPPhase::SwitchPlayers)
        {
            drawPvPSwitchScreen();
            window.display();
            return;
        }
        else
        {
            drawMenu(pauseMenuOptions);
            window.display();
            return;
        }
    }

    if(state == GameState::Scoreboard)
    {
        drawScoreboard();
        window.display();
        return;
    }
    if(state == GameState::GameOver)
    {
        if(matchType == MatchType::PvP && pvpPhase == PvPPhase::Results)
        {
            drawPvPResults();

            window.display();

            return;
        }

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

    scoreText.setString("SCORE " + std::to_string(player->getScore()));

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


    std::ostringstream ss;
    ss << player->getCombo();
    comboText.setString("COMBO " + ss.str() +"x");


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

void Game::updateScoreboard(int newScore)
{
    std::ifstream input("assets/data/scoreboard.txt");

    std::vector<int> scores;

    int score;

    while (input >> score)
    {
        scores.push_back(score);
    }

    input.close();

    scores.push_back(newScore);

    std::sort(
        scores.begin(),
        scores.end(),
        std::greater<int>());

    if (scores.size() > 10)
    {
        scores.resize(10);
    }

    std::ofstream output("assets/data/scoreboard.txt");

    for (int value : scores)
    {
        output << value << '\n';
    }
}

void Game::loadScoreboard()
{
    scoreboard.clear();

    std::ifstream input(
        "assets/data/scoreboard.txt");

    int score;

    while (input >> score)
    {
        scoreboard.push_back(score);
    }
}

void Game::drawScoreboard()
{
    sf::Text title(uiFont);

    title.setString("SCOREBOARD");

    title.setCharacterSize(50);

    title.setFillColor(sf::Color::Yellow);

    title.setOutlineColor(sf::Color::Black);

    title.setOutlineThickness(3.f);

    title.setPosition({400.f, 50.f});

    window.draw(title);

    float startY = 120.f;

    for (size_t i = 0; i < scoreboard.size(); i++)
    {
        sf::Text text(uiFont);

        text.setString(
            std::to_string(i + 1)
            + ". "
            + std::to_string(scoreboard[i]));

        text.setCharacterSize(35);

        text.setFillColor(sf::Color::White);

        text.setOutlineColor(sf::Color::Black);

        text.setOutlineThickness(2.f);

        text.setPosition(
        {
            500.f,
            startY + static_cast<float>(i) * 50.f
        });

        window.draw(text);
    }

}

void Game::startPvP()
{
    matchType = MatchType::PvP;

    pvp = PvPData();

    pvpPhase = PvPPhase::Player1Turn;

    resetGame();

    pvp.sentences.clear();

    for (int i = 0; i < 5; i++)
    {
        pvp.sentences.push_back(
            generator.generateFastSentence(i + 1));
    }

    typingText.setText(pvp.sentences[0]);

    sentenceMistakes = 0;

    pvp.timer.restart();
}


void Game::handlePvPCompletion()
{
    pvp.completedSentences++;

    if (pvp.completedSentences < 5)
    {
        typingText.setText(
            pvp.sentences[pvp.completedSentences]);

        sentenceMistakes = 0;

        wordCompleted = false;

        return;
    }

    float elapsed =
        pvp.timer.getElapsedTime().asSeconds();

    if (pvpPhase == PvPPhase::Player1Turn)
    {
        pvp.player1Score = player->getScore();

        pvp.player1Time = elapsed;

        pvpPhase = PvPPhase::SwitchPlayers;

        state = GameState::Paused;

        menuSelection = 0;
    }
    else
    {
        pvp.player2Score = player->getScore();

        pvp.player2Time = elapsed;

        pvpPhase = PvPPhase::Results;

        state = GameState::GameOver;
    }
}

void Game::drawPvPSwitchScreen()
{
    sf::Text title(uiFont);

    title.setString("PLAYER 1 COMPLETE!");

    if (pvpPhase == PvPPhase::SwitchPlayers)
    {
        title.setString("PLAYER 2, GET READY!");
    }

    title.setCharacterSize(50);

    title.setFillColor(sf::Color::White);

    title.setOutlineColor(sf::Color::Black);

    title.setOutlineThickness(3.f);

    sf::FloatRect bounds = title.getLocalBounds();

    title.setPosition(
    {
        640.f - bounds.size.x / 2.f,
        250.f
    });

    window.draw(title);


    sf::Text instruction(uiFont);

    instruction.setString("PRESS ENTER TO BEGIN");

    instruction.setCharacterSize(35);

    instruction.setFillColor(sf::Color::Yellow);

    instruction.setOutlineColor(sf::Color::Black);

    instruction.setOutlineThickness(2.f);

    bounds = instruction.getLocalBounds();

    instruction.setPosition(
    {
        640.f - bounds.size.x / 2.f,
        350.f
    });

    window.draw(instruction);
}

void Game::drawPvPResults()
{
    float slower =
        std::max(
            pvp.player1Time,
            pvp.player2Time);

    int adjusted1 =
        static_cast<int>(
            pvp.player1Score
            * slower
            / pvp.player1Time);

    int adjusted2 =
        static_cast<int>(
            pvp.player2Score
            * slower
            / pvp.player2Time);


    std::string winner;

    if (adjusted1 > adjusted2)
    {
        winner = "PLAYER 1 WINS!";
    }
    else if (adjusted2 > adjusted1)
    {
        winner = "PLAYER 2 WINS!";
    }
    else
    {
        winner = "DRAW!";
    }


    sf::Text title(uiFont);

    title.setString(winner);

    title.setCharacterSize(50);

    title.setFillColor(sf::Color::Yellow);

    title.setOutlineColor(sf::Color::Black);

    title.setOutlineThickness(3.f);

    sf::FloatRect bounds = title.getLocalBounds();

    title.setPosition(
    {
        640.f - bounds.size.x / 2.f,
        80.f
    });

    window.draw(title);


    sf::Text player1(uiFont);

    player1.setString(
        "PLAYER 1\n"
        "Score: " + std::to_string(adjusted1)
        + "\nTime: "
        + std::to_string(static_cast<int>(pvp.player1Time)));

    player1.setCharacterSize(30);

    player1.setFillColor(sf::Color::White);

    player1.setOutlineColor(sf::Color::Black);

    player1.setOutlineThickness(2.f);

    player1.setPosition({250.f, 220.f});

    window.draw(player1);


    sf::Text player2(uiFont);

    player2.setString(
        "PLAYER 2\n"
        "Score: " + std::to_string(adjusted2)
        + "\nTime: "
        + std::to_string(static_cast<int>(pvp.player2Time)));

    player2.setCharacterSize(30);

    player2.setFillColor(sf::Color::White);

    player2.setOutlineColor(sf::Color::Black);

    player2.setOutlineThickness(2.f);

    player2.setPosition({750.f, 220.f});

    window.draw(player2);


    sf::Text instruction(uiFont);

    instruction.setString(
        "PRESS ENTER TO RETURN TO MENU");

    instruction.setCharacterSize(30);

    instruction.setFillColor(sf::Color::Yellow);

    instruction.setOutlineColor(sf::Color::Black);

    instruction.setOutlineThickness(2.f);

    bounds = instruction.getLocalBounds();

    instruction.setPosition(
    {
        640.f - bounds.size.x / 2.f,
        600.f
    });

    window.draw(instruction);
}