#include "systems/TypingText.h"

#include <iostream>

TypingText::TypingText()
{
    if (!font.openFromFile("assets/fonts/pixel.ttf"))
    {
        std::cout << "ERROR: Could not load font!\n";
    }
}

void TypingText::setText(const std::string& text)
{
    targetText = text;
    currentInput.clear();

    rebuildLetters();
}

void TypingText::processInput(char c)
{
    // Prevent typing beyond sentence length
    if (currentInput.size() >= targetText.size())
    {
        return;
    }

    currentInput += c;

    rebuildLetters();
}

void TypingText::removeLastCharacter()
{
    if (!currentInput.empty())
    {
        currentInput.pop_back();

        rebuildLetters();
    }
}

bool TypingText::isComplete() const
{
    return currentInput == targetText;
}

void TypingText::rebuildLetters()
{
    letters.clear();

    float startX = 300.f;
    float startY = 200.f;

    float spacing = 36.f;

    for (size_t i = 0; i < targetText.size(); i++)
    {
        sf::Text letter(font);

        if (targetText[i] == ' ')
        {
            letter.setString("_");
        }
        else
        {
            letter.setString(std::string(1, targetText[i]));
        }

        letter.setCharacterSize(40);

        letter.setPosition({startX + i * spacing, startY});

        // Default color
        sf::Color color = sf::Color::White;

        // Letter correctness check
        if (i < currentInput.size())
        {
            if (currentInput[i] == targetText[i])
            {
                color = sf::Color::Green;
            }
            else
            {
                color = sf::Color::Red;
            }
        }

        letter.setFillColor(color);

        // OUTLINE
        letter.setOutlineColor(sf::Color::Black);
        letter.setOutlineThickness(3.f);

        letters.push_back(letter);
    }
}

void TypingText::draw(sf::RenderWindow& window)
{
    for (auto& letter : letters)
    {
        window.draw(letter);
    }
}