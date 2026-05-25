#include "systems/SentenceGenerator.h"

#include <fstream>
#include <iostream>
#include <sstream>

SentenceGenerator::SentenceGenerator()
{
    loadFastWords();
}

void SentenceGenerator::loadFastWords()
{
    std::ifstream file("assets/phrases/fast_enemy.txt");

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not open fast_enemy.txt\n";
        return;
    }

    std::string line;

    enum Section
    {
        NONE,
        VERBS,
        ADJECTIVES,
        NOUNS
    };

    Section currentSection = NONE;

    while (std::getline(file, line))
    {
        if (line == "VERBS")
        {
            currentSection = VERBS;
            continue;
        }

        if (line == "ADJECTIVES")
        {
            currentSection = ADJECTIVES;
            continue;
        }

        if (line == "NOUNS")
        {
            currentSection = NOUNS;
            continue;
        }

        if (line.empty())
        {
            continue;
        }

        switch (currentSection)
        {
            case VERBS:
                fastVerbs.push_back(line);
                break;

            case ADJECTIVES:
                fastAdjectives.push_back(line);
                break;

            case NOUNS:
                fastNouns.push_back(line);
                break;

            default:
                break;
        }
    }
}

std::string SentenceGenerator::generateFastSentence(int level)
{
    // Limit difficulty growth
    int maxIndex = std::min(level, (int)fastVerbs.size() - 1);

    int verbIndex = rand() % (maxIndex + 1);
    int nounIndex = rand() % (maxIndex + 1);

    std::string sentence;

    sentence += fastVerbs[verbIndex];
    sentence += " ";

    // Add adjective later in progression
    if (level >= 3)
    {
        int adjectiveIndex = rand() % (maxIndex + 1);

        sentence += fastAdjectives[adjectiveIndex];
        sentence += " ";
    }

    sentence += fastNouns[nounIndex];

    return sentence;
}