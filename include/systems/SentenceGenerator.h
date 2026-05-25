#pragma once

#include <string>
#include <vector>

/*
    Handles loading and generating phrases
    for different enemy types.
*/

class SentenceGenerator
{
public:
    SentenceGenerator();

    std::string generateFastSentence(int level);

private:
    void loadFastWords();

private:
    std::vector<std::string> fastVerbs;
    std::vector<std::string> fastAdjectives;
    std::vector<std::string> fastNouns;
};