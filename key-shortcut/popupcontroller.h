#ifndef POPUPCONTROLLER_H
#define POPUPCONTROLLER_H

#include <QObject>
#include <QTimer>

#include "popup.h"
#include "promptscheduler.h"

class PopupController : public QObject
{
    Q_OBJECT

public:
    explicit PopupController(
        PromptScheduler& scheduler,
        QObject* parent = nullptr);

    void start();

private:
    PromptScheduler& scheduler;
    QTimer timer;
    Popup popup;

    void checkForPrompt();
    void scheduleNextCheck();
    void handleAnswer(int id, bool correct);
};

#endif