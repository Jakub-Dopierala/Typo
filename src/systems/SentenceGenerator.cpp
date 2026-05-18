#include "systems/SentenceGenerator.h"
#include <cstdlib>

SentenceGenerator::SentenceGenerator()
{
    easySentences.push_back("destroy enemy");
    easySentences.push_back("attack goblin");
    easySentences.push_back("defeat skeleton");
}

std::string SentenceGenerator::generateSentence(int level)
{
    int index = rand() % easySentences.size();

    return easySentences[index];
}