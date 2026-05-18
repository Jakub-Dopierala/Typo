#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/*
    Handles rendering and validation
    of the typing sentence.
*/

class TypingText {
public:
    TypingText();

    void setText(const std::string& text);
    void processInput(char c);

    void draw(sf::RenderWindow& window);

private:
    std::string targetText;
    std::string currentInput;

    sf::Font font;
    std::vector<sf::Text> letters;
};