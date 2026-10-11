#include "promptscheduler.h"

#include <cmath>

// Construct the scheduler and load the prompts.
PromptScheduler::PromptScheduler(Database& database)
    : database(database)
{
    reload();
}

// Load all prompts from the database.
// Every prompt is initially eligible to appear.
void PromptScheduler::reload()
{
    scheduledPrompts.clear();

    const auto prompts = database.getAllPrompts();
    const auto now = Clock::now();

    for (const Prompt& prompt : prompts) {
        scheduledPrompts.append(
            ScheduledPrompt{prompt, now, false}
            );
    }
}

// Calculate the interval in seconds: e^level.
PromptScheduler::Clock::duration
PromptScheduler::intervalForLevel(int level)
{
    const double seconds = std::exp(
        static_cast<double>(level)
        );

    return std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>(seconds)
        );
}

// Get the earliest prompt that is due.
std::optional<Prompt> PromptScheduler::takeNextDuePrompt()
{
    const auto now = Clock::now();

    ScheduledPrompt* next = nullptr;

    for (ScheduledPrompt& scheduled : scheduledPrompts) {
        if (scheduled.awaitingAnswer ||
            scheduled.nextDue > now) {
            continue;
        }

        if (next == nullptr ||
            scheduled.nextDue < next->nextDue) {
            next = &scheduled;
        }
    }

    if (next == nullptr) {
        return std::nullopt;
    }

    // Don't return this same prompt again while it awaits an answer.
    next->awaitingAnswer = true;

    return next->prompt;
}

// Increase the level after a correct answer and save it.
bool PromptScheduler::markCorrect(int id)
{
    for (ScheduledPrompt& scheduled : scheduledPrompts) {
        if (scheduled.prompt.getId() != id) {
            continue;
        }

        if (!scheduled.awaitingAnswer) {
            return false;
        }

        Prompt updated(
            scheduled.prompt.getPrompt(),
            scheduled.prompt.getAnswer(),
            scheduled.prompt.getLevel() + 1,
            scheduled.prompt.getId()
            );

        if (!database.updatePrompt(updated)) {
            return false;
        }

        scheduled.prompt = updated;
        scheduled.nextDue =
            Clock::now() + intervalForLevel(updated.getLevel());
        scheduled.awaitingAnswer = false;

        return true;
    }

    return false;
}

// Retry an incorrect prompt after one second.
// Its level does not change.
bool PromptScheduler::markIncorrect(int id)
{
    for (ScheduledPrompt& scheduled : scheduledPrompts) {
        if (scheduled.prompt.getId() != id) {
            continue;
        }

        if (!scheduled.awaitingAnswer) {
            return false;
        }

        scheduled.nextDue =
            Clock::now() + std::chrono::seconds(1);
        scheduled.awaitingAnswer = false;

        return true;
    }

    return false;
}

// Calculate how long until the next prompt can appear.
std::chrono::milliseconds
PromptScheduler::timeUntilNextPrompt() const
{
    const auto now = Clock::now();

    bool found = false;
    Clock::time_point earliest;

    for (const ScheduledPrompt& scheduled : scheduledPrompts) {
        if (scheduled.awaitingAnswer) {
            continue;
        }

        if (!found || scheduled.nextDue < earliest) {
            earliest = scheduled.nextDue;
            found = true;
        }
    }

    if (!found) {
        return std::chrono::milliseconds::max();
    }

    if (earliest <= now) {
        return std::chrono::milliseconds::zero();
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        earliest - now
        );
}