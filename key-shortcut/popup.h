#ifndef POPUP_H
#define POPUP_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "prompt.h"

class Popup : public QDialog
{
    Q_OBJECT

private:
    int length;
    int width;
    Prompt prompt;

    QLabel* questionLabel;
    QLabel* answerLabel;
    QPushButton* answerButton;

public:
    explicit Popup(const Prompt& prompt,
                   QWidget* parent = nullptr);
};

#endif // POPUP_H
