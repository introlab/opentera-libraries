#include "ParticipantClient.h"
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>

#ifdef OPENTERA_WEBASSEMBLY
#include <emscripten.h>
#endif

ParticipantClient::ParticipantClient(QObject *parent)
    :   QObject(parent), m_comManager(nullptr)
{
    //TODO Change not verifying ssl for now
    m_comManager = new ParticipantComManager(false, this);

    QObject::connect(m_comManager, &ParticipantComManager::loginSucceeded, this, &ParticipantClient::loginSucceeded);
    QObject::connect(m_comManager, &ParticipantComManager::loginFailed, this, &ParticipantClient::loginFailed);
    QObject::connect(m_comManager, &ParticipantComManager::logoutSucceeded, this, &ParticipantClient::logoutSucceeded);
    QObject::connect(m_comManager, &ParticipantComManager::logoutFailed, this, &ParticipantClient::logoutFailed);
    QObject::connect(m_comManager, &ParticipantComManager::tokenRefreshed, this, &ParticipantClient::tokenChanged);

    // Websocket signals
    QObject::connect(m_comManager, &ParticipantComManager::websocketConnected, this, &ParticipantClient::websocketConnected);
    QObject::connect(m_comManager, &ParticipantComManager::websocketDisconnected, this, &ParticipantClient::websocketDisconnected);
    QObject::connect(m_comManager, &ParticipantComManager::deviceEvent, this, &ParticipantClient::deviceEvent);
    QObject::connect(m_comManager, &ParticipantComManager::joinSessionEvent, this, &ParticipantClient::joinSessionEvent);
    QObject::connect(m_comManager, &ParticipantComManager::joinSessionReplyEvent, this, &ParticipantClient::joinSessionReplyEvent);
    QObject::connect(m_comManager, &ParticipantComManager::leaveSessionEvent, this, &ParticipantClient::leaveSessionEvent);
    QObject::connect(m_comManager, &ParticipantComManager::participantEvent, this, &ParticipantClient::participantEvent);
    QObject::connect(m_comManager, &ParticipantComManager::stopSessionEvent, this, &ParticipantClient::stopSessionEvent);

}

ParticipantClient::~ParticipantClient()
{
    if (isConnected())
    {
        disconnect();
    }
}

void ParticipantClient::connect(const QUrl &url, const QString &username, const QString &password)
{
    qDebug() << "Connecting to server" << url.toString();
    m_comManager->loginToServer(username, password, url.toString());
}

void ParticipantClient::connectWithToken(const QUrl &url, const  QString& staticToken, const bool& withWebsocket)
{
    qDebug() << "Connecting to server" << url.toString();
    m_comManager->loginWithToken(staticToken, url.toString(), withWebsocket);
}


void ParticipantClient::disconnect() {
    qDebug() << "Disconnecting from server";
    m_comManager->logout();
}

bool ParticipantClient::isConnected() {
    return m_comManager->isConnected();
}

bool ParticipantClient::isWebAssembly()
{
#ifdef OPENTERA_WEBASSEMBLY
    return true;
#else
    return false;
#endif
}

void ParticipantClient::setUsername(const QString &username)
{
    m_comManager->setUsername(username);
}

QString ParticipantClient::getUsername()
{
    return m_comManager->getUsername();
}

void ParticipantClient::setPassword(const QString &password)
{
    m_comManager->setPassword(password);
}

QString ParticipantClient::getPassword()
{
    return m_comManager->getPassword();
}

void ParticipantClient::setServerUrl(const QUrl &url)
{
    m_comManager->setServerUrl(url);
}

QUrl ParticipantClient::getServerUrl()
{
    return m_comManager->getServerUrl();
}

void ParticipantClient::setToken(const QString &token)
{
    m_comManager->setToken(token);
}

QString ParticipantClient::getToken()
{
    return m_comManager->getToken();
}

void ParticipantClient::setClientName(const QString &clientName)
{
    m_comManager->setClientName(clientName);
}

QString ParticipantClient::getClientName()
{
    return m_comManager->getClientName();
}

void ParticipantClient::setClientVersion(const QString &clientVersion)
{
    m_comManager->setClientVersion(clientVersion);
}

QString ParticipantClient::getClientVersion()
{
    return m_comManager->getClientVersion();
}

QNetworkReplyWrapper* ParticipantClient::get(const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->get(endpoint, params, extra_headers);
    return new QNetworkReplyWrapper(reply);
}

QNetworkReplyWrapper *ParticipantClient::post(const QString &endpoint, const QVariantMap &params, const QVariantMap &data, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->post(endpoint, params, data, extra_headers);
    return new QNetworkReplyWrapper(reply);
}

QNetworkReplyWrapper *ParticipantClient::deleteResource(const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->deleteResource(endpoint, params, extra_headers);
    return new QNetworkReplyWrapper(reply);
}

QNetworkReplyWrapper *ParticipantClient::download(const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->download(endpoint, params, extra_headers);
    return new QNetworkReplyWrapper(reply, false);
}

#ifndef OPENTERA_WEBASSEMBLY
FileDownloader *ParticipantClient::downloadFile(const QString &filePath, const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->download(endpoint, params, extra_headers);
    return new FileDownloader(filePath, new QNetworkReplyWrapper(reply, false));
}
#else
FileDownloader* ParticipantClient::downloadFile(const QString &filePath, const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    // Add filename to json
    QJsonDocument document = m_comManager->downloadDocumentJson(endpoint, params, extra_headers);
    QJsonObject rootObject = document.object();
    rootObject.insert("filename", filePath);
    document.setObject(rootObject);

    EM_ASM({ window.parent.fileDownloadFromBrowser(UTF8ToString($0)); },
           document.toJson().toStdString().c_str()
           );

    return nullptr;
}

#endif
