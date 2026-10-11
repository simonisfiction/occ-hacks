#include <QApplication>
#include "popup.h"
#include "prompt.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Prompt testPrompt(
        "What is the capital of France?",
        "Paris"
        );

    Popup popup(testPrompt);
    popup.show();

    return app.exec();
}