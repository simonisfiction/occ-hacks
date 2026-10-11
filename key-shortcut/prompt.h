
#ifndef PROMPT_H
#define PROMPT_H

#include <string>
#include <QtGlobal>

class Prompt
{
private:
    std::string prompt;
    std::string answer;
    int level;
    int id;
    qint64 nextDue;

public:
    // Create a new prompt
    Prompt(const std::string& prompt,
           const std::string& answer);

    // Reconstruct a prompt retrieved from the database
    Prompt(const std::string& prompt,
           const std::string& answer,
           int level,
           int id,
           qint64 nextDue);

    int getId() const;
    int getLevel() const;
    qint64 getNextDue() const;

    const std::string& getPrompt() const;
    const std::string& getAnswer() const;
};

#endif
