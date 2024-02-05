#include "UserComManager.h"
#include "QtCore/qjsonobject.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonValue>
#include "UserWebAPI.h"

UserComManager::UserComManager(bool verify_ssl, QObject *parent)
    : QObject{parent},
    m_networkAccessManager(nullptr),
    m_websocketManager(nullptr),
    m_refreshTokenTimer(nullptr),
    m_verifySsl{verify_ssl},
    m_clientName("UserComManager"),
    m_clientVersion("1.0.0")
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_websocketManager = new UserWebSocketManager(m_verifySsl, this);
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

QNetworkReply* UserComManager::get(const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QUrl url(m_serverUrl);
    url.setPath(endpoint);

    // Fill query params
    QUrlQuery query_params;
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        query_params.addQueryItem(it.key(), it.value().toString());
    }

    // Convert extra_headers to QMap<QString, QString>
    QMap<QString, QString> extra_headers_map;
    for (auto it = extra_headers.begin(); it != extra_headers.end(); ++it) {
        extra_headers_map.insert(it.key(), it.value().toString());
    }


    return _doGet(url, query_params, extra_headers_map, true);
}

QNetworkReply *UserComManager::post(const QString &endpoint, const QVariantMap &params, const QVariantMap &data, const QVariantMap &extra_headers)
{
    QUrl url(m_serverUrl);
    url.setPath(endpoint);

    // Fill query params
    QUrlQuery query_params;
    for (auto it = params.begin(); it != params.end(); ++it)
    {
        query_params.addQueryItem(it.key(), it.value().toString());
    }

    // Convert extra_headers to QMap<QString, QString>
    QMap<QString, QString> extra_headers_map;
    for (auto it = extra_headers.begin(); it != extra_headers.end(); ++it) {
        extra_headers_map.insert(it.key(), it.value().toString());
    }

    // Converting QVariantMap to QIODevice
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(data);

    //Post
    return _doPost(url, query_params, jsonDocument.toJson(), extra_headers_map, true);
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
    url.setPath(UserWebAPI::ENDPOINT_USER_LOGIN);

    QUrlQuery args;
    args.addQueryItem("with_websockets", "true");
    QNetworkReply* reply = _doGet(url, args, QMap<QString, QString>(), false);

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
                    QString error = reply->errorString();
                    if (statusCode.toInt() == 401)
                        error = tr("Invalid username or password.");
                    emit loginFailed(error);
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
    url.setPath(UserWebAPI::ENDPOINT_USER_LOGOUT);

    QUrlQuery args; // empty

    QNetworkReply* reply = _doGet(url, args, QMap<QString, QString>(), true);

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

                //Reset token
                m_token = "";
                emit tokenRefreshed(m_token);

                //Emit signal
                emit logoutSucceeded();

                //Done with reply
                reply->deleteLater();

            });
}

void UserComManager::setToken(const QString &token)
{
    m_token = token;
    emit tokenRefreshed(m_token);
}

void UserComManager::setClientName(const QString &client_name)
{
    m_clientName = client_name;
}

void UserComManager::setClientVersion(const QString &client_version)
{
    m_clientVersion = client_version;
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
    Q_UNUSED(reply)
    //qDebug() << "onNetworkFinished";
}

#ifndef OPENTERA_WEBASSEMBLY

void UserComManager::onNetworkEncrypted(QNetworkReply *reply)
{
    Q_UNUSED(reply)
    qDebug() << "Network Encrypted";
}


void UserComManager::onNetworkSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    if (!m_verifySsl)
    {
        qDebug() << "onNetworkSslErrors - Ignore SSL errors (certificate)";
        reply->ignoreSslErrors();
        return;
    }
    else
    {
        qDebug() << "onNetworkSslErrors - Verify SSL errors (certificate)" << errors;
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

QNetworkReply* UserComManager::_doPost(const QUrl &url, const QUrlQuery &query_args,
                                       const QByteArray &post_data, const QMap<QString, QString> &extra_headers, bool use_token)
{
    QUrl query = url;
    if (!query_args.isEmpty())
    {
        query.setQuery(query_args);
    }

    QNetworkRequest request(query);
    _setRequestCredentials(request, use_token);
    _setRequestExtraHeaders(request, extra_headers);
    _setRequestLanguage(request);
    _setRequestVersions(request);

    request.setRawHeader("Content-Type", "application/json");

    return m_networkAccessManager->post(request, post_data);
}

QNetworkReply* UserComManager::_doGet(const QUrl &url, const QUrlQuery &query_args,
                                      const QMap<QString, QString> &extra_headers, bool use_token)
{

    QUrl query = url;
    if (!query_args.isEmpty())
    {
        query.setQuery(query_args);
    }

    QNetworkRequest request(query);
    _setRequestExtraHeaders(request, extra_headers);
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
    request.setRawHeader("X-Client-Name", m_clientName.toUtf8());
    request.setRawHeader("X-Client-Version", m_clientName.toUtf8());
    request.setRawHeader("X-OS-Name", QString("Unknown").toUtf8());
    request.setRawHeader("X-OS-Version", QString("Unknown").toUtf8());
}

void UserComManager::_setRequestExtraHeaders(QNetworkRequest &request, QMap<QString, QString> extra_headers)
{
    //Iterate over extra headers
    QMapIterator<QString, QString> i(extra_headers);
    while (i.hasNext()) {
        i.next();
        request.setRawHeader(i.key().toUtf8(), i.value().toUtf8());
    }
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
    url.setPath(UserWebAPI::ENDPOINT_USER_REFRESH_TOKEN);
    QUrlQuery args;
    QNetworkReply* reply = _doGet(url, args, QMap<QString, QString>(), true);

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
