#pragma once

#include <string>
#include <vector>

/*
    Handles loading and generating phrases
    for different enemy types.
*/

enum class EnemyType
{
    Fast,
    Tank
};

struct WordSet
{
    std::vector<std::string> verbs;
    std::vector<std::string> adjectives;
    std::vector<std::string> nouns;
};

class SentenceGenerator
{
public:
    SentenceGenerator();

    std::string generateSentence(EnemyType type, int level);
    std::string generateTrickSentence(int level);

private:
    void loadWords(const std::string& filePath, WordSet& wordSet);
    void loadTrickSentences();

    std::vector<std::string> trickSentences;

    WordSet fastWords;
    WordSet tankWords;
};