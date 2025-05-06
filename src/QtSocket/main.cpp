#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include"SocketClientController.h"
#include"SocketServerController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<SocketClientController>("Treg", 1, 0, "SocketClientController");
    qmlRegisterType<SocketServerController>("Treg", 1, 0, "SocketServerController");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QtSocket", "Main");

    return app.exec();
}
