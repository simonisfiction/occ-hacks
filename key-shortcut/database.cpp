
#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QUuid>
#include <QDateTime>

// Constructor
Database::Database()
{
}

// Destructor
Database::~Database()
{
    close();
}

// Open the SQLite database
bool Database::open()
{
    if (db.isOpen()) {
        return true;
    }

    // Create a connection if one doesn't already exist.
    if (!db.isValid()) {
        QString connectionName =
            "key-shortcut-" +
            QUuid::createUuid().toString(QUuid::WithoutBraces);

        db = QSqlDatabase::addDatabase(
            "QSQLITE", connectionName);
    }

    // Find a writable, platform-independent data directory.
    QString dataDir =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);

    if (dataDir.isEmpty() || !QDir().mkpath(dataDir)) {
        qWarning() << "Could not create application data directory";
        return false;
    }

    db.setDatabaseName(dataDir + "/database.db");
    qDebug() << "Database location:" << db.databaseName();
    if (!db.open()) {
        qWarning() << "Database open failed:"
                   << db.lastError().text();
        return false;
    }

    return createTables();
}

// Close the database
void Database::close()
{
    if (db.isValid() && db.isOpen()) {
        db.close();
    }
}

// Create the prompts table
bool Database::createTables()
{
    QSqlQuery query(db);

    QString sql = R"(
        CREATE TABLE IF NOT EXISTS prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level INTEGER NOT NULL DEFAuLT 1,
            prompt TEXT NOT NULL,
            answer TEXT NOT NULL,
            next_due INTEGER NOT NULL
        )
    )";

    if (!query.exec(sql)) {
        qWarning() << "Table creation failed:"
                   << query.lastError().text();
        return false;
    }

    return true;
}

// Insert a new prompt
bool Database::addPrompt(const Prompt& prompt)
{
    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO prompts (level, prompt, answer, next_due) "
        "VALUES (:level, :prompt, :answer, :next_due)"
        );

    query.bindValue(":level", prompt.getLevel());
    query.bindValue(":prompt",
                    QString::fromStdString(prompt.getPrompt()));
    query.bindValue(":answer",
                    QString::fromStdString(prompt.getAnswer()));
    query.bindValue(":next_due", prompt.getNextDue());

    if (!query.exec()) {
        qWarning() << "Insert failed:" << query.lastError().text();
        return false;
    }

    return true;
}

// Update an existing prompt
bool Database::updatePrompt(const Prompt& prompt)
{
    QSqlQuery query(db);

    query.prepare(
        "UPDATE prompts "
        "SET level = :level, prompt = :prompt, "
        "answer = :answer, next_due = :next_due "
        "WHERE id = :id"
        );

    query.bindValue(":id", prompt.getId());
    query.bindValue(":level", prompt.getLevel());
    query.bindValue(":prompt",
                    QString::fromStdString(prompt.getPrompt()));
    query.bindValue(":answer",
                    QString::fromStdString(prompt.getAnswer()));
    query.bindValue(":next_due", prompt.getNextDue());

    if (!query.exec()) {
        qWarning() << "Update failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

// Delete a prompt by ID
bool Database::deletePrompt(int id)
{
    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM prompts WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Delete failed:"
                   << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

// Retrieve one prompt by ID
Prompt Database::getPrompt(int id)
{
    QSqlQuery query(db);

    query.prepare(
        "SELECT id, level, prompt, answer, next_due "
        "FROM prompts WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Select failed:"
                   << query.lastError().text();
        return Prompt("", "");
    }

    if (!query.next()) {
        qWarning() << "No prompt found with ID:" << id;
        return Prompt("", "");
    }

    return Prompt(
        query.value("prompt").toString().toStdString(),
        query.value("answer").toString().toStdString(),
        query.value("level").toInt(),
        query.value("id").toInt(),
        query.value("next_due").toLongLong()
        );
}

// Retrieve all prompts
QList<Prompt> Database::getAllPrompts()
{
    QList<Prompt> prompts;
    QSqlQuery query(db);

    if (!query.exec(
            "SELECT id, level, prompt, answer, next_due FROM prompts")) {
        qWarning() << "Select all failed:"
                   << query.lastError().text();
        return prompts;
    }

    while (query.next()) {
        prompts.append(Prompt(
            query.value("prompt").toString().toStdString(),
            query.value("answer").toString().toStdString(),
            query.value("level").toInt(),
            query.value("id").toInt(),
            query.value("next_due").toLongLong()
            ));
    }

    return prompts;
}
