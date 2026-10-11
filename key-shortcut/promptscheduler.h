#ifndef PROMPTSCHEDULER_H
#define PROMPTSCHEDULER_H

#include "database.h"
#include "prompt.h"

#include <QList>
#include <chrono>
#include <optional>

class PromptScheduler
{
public:
    explicit PromptScheduler(Database& database);

    // Reload prompts from the database.
    void reload();

    // Return the next due prompt, or std::nullopt if none is due.
    // A returned prompt is marked as awaiting an answer.
    std::optional<Prompt> takeNextDuePrompt();

    // Update a prompt after the employee answers.
    bool markCorrect(int id);
    bool markIncorrect(int id);

    // Time until the next available prompt is due.
    // Returns milliseconds::max() if no prompt is available.
    std::chrono::milliseconds timeUntilNextPrompt() const;

private:
    using Clock = std::chrono::steady_clock;

    struct ScheduledPrompt
    {
        Prompt prompt;
        Clock::time_point nextDue;
        bool awaitingAnswer;
    };

    Database& database;
    QList<ScheduledPrompt> scheduledPrompts;

    static Clock::duration intervalForLevel(int level);

};

#endif // PROMPTSCHEDULER_H