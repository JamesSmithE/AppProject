#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "TextDataPacket.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Project", "Main");

    TextData td;
    TextDataPacket tdp(std::move(td));

    return app.exec();
}
