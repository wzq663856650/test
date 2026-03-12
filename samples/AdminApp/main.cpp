#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "App.h"

int main(int argc, char* argv[])
{
    QGuiApplication guiApp(argc, argv);
    QQmlApplicationEngine engine;

    qDebug() << "========================================";
    qDebug() << "  QtPrism Admin Sample Application";
    qDebug() << "========================================";

    App app(&guiApp, &engine);
    app.Initialize();

    if (engine.rootObjects().isEmpty())
    {
        qDebug() << "[Main] Shell loaded (headless mode - no display)";
        qDebug() << "[Main] All framework components initialized successfully!";
        return 0;
    }

    return guiApp.exec();
}
