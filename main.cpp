#include "include/BleGattClient.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qRegisterMetaType<SimpleBTDevice>("SimpleBTDevice");

    qmlRegisterType<BleGattClient>("CANController", 1, 0, "BLEClient");
    
    BleGattClient bleClient;
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
