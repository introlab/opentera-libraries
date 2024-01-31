#include "UserClient.h"
#include <QThread>

UserClient::UserClient(QObject *parent)
    :   m_comManager(nullptr)
{
    m_comManager = new UserComManager(this);

    QObject::connect(m_comManager, &UserComManager::loginSucceeded, this, &UserClient::loginSucceeded);
    QObject::connect(m_comManager, &UserComManager::loginFailed, this, &UserClient::loginFailed);
    QObject::connect(m_comManager, &UserComManager::logoutSucceeded, this, &UserClient::logoutSucceeded);
    QObject::connect(m_comManager, &UserComManager::logoutFailed, this, &UserClient::logoutFailed);

}

UserClient::~UserClient()
{
    if (isConnected())
    {
        disconnect();
    }
}

void UserClient::connect(const QUrl &url, const QString &username, const QString &password)
{
    qDebug() << "Connecting to server" << url.toString();
    m_comManager->loginToServer(username, password, url.toString());
}

void UserClient::disconnect() {
    qDebug() << "Disconnecting from server";
    m_comManager->logout();
}

bool UserClient::isConnected() {
    return m_comManager->isConnected();
}

void UserClient::setUsername(const QString &username)
{
    m_comManager->setUsername(username);
}

QString UserClient::getUsername()
{
    return m_comManager->getUsername();
}

void UserClient::setPassword(const QString &password)
{
    m_comManager->setPassword(password);
}

QString UserClient::getPassword()
{
    return m_comManager->getPassword();
}

void UserClient::setServerUrl(const QUrl &url)
{
    m_comManager->setServerUrl(url);
}

QUrl UserClient::getServerUrl()
{
    return m_comManager->getServerUrl();
}

QNetworkReplyWrapper* UserClient::get(const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->get(endpoint, params, extra_headers);
    return new QNetworkReplyWrapper(reply);
}
