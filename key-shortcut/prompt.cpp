
#include "prompt.h"

// Constructor for a new prompt
Prompt::Prompt(const std::string& prompt,
               const std::string& answer)
    : prompt(prompt),
    answer(answer),
    level(1),
    id(-1)
{
}

// Constructor for a prompt loaded from the database
Prompt::Prompt(const std::string& prompt,
               const std::string& answer,
               int level,
               int id)
    : prompt(prompt),
    answer(answer),
    level(level),
    id(id)
{
}

int Prompt::getId() const
{
    return id;
}

int Prompt::getLevel() const
{
    return level;
}

const std::string& Prompt::getPrompt() const
{
    return prompt;
}

const std::string& Prompt::getAnswer() const
{
    return answer;
}
