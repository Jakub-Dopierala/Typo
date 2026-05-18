#pragma once
#include <string>
#include <vector>

/*
    Generates phrases depending on difficulty.
*/

class SentenceGenerator {
public:
    SentenceGenerator();

    std::string generateSentence(int level);

private:
    std::vector<std::string> easySentences;
};