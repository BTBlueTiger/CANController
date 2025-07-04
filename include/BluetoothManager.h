#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothSocket>

class BluetoothManager : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothManager(QObject *parent = nullptr);

    Q_INVOKABLE void startDiscovery();
    Q_INVOKABLE void connectToDevice(const QString &deviceAddress);
    Q_INVOKABLE void sendMessage(const QString &message);

signals:
    void deviceDiscovered(const QString &deviceName, const QString &deviceAddress);
    void discoveryFinished();
    void deviceConnected();
    void deviceDisconnected();
    void messageSent();
    void errorOccurred(const QString &error);

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onDiscoveryFinished();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QBluetoothSocket::SocketError error);

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothSocket *m_socket;
};

#endif // BLUETOOTHMANAGER_H