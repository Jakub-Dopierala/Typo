#include "entities/Player.h"

sf::Texture Player::texture;

Player::Player()
    : health(2),
      score(0),
      comboMultiplier(1.0f),
      consecutiveCorrect(0),
      sprite(texture)
{
    if (texture.getSize().x == 0)
    {
        if (!texture.loadFromFile("assets/textures/wiz1.png"))
        {
            std::cerr << "Failed to load wiz.png\n";
        }
        texture.setSmooth(false);
    }

    sprite.setTexture(texture);
    sprite.setScale({10.f, 10.f});

    position = {1280.f * 0.2f, 720.f * 0.60f};
    sprite.setPosition(position);

    body.setSize(sf::Vector2f(80.f, 80.f));
    body.setFillColor(sf::Color::Blue);
    body.setPosition(position);
}

void Player::update(float dt)
{
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

int Player::getHealth() const
{
    return health;
}

void Player::takeDamage(int amount)
{
    health -= amount;
}

void Player::addScore(int points)
{
    score += points;
}

int Player::getScore() const
{
    return score;
}

float Player::getCombo() const
{
    return comboMultiplier;
}

void Player::registerCorrectLetter()
{
    consecutiveCorrect++;

    if (consecutiveCorrect % 5 == 0)
    {
        comboMultiplier += 0.1f;

        if (comboMultiplier > 3.f)
        {
            comboMultiplier = 3.f;
        }
    }
}

void Player::registerMistake()
{
    consecutiveCorrect = 0;

    comboMultiplier -= 0.2f;

    if (comboMultiplier < 1.f)
    {
        comboMultiplier = 1.f;
    }
}

void Player::resetStreak()
{
    consecutiveCorrect = 0;
}

void Player::addScore(
    int sentenceLength,
    int mistakesInSentence,
    float timeLeftPercent)
{
    if (sentenceLength <= 0)
    {
        return;
    }

    float baseScore = static_cast<float>(sentenceLength);

    float mistakePercent =
        static_cast<float>(mistakesInSentence) / sentenceLength;

    float accuracyMultiplier;

    if (mistakesInSentence == 0)
        accuracyMultiplier = 2.f;
    else if (mistakePercent < 0.05f)
        accuracyMultiplier = 1.5f;
    else if (mistakePercent < 0.10f)
        accuracyMultiplier = 1.2f;
    else if (mistakePercent < 0.20f)
        accuracyMultiplier = 1.f;
    else
        accuracyMultiplier = 0.8f;

    float speedMultiplier;

    if (timeLeftPercent > 0.35f)
        speedMultiplier = 1.5f;
    else if (timeLeftPercent > 0.20f)
        speedMultiplier = 1.2f;
    else
        speedMultiplier = 1.f;

    score += static_cast<int>(
        std::round(
            baseScore *
            comboMultiplier *
            accuracyMultiplier *
            speedMultiplier));
}

void Player::addScore(
    int sentenceLength,
    int mistakesInSentence)
{
    if (sentenceLength <= 0)
    {
        return;
    }

    float baseScore = static_cast<float>(sentenceLength);

    float mistakePercent =
        static_cast<float>(mistakesInSentence) / sentenceLength;

    float accuracyMultiplier;

    if (mistakesInSentence == 0)
        accuracyMultiplier = 2.f;
    else if (mistakePercent < 0.05f)
        accuracyMultiplier = 1.5f;
    else if (mistakePercent < 0.10f)
        accuracyMultiplier = 1.2f;
    else if (mistakePercent < 0.20f)
        accuracyMultiplier = 1.f;
    else
        accuracyMultiplier = 0.8f;

    score += static_cast<int>(
        std::round(
            baseScore *
            comboMultiplier *
            accuracyMultiplier));
}