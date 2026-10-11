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

    //Insert sample prompts if DB is brand new
    if (db.getAllPrompts().isEmpty()) {
        Prompt p1("what is letter a", "a");
        Prompt p2("what is letter b", "b");
        Prompt p3("what is letter c", "c");

        db.addPrompt(p1);
        db.addPrompt(p2);
        db.addPrompt(p3);


        qDebug() << "Inserted sample prompts.";
    }

    // Initialize and start scheduled popups
    PromptScheduler scheduler(db);
    PopupController controller(scheduler);
    // controller.start();

    // Show Admin Panel on application launch
    MainWindow mainWindow(scheduler, controller);
    mainWindow.show();

    return app.exec();
}