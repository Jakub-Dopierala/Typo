#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/*
    Handles:
    - displaying text letter by letter
    - validating typed input
    - coloring letters
*/

class TypingText {
public:
    TypingText();

    void setText(const std::string& text);

    void processInput(char c);

    void removeLastCharacter();

    void draw(sf::RenderWindow& window);

    bool isComplete() const;


private:
    void rebuildLetters();

private:
    std::string targetText;
    std::string currentInput;

    sf::Font font;
    std::vector<sf::Text> letters;
    TypingText(const TypingText&) = delete;
    TypingText& operator=(const TypingText&) = delete;
    
};