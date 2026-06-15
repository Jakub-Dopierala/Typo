#include "systems/SentenceGenerator.h"

#include <fstream>
#include <iostream>
#include <algorithm>

SentenceGenerator::SentenceGenerator()
{
    loadWords("assets/phrases/fast_enemy.txt", fastWords);
    loadWords("assets/phrases/tank_enemy.txt", tankWords);
    loadTrickSentences();
}

void SentenceGenerator::loadWords(const std::string& filePath, WordSet& wordSet)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not open " << filePath << "\n";
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
                wordSet.verbs.push_back(line);
                break;

            case ADJECTIVES:
                wordSet.adjectives.push_back(line);
                break;

            case NOUNS:
                wordSet.nouns.push_back(line);
                break;

            default:
                break;
        }
    }
}

std::string SentenceGenerator::generateSentence(EnemyType type, int level)
{
    const WordSet& words =
        (type == EnemyType::Fast) ? fastWords : tankWords;

    if (words.verbs.empty() ||
        words.adjectives.empty() ||
        words.nouns.empty())
    {
        return "ERROR";
    }

    int verbMaxIndex = std::min(level, static_cast<int>(words.verbs.size()) - 1);
    int nounMaxIndex = std::min(level, static_cast<int>(words.nouns.size()) - 1);

    int verbIndex = rand() % (verbMaxIndex + 1);
    int nounIndex = rand() % (nounMaxIndex + 1);

    std::string sentence;

    sentence += words.verbs[verbIndex];
    sentence += " ";

    if (level >= 8)
    {
        int adjectiveMax =
            std::min(level, static_cast<int>(words.adjectives.size()) - 1);

        int adjectiveIndex = rand() % (adjectiveMax + 1);

        sentence += words.adjectives[adjectiveIndex];
        sentence += " ";
    }

    sentence += words.nouns[nounIndex];

    return sentence;
}

void SentenceGenerator::loadTrickSentences()
{
    std::ifstream file("assets/phrases/trick_enemy.txt");

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not open trick_enemy.txt\n";
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            trickSentences.push_back(line);
        }
    }
}

std::string SentenceGenerator::generateTrickSentence(int level)
{
    if (trickSentences.empty())
    {
        return "ERROR";
    }

    int maxIndex =
        std::min(level, static_cast<int>(trickSentences.size()) - 1);

    int sentenceIndex = rand() % (maxIndex + 1);

    return trickSentences[sentenceIndex];
}