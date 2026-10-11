
#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QList>
#include "prompt.h"

class Database
{
public:
    Database();
    ~Database();

    bool open();
    void close();
    bool createTables();

    bool addPrompt(const Prompt& prompt);
    bool updatePrompt(const Prompt& prompt);
    bool deletePrompt(int id);

    Prompt getPrompt(int id);
    QList<Prompt> getAllPrompts();

private:
    QSqlDatabase db;
};

#endif
