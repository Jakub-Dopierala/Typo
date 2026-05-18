#include "systems/TypingText.h"

TypingText::TypingText()
{
}

void TypingText::setText(const std::string& text)
{
    targetText = text;
}

void TypingText::processInput(char c)
{
    currentInput += c;
}

void TypingText::draw(sf::RenderWindow& window)
{
    for (auto& letter : letters)
    {
        window.draw(letter);
    }
}