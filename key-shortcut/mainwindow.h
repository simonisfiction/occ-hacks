#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPoint>
#include <QMessageBox>
#include <QPointer>

#include "database.h"
#include "promptscheduler.h"
#include "popupcontroller.h" // Add header

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Pass PopupController reference into constructor
    explicit MainWindow(PromptScheduler& scheduler, PopupController& controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();
    void showContextMenu(const QPoint &pos);
    void deleteSelectedPrompt();
    void on_btnTerminate_clicked();
    void on_btnStart_clicked(); // Slot for the Start button

private:
    Ui::MainWindow *ui;
    Database db;
    PromptScheduler& scheduler;
    PopupController& controller; // Reference to PopupController
    bool isRunning = false;       // Track scheduler status

    void loadPrompts();
};

#endif // MAINWINDOW_H