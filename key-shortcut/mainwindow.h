#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPoint>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();
    void showContextMenu(const QPoint &pos);
    void deleteSelectedPrompt();
    void on_btnTerminate_clicked();

private:
    Ui::MainWindow *ui;
    Database db;

    void loadPrompts();
};

#endif // MAINWINDOW_H
