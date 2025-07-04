#include "include/BluetoothManager.h"
#include <QDebug>

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent),
      m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
      m_socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this))
{
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothManager::onDeviceDiscovered);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothManager::onDiscoveryFinished);
    connect(m_socket, &QBluetoothSocket::connected, this, &BluetoothManager::onConnected);
    connect(m_socket, &QBluetoothSocket::disconnected, this, &BluetoothManager::onDisconnected);
    connect(m_socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::errorOccurred),
            this, &BluetoothManager::onErrorOccurred);
}

void BluetoothManager::startDiscovery()
{
    m_discoveryAgent->start();
}

void BluetoothManager::connectToDevice(const QString &deviceAddress)
{
    if (m_socket->isOpen()) {
        m_socket->close();
    }

    m_socket->connectToService(QBluetoothAddress(deviceAddress), 1); // Replace '1' with your ESP's RFCOMM channel
}

void BluetoothManager::sendMessage(const QString &message)
{
    if (m_socket->isOpen()) {
        m_socket->write(message.toUtf8());
        emit messageSent();
    } else {
        emit errorOccurred("Socket is not connected.");
    }
}

void BluetoothManager::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    emit deviceDiscovered(device.name(), device.address().toString());
}

void BluetoothManager::onDiscoveryFinished()
{
    emit discoveryFinished();
}

void BluetoothManager::onConnected()
{
    qDebug() << "Connected to device.";
    emit deviceConnected();
}

void BluetoothManager::onDisconnected()
{
    qDebug() << "Disconnected from device.";
    emit deviceDisconnected();
}

void BluetoothManager::onErrorOccurred(QBluetoothSocket::SocketError error)
{
    qDebug() << "Bluetooth error:" << m_socket->errorString();
    emit errorOccurred(m_socket->errorString());
}
