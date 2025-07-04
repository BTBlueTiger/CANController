#ifndef SIMPLEBTDEVICE_H
#define SIMPLEBTDEVICE_H

#include <QString>
#include <qobjectdefs.h>

struct SimpleBTDevice {
    Q_GADGET

    Q_PROPERTY(QString name MEMBER m_name FINAL)
    Q_PROPERTY(QString address MEMBER m_address FINAL)

public:
    QString m_name;
    QString m_address;

    SimpleBTDevice(QString name = QString(), QString address = QString())
        : m_name(name), m_address(address) {}
};

#endif // SIMPLEBTDEVICE_H
