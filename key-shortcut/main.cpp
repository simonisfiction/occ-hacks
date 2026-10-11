#include <QApplication>
#include "popup.h"
#include "prompt.h"
#include "database.h"
#include <QDebug>
#include <QString>
#include <QList>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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
        Prompt p2("Paste copied text", "Ctrl+V");
        Prompt p3("Cut selected text", "Ctrl+X");
        Prompt p4("Undo the last action", "Ctrl+Z");
        Prompt p5("Save the current document", "Ctrl+S");
        db.addPrompt(p1);
        db.addPrompt(p2);
        db.addPrompt(p3);
        db.addPrompt(p4);
        db.addPrompt(p5);

        qDebug() << "Inserted sample prompts.";

    }

    // proving that the prompts are in the database
    qDebug() << "Total prompts:" << db.getAllPrompts().size();
    QList<Prompt> prompts = db.getAllPrompts();

    for (const Prompt& p : prompts) {
        qDebug() << "ID:" << p.getId()
        << "| Level:" << p.getLevel()
        << "| Prompt:" << QString::fromStdString(p.getPrompt())
        << "| Answer:" << QString::fromStdString(p.getAnswer());
    }

    Prompt testPrompt(
        "What is the capital of France?",
        "Paris"
        );

    Popup popup(testPrompt);
    popup.show();

    return app.exec();
}