#ifndef POPUP_H
#define POPUP_H

#include <QMainWindow>
#include "prompt.h"

namespace Ui {
class MainWindow;
}

class Popup : public QMainWindow
{
    Q_OBJECT

private:
    Prompt prompt;
    Ui::MainWindow *ui;

private slots:
    void checkAnswer();

public:
    explicit Popup(const Prompt& prompt,
                   QWidget* parent = nullptr);

    ~Popup();
};

#endif // POPUP_H