#ifndef PROMPT_H
#define PROMPT_H

#include <string>

class Prompt
{
private:
    std::string question;
    std::string answer;

public:
    Prompt(const std::string& question,
           const std::string& answer);

    std::string getQuestion() const;
    std::string getAnswer() const;
};

#endif // PROMPT_H
