#include "popup.h"
#include "ui_popupwindow.h"

#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <Qt>

Popup::Popup(const Prompt& prompt, QWidget* parent)
    : QMainWindow(parent),
    prompt(prompt),
    ui(new Ui::PopupWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog);
    setWindowTitle("Prompt");

    ui->promptLabel->setText(
        QString::fromStdString(prompt.getPrompt()));

    ui->feedbackLabel->clear();
    ui->answerLineEdit->setFocus();

    connect(ui->answerLineEdit,
            &QLineEdit::returnPressed,
            this,
            &Popup::checkAnswer);
}

Popup::~Popup()
{
    delete ui;
}

void Popup::checkAnswer()
{
    QString userAnswer =
        ui->answerLineEdit->text().trimmed();

    QString correctAnswer =
        QString::fromStdString(prompt.getAnswer()).trimmed();

    ui->answerLineEdit->setDisabled(true);

    if (userAnswer.compare(
            correctAnswer, Qt::CaseInsensitive) == 0)
    {
        ui->feedbackLabel->setText("Correct!");

        QTimer::singleShot(
            1000, this, &QWidget::close);
    }
    else
    {
        ui->feedbackLabel->setText(
            "Incorrect! The correct answer is: "
            + correctAnswer);

        QTimer::singleShot(
            10000, this, &QWidget::close);
    }
}

