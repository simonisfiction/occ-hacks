#include "popup.h"
#include "ui_popupwindow.h"

#include <QLineEdit>
#include <QTimer>
#include <QCloseEvent>

Popup::Popup(const Prompt& prompt, QWidget* parent)
    : QMainWindow(parent),
    prompt(prompt),
    ui(new Ui::PopupWindow)
{
    ui->setupUi(this);

    setWindowTitle("Keyboard Shortcut Practice");
    setWindowFlag(Qt::WindowStaysOnTopHint);

    connect(ui->answerLineEdit,
            &QLineEdit::returnPressed,
            this,
            &Popup::checkAnswer);

    setPrompt(prompt);
}

Popup::~Popup()
{
    delete ui;
}

void Popup::setPrompt(const Prompt& newPrompt)
{
    prompt = newPrompt;
    answerSubmitted = false;

    ui->promptLabel->setText(
        QString::fromStdString(prompt.getPrompt()));

    ui->answerLineEdit->clear();
    ui->answerLineEdit->setEnabled(true);
    ui->feedbackLabel->clear();
    ui->answerLineEdit->setFocus();
}

void Popup::checkAnswer()
{
    if (answerSubmitted) {
        return;
    }

    answerSubmitted = true;

    QString userAnswer =
        ui->answerLineEdit->text().trimmed();

    QString correctAnswer =
        QString::fromStdString(prompt.getAnswer()).trimmed();

    bool correct =
        userAnswer.compare(correctAnswer,
                           Qt::CaseInsensitive) == 0;

    ui->answerLineEdit->setEnabled(false);

    if (correct) {
        ui->feedbackLabel->setText("Correct!");
    } else {
        ui->feedbackLabel->setText(
            "Incorrect! The correct answer is: "
            + correctAnswer);
    }

    int delay = correct ? 1000 : 3000;

    QTimer::singleShot(delay, this, [this, correct]() {
        emit answered(prompt.getId(), correct);
        hide();
    });
}

void Popup::closeEvent(QCloseEvent* event)
{
    if (!answerSubmitted) {
        answerSubmitted = true;
        emit answered(prompt.getId(), false);
    }

    QMainWindow::closeEvent(event);
}