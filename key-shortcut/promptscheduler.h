#ifndef PROMPTSCHEDULER_H
#define PROMPTSCHEDULER_H

#include "database.h"

#include <optional>
#include <chrono>

class PromptScheduler
{
public:
    explicit PromptScheduler(Database& database);

    std::optional<Prompt> takeNextDuePrompt();

    bool markCorrect(int id);
    bool markIncorrect(int id);

    std::chrono::milliseconds timeUntilNextPrompt() const;

private:
    Database& database;
    std::optional<int> activePromptId;
};

#endif