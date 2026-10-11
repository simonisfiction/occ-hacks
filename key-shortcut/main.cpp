#include <QApplication>
#include <QDebug>
#include <QString>
#include <QList>
#include <QtCore/qdatetime.h>

#include "mainwindow.h"
#include "popup.h"
#include "prompt.h"
#include "database.h"
#include "promptscheduler.h"
#include "popupcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Prevent app exit when popup windows close while MainWindow is hidden/minimized
    app.setQuitOnLastWindowClosed(false);

    // Instantiate and open database
    Database db;
    if (!db.open()) {
        qCritical() << "Failed to open database";
        return 1;
    }

    // Insert sample prompts if DB is brand new
    if (db.getAllPrompts().isEmpty()) {
        Prompt p1("Copy selected text", "Ctrl+C");

        // Prompt p2("Paste copied text", "Ctrl+V");
        // Prompt p3("Cut selected text", "Ctrl+X");
        // Prompt p4("Undo the last action", "Ctrl+Z");
        // Prompt p5("Save the current document", "Ctrl+S");
        if (db.addPrompt(p1)) {
            qDebug() << "Prompt inserted successfully!";
        } else {
            qWarning() << "Failed to insert prompt!";
        }
        // if (db.addPrompt(p2)) {
        //     qDebug() << "Prompt inserted successfully!";
        // } else {
        //     qWarning() << "Failed to insert prompt!";
        // }
        // if (db.addPrompt(p3)) {
        //     qDebug() << "Prompt inserted successfully!";
        // } else {
        //     qWarning() << "Failed to insert prompt!";
        // }
        // if (db.addPrompt(p4)) {
        //     qDebug() << "Prompt inserted successfully!";
        // } else {
        //     qWarning() << "Failed to insert prompt!";
        // }
        // if (db.addPrompt(p5)) {
        //     qDebug() << "Prompt inserted successfully!";
        // } else {
        //     qWarning() << "Failed to insert prompt!";
        // }




        qDebug() << "Inserted sample prompts.";
    }

    // Initialize and start scheduled popups
    PromptScheduler scheduler(db);
    PopupController controller(scheduler);
    controller.start();

    // Show Admin Panel on application launch
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}