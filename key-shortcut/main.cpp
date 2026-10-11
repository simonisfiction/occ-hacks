#include <QApplication>
#include <QDebug>
#include <QString>
#include <QList>
#include <QtCore/qdatetime.h>

#include "popup.h"
#include "prompt.h"
#include "database.h"
#include "promptscheduler.h"
#include "popupcontroller.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    //instantiate the database object
    Database db;
    //opens the database. if the batabase file doesn't exist, this creates it.
    if (!db.open()) {
        qCritical() << "Failed to open database";
        return 1;
    }
    //inserts test prompts and answers into the db
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


    // After db.open() succeeds:
    PromptScheduler scheduler(db);

    PopupController controller(scheduler);
    controller.start();


    // auto next = scheduler.takeNextDuePrompt();

    // if (next.has_value()) {
    //     qDebug() << "Due prompt:"
    //              << QString::fromStdString(next->getPrompt());

    //     qDebug() << "Current level:" << next->getLevel();

    //     if (scheduler.markCorrect(next->getId())) {
    //         qDebug() << "Correct answer recorded!";
    //     }

    //     Prompt updated = db.getPrompt(next->getId());

    //     qDebug() << "New level:" << updated.getLevel();
    //     qDebug() << "Next due:"
    //              << QDateTime::fromMSecsSinceEpoch(
    //                     updated.getNextDue()).toString();
    // }

    // // proving that the prompts are in the database
    // qDebug() << "Total prompts:" << db.getAllPrompts().size();
    // QList<Prompt> prompts = db.getAllPrompts();

    // for (const Prompt& p : prompts) {
    //     qDebug() << "ID:" << p.getId()
    //     << "| Level:" << p.getLevel()
    //     << "| Prompt:" << QString::fromStdString(p.getPrompt())
    //     << "| Answer:" << QString::fromStdString(p.getAnswer());
    // }

    // Prompt testPrompt(
    //     "What is the capital of France?",
    //     "Paris"
    //     );

    // Popup popup(testPrompt);
    // popup.show();

    return app.exec();
}