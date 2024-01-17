#ifndef DEVICECOMMANAGER_H
#define DEVICECOMMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "BaseComManager.h"

class DeviceComManager : public BaseComManager
{
    Q_OBJECT
public:
    explicit DeviceComManager(QUrl serverUrl, QString device_static_token = QString(), QObject *parent = nullptr);

    void setDeviceStaticToken(const QString& token);

    void registerDeviceForTokenAccess(const QString& device_name, const int& id_device_type, const int& id_device_subtype = -1);

private:
    bool processDeviceRegisterReply(const QString& reply);

    QString m_deviceStaticToken;

protected:
    bool processNetworkReply(QNetworkReply* reply) override;

signals:
    void deviceRegisteredWithToken(QString token);

};

#endif // DEVICECOMMANAGER_H
