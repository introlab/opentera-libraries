#include "UserComManager.h"
#include "QtCore/qjsonobject.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonValue>
#include "UserWebAPI.h"

UserComManager::UserComManager(QObject *parent)
    : QObject{parent},
    m_networkAccessManager(nullptr),
    m_websocketManager(nullptr),
    m_refreshTokenTimer(nullptr)
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_websocketManager = new UserWebSocketManager(this);
    m_refreshTokenTimer = new QTimer(this);

    // Connect base signals
    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &UserComManager::onNetworkFinished);
#ifndef OPENTERA_WEBASSEMBLY
    connect(m_networkAccessManager, &QNetworkAccessManager::sslErrors, this, &UserComManager::onNetworkSslErrors);
    connect(m_networkAccessManager, &QNetworkAccessManager::encrypted, this, &UserComManager::onNetworkEncrypted);
#endif
    connect(m_networkAccessManager, &QNetworkAccessManager::authenticationRequired, this, &UserComManager::onNetworkAuthenticationRequired);

}

bool UserComManager::isConnected()
{
    if (m_websocketManager)
    {
        return m_websocketManager->isConnected();
    }

    return false;
}

void UserComManager::loginToServer(QString username, QString password, QString server_name)
{
    m_username = username;
    m_password = password;
    m_serverUrl = QUrl(server_name);

    login();
}

void UserComManager::login()
{
    QUrl url(m_serverUrl);
    url.setPath(WEB_LOGIN_PATH);

    QUrlQuery args;
    args.addQueryItem("with_websockets", "true");
    QNetworkReply* reply = _doGet(url, args, false);

    //Finished lambda
    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                QByteArray responseData = reply->readAll();
                qDebug() << responseData;

                QJsonParseError jsonParseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);


                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                qDebug() << "login status code: " << statusCode;

                if (statusCode.toInt() != 200)
                {
                    emit loginFailed(reply->errorString());
                }
                else if (jsonParseError.error == QJsonParseError::NoError) {
                    if (jsonResponse.isObject())
                    {
                        QJsonObject jsonObject = jsonResponse.object();
                        //Access specific fields heres
                        m_token = jsonObject["user_token"].toString();
                        qDebug() << "token: " << m_token;


                        QString websocketUrl = jsonObject["websocket_url"].toString();
                        qDebug() << "websocket_url: " << websocketUrl;
                        _connectWebSocket(QUrl(websocketUrl));
                        emit loginSucceeded();
                        _startRefreshTokenTimer();
                    }
                }
                else
                {
                    qDebug() << jsonParseError.errorString();
                    emit loginFailed(jsonParseError.errorString());
                }


                //Done with reply
                reply->deleteLater();
            });



}

void UserComManager::logout()
{
    QUrl url(m_serverUrl);
    url.setPath(WEB_LOGOUT_PATH);

    QUrlQuery args; // empty

    QNetworkReply* reply = _doGet(url, args, true);

    //Finished lambda
    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                //QByteArray responseData = reply->readAll();
                //qDebug() << responseData;
                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

                if (statusCode.toInt() != 200)
                {
                    qDebug() << "Cannot logout";
                    emit logoutFailed();
                    return;
                }

                // Stop timer
                m_refreshTokenTimer->stop();

                //Disconnect websocket
                m_websocketManager->close();

                //Emit signal
                emit logoutSucceeded();

                //Done with reply
                reply->deleteLater();

            });


}

void UserComManager::getOnlineParticipants()
{
    QUrl url(m_serverUrl);
    url.setPath(WEB_ONLINEPARTICIPANTINFO_PATH);

    QUrlQuery args; // empty

    QNetworkReply* reply = _doGet(url, args, true);

    //Finished lambda
    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                QByteArray responseData = reply->readAll();
                //qDebug() << responseData;
                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

                if (statusCode.toInt() != 200)
                {
                    qDebug() << "Cannot get online participants";
                    return;
                }

                QJsonParseError jsonParseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);
                if (jsonParseError.error == QJsonParseError::NoError) {

                    QJsonArray jsonArray = jsonResponse.array();
                    //qDebug() << "onlineParticipants" << jsonArray;
                    for (auto i = 0; i < jsonArray.size(); i++)
                    {
                        /*
                         * SAMPLE
                        [
                            {
                            "id_participant\": 2,
                            "id_participant_group": null,
                            "id_project": 1,
                            "participant_email": null,
                            "participant_enabled": true,
                            "participant_name": "P2",
                            "participant_token_enabled": true,
                            "participant_uuid": "9dcec88e-a6e0-45fb-a9f5-eeaf747af538",
                            "participant_online": true,
                            "participant_busy": false
                            }
                        ]
                        */

                        QJsonValue jsonValue = jsonArray.at(i);
                        auto jsonObject = jsonValue.toObject();
                        int id_participant = jsonObject["id_participant"].toInt();
                        int id_participant_group = jsonObject["id_participant_group"].toInt();
                        int id_project = jsonObject["id_project"].toInt();
                        QString participant_email = jsonObject["participant_email"].toString();
                        QString participant_name = jsonObject["participant_name"].toString("Anonymous");
                        bool participant_token_enabled = jsonObject["participant_token_enabled"].toBool();
                        QString participant_uuid = jsonObject["participant_uuid"].toString();
                        bool participant_online = jsonObject["participant_online"].toBool();
                        bool participant_busy = jsonObject["participant_busy"].toBool();

                        emit participantOnline(id_participant, id_participant_group, id_project,
                                               participant_email, participant_name, participant_token_enabled, participant_uuid, participant_online, participant_busy);
                    }
                }

            });

}

void UserComManager::getOnlineDevices()
{
    QUrl url(m_serverUrl);
    url.setPath(WEB_ONLINEDEVICEINFO_PATH);

    QUrlQuery args; // empty

    QNetworkReply* reply = _doGet(url, args, true);

    //Finished lambda
    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                QByteArray responseData = reply->readAll();
                //qDebug() << responseData;
                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

                if (statusCode.toInt() != 200)
                {
                    qDebug() << "Cannot get online robots";
                    return;
                }

                QJsonParseError jsonParseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);
                if (jsonParseError.error == QJsonParseError::NoError) {

                    QJsonArray jsonArray = jsonResponse.array();
                    qDebug() << "onlineDevices" << jsonArray;
                    for (auto i = 0; i < jsonArray.size(); i++)
                    {
                        /*
                         * SAMPLE
                        [
                            {
                                "device_busy":false,
                                "device_enabled":true,
                                "device_name":"T-TOPv4-Blanc",
                                "device_online":true,
                                "device_status":{"busy":false,"online":true,"status":{"status":"CUSTOM JSON STRING BY DEVICE","timestamp":1694030867.759326}},
                                "device_uuid":"0a0f0245-afe6-44b7-9f0d-cf93d072da41",
                                "id_device":26,
                                "id_device_subtype":null,
                                "id_device_type":8
                            }
                        ]
                        */

                        QJsonValue jsonValue = jsonArray.at(i);
                        auto jsonObject = jsonValue.toObject();

                        bool device_busy = jsonObject["device_busy"].toBool();
                        bool device_enable = jsonObject["device_enable"].toBool();
                        QString device_name = jsonObject["device_name"].toString();
                        bool device_online = jsonObject["device_online"].toBool();
                        // Structure a little complicated
                        auto jsonStatusObject = jsonObject["device_status"].toObject()["status"].toObject();
                        qint64 timestamp = jsonStatusObject["timestamp"].toInteger();
                        QString status_string = jsonStatusObject["status"].toString();
                        QString device_uuid = jsonObject["device_uuid"].toString();
                        int id_device = jsonObject["id_device"].toInt();
                        int id_device_subtype = jsonObject["id_device_subtype"].toInt(-1);
                        int id_device_type =  jsonObject["id_device_type"].toInt();

                        emit deviceOnline(device_busy, device_enable, device_name, device_online,
                                          timestamp, status_string, device_uuid, id_device, id_device_subtype, id_device_type);

                    }
                }

            });
}

void UserComManager::getSessionTypes()
{
    QUrl url(m_serverUrl);
    url.setPath(WEB_SESSIONTYPE_PATH);

    QUrlQuery args;

    QNetworkReply* reply = _doGet(url, args, true);

    //Finished lambda
    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                QByteArray responseData = reply->readAll();
                //qDebug() << responseData;
                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

                if (statusCode.toInt() != 200)
                {
                    qDebug() << "Cannot get online participants";
                    return;
                }

                QJsonParseError jsonParseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);
                if (jsonParseError.error == QJsonParseError::NoError) {

                    QJsonArray jsonArray = jsonResponse.array();
                    //qDebug() << "sessionTypes" << jsonArray;

                    for (auto i = 0; i < jsonArray.size(); i++)
                    {

                        /*
                         * SAMPLE
                        [
                            {
                                "id_service": 5,
                                "id_session_type": 5,
                                "session_type_category": 1,
                                "session_type_color": "#00FF00",
                                "session_type_config": "json_config",
                                "session_type_name": "TTOPGamesWithTablet",
                                "session_type_online": true,
                                "session_type_service_key": "TTOPGamesService",
                                "session_type_service_uuid": "b6681df8-76bc-4aa4-9107-865bb0faadd5"
                            }
                        ]"
                        */

                        QJsonValue jsonValue = jsonArray.at(i);
                        auto jsonObject = jsonValue.toObject();
                        int id_service = jsonObject["id_service"].toInt();
                        int id_session_type = jsonObject["id_session_type"].toInt();
                        int session_type_category = jsonObject["session_type_category"].toInt();
                        QString session_type_color = jsonObject["session_type_color"].toString();
                        QString session_type_name = jsonObject["session_type_name"].toString();
                        bool session_type_online = jsonObject["session_type_online"].toBool();
                        QString session_type_service_key = jsonObject["session_type_service_key"].toString();
                        QString session_type_service_uuid = jsonObject["session_type_service_uuid"].toString();

                        emit sessionTypeAvailable(id_service, id_session_type, session_type_category, session_type_color,
                                                  session_type_name, session_type_online, session_type_service_key, session_type_service_uuid);
                    }

                }

            });
}



void UserComManager::onNetworkAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply)

    qDebug() << "onNetworkAuthenticationRequired";
    authenticator->setUser(m_username);
    authenticator->setPassword(m_password);
}



void UserComManager::onNetworkFinished(QNetworkReply *reply)
{
    //qDebug() << "onNetworkFinished";
}

#ifndef OPENTERA_WEBASSEMBLY

void UserComManager::onNetworkEncrypted(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    qDebug() << "Network Encrypted";
}


void UserComManager::onNetworkSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << "onNetworkSslErrors - Ignore SSL errors (certificate)";
    reply->ignoreSslErrors();
    for(const QSslError &error : errors){
        qDebug() << error;
    }

}
#endif

void UserComManager::onRefreshTokenTimeout()
{
    qDebug() << "onRefreshTokenTimeout";
    if (m_token.size() > 0)
    {
        _refreshToken();
    }
}

QNetworkReply* UserComManager::_doPost(const QUrl &url, const QUrlQuery &query_args, const QString &post_data, bool use_token)
{
    QUrl query = url;
    if (!query_args.isEmpty())
    {
        query.setQuery(query_args);
    }

    QNetworkRequest request(query);
    _setRequestCredentials(request, use_token);
    _setRequestLanguage(request);
    _setRequestVersions(request);

    request.setRawHeader("Content-Type", "application/json");

    return m_networkAccessManager->post(request, post_data.toUtf8());
}

QNetworkReply* UserComManager::_doGet(const QUrl &url, const QUrlQuery &query_args, bool use_token)
{

    QUrl query = url;
    if (!query_args.isEmpty())
    {
        query.setQuery(query_args);
    }

    QNetworkRequest request(query);
    _setRequestCredentials(request, use_token);
    _setRequestLanguage(request);
    _setRequestVersions(request);

    return m_networkAccessManager->get(request);
}

void UserComManager::_setRequestLanguage(QNetworkRequest &request)
{
    //Locale will be initialized with default locale
    QString localeString = QLocale().bcp47Name();
    //qDebug() << "localeString : " << localeString;
    request.setRawHeader(QByteArray("Accept-Language"), localeString.toUtf8());
}

void UserComManager::_setRequestCredentials(QNetworkRequest &request, const bool &use_token)
{
    //Needed?
    request.setAttribute(QNetworkRequest::AuthenticationReuseAttribute, false);

    // Pack in credentials
    QString headerData;
    if (!use_token){
        //qDebug() << "NOT Using token";
        QString concatenatedCredentials = m_username + ":" + m_password;
        QByteArray data = concatenatedCredentials.toLocal8Bit().toBase64();
        headerData = "Basic " + data;
    }else{
        //qDebug() << "Using token";
        headerData = "OpenTera " + m_token;
    }

    // Pack in credentials
    request.setRawHeader( "Authorization", headerData.toLocal8Bit());

}

void UserComManager::_setRequestVersions(QNetworkRequest &request)
{
    //TODO set OS NAME and VERSION
    //request.setRawHeader("X-Client-Name", QByteArray("UserComManager"));
    //request.setRawHeader("X-Client-Version", QByteArray("1.0"));
    //request.setRawHeader("X-OS-Name", QString("Unknown").toUtf8());
    //request.setRawHeader("X-OS-Version", QString("Unknown").toUtf8());
}

void UserComManager::_startRefreshTokenTimer()
{
    if (m_refreshTokenTimer)
    {
        connect(m_refreshTokenTimer, &QTimer::timeout, this, &UserComManager::onRefreshTokenTimeout);
        m_refreshTokenTimer->start(1000 * 60 * 15); // 15 minutes
    }
}

void UserComManager::_stopRefreshTokenTimer()
{
    if (m_refreshTokenTimer)
    {
        disconnect(m_refreshTokenTimer, &QTimer::timeout, this, &UserComManager::onRefreshTokenTimeout);
        m_refreshTokenTimer->stop();
    }
}

void UserComManager::_refreshToken()
{
    QUrl url(m_serverUrl);
    url.setPath(WEB_REFRESH_TOKEN_PATH);
    QUrlQuery args;
    QNetworkReply* reply = _doGet(url, args, true);

    //Finished lambda
    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                QByteArray responseData = reply->readAll();
                qDebug() << responseData;

                QJsonParseError jsonParseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);


                QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                qDebug() << "_refreshToken status code: " << statusCode;

                if (statusCode.toInt() != 200)
                {
                    qDebug() << "error refreshing token";
                }
                else if (jsonParseError.error == QJsonParseError::NoError) {
                    if (jsonResponse.isObject())
                    {
                        QJsonObject jsonObject = jsonResponse.object();
                        //Access specific fields heres
                        m_token = jsonObject["refresh_token"].toString();
                        emit tokenRefreshed(m_token);
                    }
                }
                else
                {
                    qDebug() << jsonParseError.errorString();
                }


                //Done with reply
                reply->deleteLater();
            });

}

void UserComManager::_connectWebSocket(const QUrl &url)
{
    m_websocketManager->connectTo(url);
}
