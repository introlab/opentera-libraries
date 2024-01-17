#include "DeviceComManager.h"
#include "DeviceWebApi.h"

DeviceComManager::DeviceComManager(QUrl serverUrl, QString device_static_token, QObject *parent)
    : BaseComManager{serverUrl, parent}
{
    setDeviceStaticToken(device_static_token);
}

void DeviceComManager::setDeviceStaticToken(const QString &token)
{
    m_deviceStaticToken = token;
}

void DeviceComManager::registerDeviceForTokenAccess(const QString &device_name, const int &id_device_type, const int &id_device_subtype)
{
    QJsonObject base_obj;
    QJsonObject device_obj;
    device_obj["device_name"] = device_name;
    device_obj["id_device_type"] = id_device_type;
    if (id_device_subtype > 0)
        device_obj["id_device_subtype"] = id_device_subtype;
    base_obj["device_info"] = device_obj;

    QJsonDocument doc;
    doc.setObject(base_obj);

    doPost(URL_DEVICE_REGISTER, doc.toJson(QJsonDocument::Compact));

}

bool DeviceComManager::processDeviceRegisterReply(const QString &reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());

    return false;

}

bool DeviceComManager::processNetworkReply(QNetworkReply *reply)
{
    QString reply_path = reply->url().path();
    QString reply_data = reply->readAll();
    QUrlQuery reply_query = QUrlQuery(reply->url().query());

    if (reply->operation()==QNetworkAccessManager::PostOperation){
        if (reply_path == URL_DEVICE_REGISTER){
            return processDeviceRegisterReply(reply_data);
        }
    }
    return false;
}
