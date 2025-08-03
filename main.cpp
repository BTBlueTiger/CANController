#include "include/BleGattClient.h"
#include "include/SDLController.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qRegisterMetaType<SimpleBTDevice>("SimpleBTDevice");

    qmlRegisterType<SDLController>("CanController", 1, 0, "SDLController");
    qmlRegisterType<BleGattClient>("CanController", 1, 0, "BLEClient");

    BleGattClient bleClient;
    SDLController* controller = &SDLController::instance();

    controller->init(&app, &bleClient);
    controller->connectToController();

    engine.rootContext()->setContextProperty("_sdlController", controller);
    engine.rootContext()->setContextProperty("_bleClient", &bleClient);




    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("CANController", "Main");

    return app.exec();
}
