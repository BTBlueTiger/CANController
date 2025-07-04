#ifndef BLE_GATT_CLIENT_H
#define BLE_GATT_CLIENT_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QVariantList>
#include <QDebug>

#include "SimpleBTDevice.h"




class BleGattClient : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged FINAL)
    Q_PROPERTY(bool isDeviceConnected READ isDeviceConnected NOTIFY isDeviceConnectedChanged FINAL)
    Q_PROPERTY(QList<SimpleBTDevice> devices READ devices NOTIFY devicesChanged FINAL)

public:

    explicit BleGattClient(QObject *parent = nullptr);
    ~BleGattClient();

    Q_INVOKABLE void startScan();
    Q_INVOKABLE void connectToDevice(const QBluetoothDeviceInfo &device);
    Q_INVOKABLE void sendCommand();

    QList<SimpleBTDevice>  devices() const;;
    bool isScanning() const;
    bool isDeviceConnected() const;

signals:
    void isScanningChanged();
    void isDeviceConnectedChanged();
    void onChanged();
    void devicesChanged();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void deviceConnected();
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState state);
    void characteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void deviceDisconnected();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller;
    QLowEnergyService *service;
    QLowEnergyCharacteristic ledCharacteristic;

    QList<SimpleBTDevice> mDevices;
    bool mIsScanning = false;
    bool mOn = false;
};

#endif // BLE_GATT_CLIENT_H
