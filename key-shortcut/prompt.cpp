#include "prompt.h"

Prompt::Prompt(const std::string& question,
               const std::string& answer)
    : question(question), answer(answer)
{
}

std::string Prompt::getQuestion() const
{
    return question;
}

std::string Prompt::getAnswer() const
{
    return answer;
}
