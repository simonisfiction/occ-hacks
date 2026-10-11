
#ifndef PROMPT_H
#define PROMPT_H

#include <string>

class Prompt
{
private:
    std::string prompt;
    std::string answer;
    int level;
    int id;

public:
    // Create a new prompt
    Prompt(const std::string& prompt,
           const std::string& answer);

    // Reconstruct a prompt retrieved from the database
    Prompt(const std::string& prompt,
           const std::string& answer,
           int level,
           int id);

    int getId() const;
    int getLevel() const;
    const std::string& getPrompt() const;
    const std::string& getAnswer() const;
};

#endif
