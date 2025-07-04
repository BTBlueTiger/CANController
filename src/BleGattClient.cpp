#include "../include/BleGattClient.h"

BleGattClient::BleGattClient(QObject *parent)
    : QObject(parent),
      discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this))
{
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleGattClient::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, [this]() {
        mIsScanning = false;
        emit isScanningChanged();
    });
}

BleGattClient::~BleGattClient()
{
    if (controller) {
        controller->disconnectFromDevice(); // Explicitly disconnect from the BLE device
        delete controller; // Clean up the controller object
    }
    delete discoveryAgent; // Clean up the discovery agent
}

void BleGattClient::startScan()
{
    mDevices.clear();
    mIsScanning = true;
    emit isScanningChanged();
    discoveryAgent->start();
}

void BleGattClient::connectToDevice(const QString &deviceAdress)
{
    qDebug() << "Trying to connect";
    auto it = std::find_if(vecDeviceInfos.begin(), vecDeviceInfos.end(), [&](const QBluetoothDeviceInfo &d) {
        return d.address().toString() == deviceAdress;
    });

    if(it == vecDeviceInfos.end()) {
        qDebug() << "Device cant be found";
        return;
    }

    controller = QLowEnergyController::createCentral(*it, this);
    connect(controller, &QLowEnergyController::connected, this, &BleGattClient::deviceConnected);
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &BleGattClient::serviceDiscovered);
    connect(controller, &QLowEnergyController::disconnected, this, &BleGattClient::deviceDisconnected);
    controller->connectToDevice();
}

void BleGattClient::sendCommand()
{
    if (ledCharacteristic.isValid()) {
        QByteArray value;
        value.append(mOn ? 0 : 1); // 1 for "on", 0 for "off"
        service->writeCharacteristic(ledCharacteristic, value, QLowEnergyService::WriteWithoutResponse);
        qDebug() << "Command sent:" << (!mOn ? "ON" : "OFF");
        mOn = !mOn;
        emit onChanged();
    } else {
        qDebug() << "LED characteristic not found.";
    }
}

void BleGattClient::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found device:" << device.name() << device.address().toString();
    SimpleBTDevice simpleBTDevice(device.name(), device.address().toString());
    mDevices.append(simpleBTDevice);
    vecDeviceInfos.emplace_back(device);
    emit devicesChanged();
}
void BleGattClient::deviceConnected()
{
    qDebug() << "Device connected.";
    controller->discoverServices();
    emit isDeviceConnectedChanged();
}

void BleGattClient::serviceDiscovered(const QBluetoothUuid &uuid)
{
    qDebug() << "Service discovered:" << uuid.toString();
    service = controller->createServiceObject(uuid, this);
    if (service) {
        connect(service, &QLowEnergyService::stateChanged, this, &BleGattClient::serviceStateChanged);
        connect(service, &QLowEnergyService::characteristicRead, this, &BleGattClient::characteristicRead);
        service->discoverDetails();
    }
}

void BleGattClient::serviceScanDone()
{
    qDebug() << "Service scan done.";
    const auto services = controller->services();
    for (const auto &serviceUuid : services) {
        QLowEnergyService *service = controller->createServiceObject(serviceUuid, this);
        if (service) {
            connect(service, &QLowEnergyService::stateChanged, this, &BleGattClient::serviceStateChanged);
            service->discoverDetails();
        }
    }
}

void BleGattClient::serviceStateChanged(QLowEnergyService::ServiceState state)
{
    if (state == QLowEnergyService::RemoteServiceDiscovered) {
        qDebug() << "Service discovered.";
        const auto characteristics = service->characteristics();
        for (const auto &characteristic : characteristics) {
            qDebug() << "Characteristic:" << characteristic.uuid().toString();
            service->readCharacteristic(characteristic);
        }
    }
}

void BleGattClient::characteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << "Characteristic read:" << characteristic.uuid().toString();
    qDebug() << "Value:" << value;

    QString descriptorValue = QString::fromUtf8(value);
    qDebug() << "Descriptor Value:" << descriptorValue;
}

void BleGattClient::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << "Characteristic changed:" << characteristic.uuid().toString() << value;
}

void BleGattClient::deviceDisconnected()
{
    qDebug() << "Device disconnected.";
    emit isDeviceConnectedChanged();
}

bool BleGattClient::isScanning() const
{
    return mIsScanning;
}

bool BleGattClient::isDeviceConnected() const
{
    return controller && controller->state() == QLowEnergyController::ConnectedState;
}

QList<SimpleBTDevice>  BleGattClient::devices() const
{
    return mDevices;
}
