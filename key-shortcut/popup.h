#ifndef POPUP_H
#define POPUP_H

#include <QMainWindow>
#include "prompt.h"


namespace Ui {
class PopupWindow;
}

class Popup : public QMainWindow
{
    Q_OBJECT

public:
    explicit Popup(const Prompt& prompt,
                   QWidget* parent = nullptr);
    ~Popup();

    void setPrompt(const Prompt& prompt);

signals:
    void answered(int promptId, bool correct);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void checkAnswer();
    void on_btnTerminatePopup_clicked();
    void on_btnSubmitPopup_clicked();


private:
    Prompt prompt;
    Ui::PopupWindow* ui;
    bool answerSubmitted = false;
};

#endif