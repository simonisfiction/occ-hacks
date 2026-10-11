
#include "popup.h"

#include <QVBoxLayout>
#include <QString>

Popup::Popup(const Prompt& prompt, QWidget* parent)
    : QDialog(parent),
    length(400),
    width(200),
    prompt(prompt)
{
    setWindowTitle("Prompt");
    resize(length, width);

    questionLabel = new QLabel(
        QString::fromStdString(prompt.getQuestion()), this);

    answerLabel = new QLabel(this);
    answerLabel->setText(
        QString::fromStdString(prompt.getAnswer()));
    answerLabel->hide();

    answerButton = new QPushButton("Show Answer", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(questionLabel);
    layout->addWidget(answerLabel);
    layout->addWidget(answerButton);

    connect(answerButton, &QPushButton::clicked,
            this, [this]() {
                answerLabel->show();
                answerButton->hide();
            });
}

