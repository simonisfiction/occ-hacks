#include "mainwindow.h"
#include <QApplication>
#include "popup.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Prompt prompt("What is the capital of France?", "Paris");

    Popup popup(prompt);
    popup.exec();

    return app.exec();
}