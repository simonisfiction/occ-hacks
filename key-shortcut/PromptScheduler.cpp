#include "promptscheduler.h"

#include <QDateTime>
#include <algorithm>
#include <cmath>

PromptScheduler::PromptScheduler(Database& database)
    : database(database)
{
}

std::optional<Prompt> PromptScheduler::takeNextDuePrompt()
{
    // Prevent two simultaneous popups.
    if (activePromptId.has_value()) {
        return std::nullopt;
    }

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<Prompt> prompts = database.getAllPrompts();

    std::optional<Prompt> earliest;

    for (const Prompt& prompt : prompts) {
        if (prompt.getNextDue() > now) {
            continue;
        }

        if (!earliest.has_value() ||
            prompt.getNextDue() < earliest->getNextDue() ||
            (prompt.getNextDue() == earliest->getNextDue() &&
             prompt.getId() < earliest->getId())) {
            earliest = prompt;
        }
    }

    if (earliest.has_value()) {
        activePromptId = earliest->getId();
    }

    return earliest;
}

bool PromptScheduler::markCorrect(int id)
{
    if (!activePromptId.has_value() ||
        *activePromptId != id) {
        return false;
    }

    Prompt old = database.getPrompt(id);

    if (old.getId() != id) {
        return false;
    }

    int newLevel = old.getLevel() + 1;

    // Cap the level to prevent exponential overflow.
    newLevel = std::min(newLevel, 20);

    qint64 intervalMs =
        static_cast<qint64>(std::exp(newLevel) * 1000.0);

    qint64 nextDue =
        QDateTime::currentMSecsSinceEpoch() + intervalMs;

    Prompt updated(
        old.getPrompt(),
        old.getAnswer(),
        newLevel,
        old.getId(),
        nextDue
        );

    if (!database.updatePrompt(updated)) {
        return false;
    }

    activePromptId.reset();
    return true;
}

bool PromptScheduler::markIncorrect(int id)
{
    if (!activePromptId.has_value() ||
        *activePromptId != id) {
        return false;
    }

    Prompt old = database.getPrompt(id);

    if (old.getId() != id) {
        return false;
    }
    int newLevel = old.getLevel();
    if(old.getLevel() > 1)
    {
        newLevel = newLevel - 1;
    }

    newLevel = std::min(newLevel, 20);
    qint64 intervalMs =
        static_cast<qint64>(std::exp(newLevel) * 1000.0);

    qint64 nextDue =
        QDateTime::currentMSecsSinceEpoch() + intervalMs;

    Prompt updated(
        old.getPrompt(),
        old.getAnswer(),
        newLevel,
        old.getId(),
        nextDue
        );

    if (!database.updatePrompt(updated)) {
        return false;
    }

    activePromptId.reset();
    return true;
}

std::chrono::milliseconds
PromptScheduler::timeUntilNextPrompt() const
{
    if (activePromptId.has_value()) {
        return std::chrono::milliseconds::max();
    }

    QList<Prompt> prompts = database.getAllPrompts();

    if (prompts.isEmpty()) {
        return std::chrono::milliseconds::max();
    }

    qint64 earliest = prompts.first().getNextDue();

    for (const Prompt& prompt : prompts) {
        earliest = std::min(earliest, prompt.getNextDue());
    }

    qint64 now = QDateTime::currentMSecsSinceEpoch();

    return std::chrono::milliseconds(
        std::max(qint64(0), earliest - now)
        );
}