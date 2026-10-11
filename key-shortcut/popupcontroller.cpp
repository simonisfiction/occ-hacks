#include "popupcontroller.h"

#include <QDebug>
#include <algorithm>
#include <chrono>

PopupController::PopupController(
    PromptScheduler& scheduler,
    QObject* parent)
    : QObject(parent),
    scheduler(scheduler),
    popup(Prompt("", ""))
{
    timer.setSingleShot(true);

    connect(&timer, &QTimer::timeout,
            this, &PopupController::checkForPrompt);

    connect(&popup, &Popup::answered,
            this, &PopupController::handleAnswer);
}

void PopupController::start()
{
    timer.start(0);
}

void PopupController::checkForPrompt()
{
    if (popup.isVisible()) {
        return;
    }

    auto next = scheduler.takeNextDuePrompt();

    if (next.has_value()) {
        qDebug() << "Showing prompt ID:"
                 << next->getId();

        popup.setPrompt(*next);
        popup.show();
        popup.raise();
        popup.activateWindow();
    } else {
        scheduleNextCheck();
    }
}

void PopupController::scheduleNextCheck()
{
    auto remaining = scheduler.timeUntilNextPrompt();

    // Recheck at least once per minute.
    auto delay = std::min(
        remaining,
        std::chrono::milliseconds(60000)
        );

    timer.start(static_cast<int>(delay.count()));
}

void PopupController::handleAnswer(int id, bool correct)
{
    bool saved = correct
                     ? scheduler.markCorrect(id)
                     : scheduler.markIncorrect(id);

    if (!saved) {
        qWarning() << "Could not save answer for prompt" << id;
        // Stop here rather than continuing with an active prompt.
        timer.stop();
        return;
    }

    qDebug() << "Prompt" << id
             << (correct ? "correct" : "incorrect");

    // Allow a short break before the next popup.
    timer.start(3000);
}