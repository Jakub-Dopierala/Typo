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

    

    float boxX = (1280.f - 700.f) / 2.f;

    float startY = 200.f;

    unsigned int characterSize = 40;

    if (targetText.size() > 20)
    {
        characterSize = 32;
    }

    if (targetText.size() > 28)
    {
        characterSize = 24;
    }

    float letterSpacing = characterSize * 1.f;
    float totalWidth = targetText.size()*letterSpacing;

    float startX = boxX + (700.f - totalWidth) / 2.f;

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


        letter.setCharacterSize(characterSize);

        letter.setPosition({startX + i * letterSpacing, startY});

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
    sf::RectangleShape backdrop;
    backdrop.setSize({1000.f, 100.f});

    backdrop.setFillColor(sf::Color(80, 80, 80));

    backdrop.setOutlineColor(sf::Color::Black);

    backdrop.setOutlineThickness(3.f);

    backdrop.setPosition({140.f, 160.f});
    window.draw(backdrop);

    for (auto& letter : letters)
    {
        window.draw(letter);
    }
}