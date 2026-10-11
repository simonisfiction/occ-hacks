#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "prompt.h"
#include <QApplication>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Space Cards");

    // Ensure database connection is established
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Failed to open the database.");
    }

    // Configure promptList for right-click context menus
    ui->promptList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->promptList, &QListWidget::customContextMenuRequested,
            this, &MainWindow::showContextMenu);

    // Populate initial prompt list from database
    loadPrompts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Loads all prompts from the database into promptList
void MainWindow::loadPrompts()
{
    ui->promptList->clear();
    QList<Prompt> prompts = db.getAllPrompts();

    for (const Prompt &p : prompts) {
        // Display string format in the list widget
        QString displayText = QString::fromStdString(p.getPrompt()) +
                              " (Ans: " + QString::fromStdString(p.getAnswer()) + ")";

        QListWidgetItem *item = new QListWidgetItem(displayText, ui->promptList);

        // Store the database ID in the item's custom UserRole data
        item->setData(Qt::UserRole, p.getId());
    }
}

// Handler for btnAdd clicked
void MainWindow::on_btnAdd_clicked()
{
    QString promptText = ui->addPrompt->text().trimmed();
    QString answerText = ui->addAnswer->text().trimmed();

    if (promptText.isEmpty() || answerText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both a prompt and an answer.");
        return;
    }

    // Create a new Prompt using the 2-parameter constructor.
    // Level, ID, and initial next_due default automatically.
    Prompt newPrompt(
        promptText.toStdString(),
        answerText.toStdString()
        );

    if (db.addPrompt(newPrompt)) {
        ui->addPrompt->clear();
        ui->addAnswer->clear();
        loadPrompts(); // Refresh prompt display
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to add prompt to database.");
    }
}

// Context menu slot when right-clicking promptList
void MainWindow::showContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = ui->promptList->itemAt(pos);
    if (!item) {
        return; // User clicked empty space inside list widget
    }

    QMenu contextMenu(this);
    QAction *deleteAction = contextMenu.addAction("Delete Prompt");

    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedPrompt);

    contextMenu.exec(ui->promptList->mapToGlobal(pos));
}

// Handles prompt deletion
void MainWindow::deleteSelectedPrompt()
{
    QListWidgetItem *item = ui->promptList->currentItem();
    if (!item) {
        return;
    }

    // Extract the stored primary key ID
    int promptId = item->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton confirm = QMessageBox::question(
        this,
        "Confirm Delete",
        "Are you sure you want to delete this prompt?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirm == QMessageBox::Yes) {
        if (db.deletePrompt(promptId)) {
            delete item; // Remove row item directly from widget
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete prompt from database.");
        }
    }
}

void MainWindow::on_btnTerminate_clicked()
{
    QApplication::quit();
}