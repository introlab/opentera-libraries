#ifndef DEVICECLIENT_H
#define DEVICECLIENT_H

#include <QObject>

class DeviceClient : public QObject
{
    Q_OBJECT
public:
    explicit DeviceClient(QObject *parent = nullptr);

signals:
};

#endif // DEVICECLIENT_H
