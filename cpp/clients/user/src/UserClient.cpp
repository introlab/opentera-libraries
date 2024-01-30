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

    QObject::connect(m_comManager, &UserComManager::onlineParticipants, this, &UserClient::protectedOnlineParticipants);
    QObject::connect(m_comManager, &UserComManager::onlineDevices, this, &UserClient::onlineDevices);
    QObject::connect(m_comManager, &UserComManager::onlineUsers, this, &UserClient::onlineUsers);
    QObject::connect(m_comManager, &UserComManager::sessionTypes, this, &UserClient::sessionTypes);

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
/*
void UserClient::getOnlineParticipants(std::function<void (const QVariant &)> &callback)
{

}
*/

void UserClient::getOnlineParticipants(QObject *caller)
{
    qDebug() << "getOnlineParticipants with caller " << caller;
    m_comManager->getOnlineParticipants(caller);
}

QNetworkReplyWrapper *UserClient::get(const QString &endpoint, const QVariantMap &params, const QVariantMap &extra_headers)
{
    QNetworkReply *reply = m_comManager->get(endpoint, params, extra_headers);
    return new QNetworkReplyWrapper(reply);
}


void UserClient::getOnlineUsers()
{
    m_comManager->getOnlineUsers();
}

void UserClient::getOnlineDevices()
{
    m_comManager->getOnlineDevices();
}

void UserClient::getSessionTypes()
{
    m_comManager->getSessionTypes();
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

void UserClient::protectedOnlineParticipants(QObject *caller, const QVariant &results)
{

    qDebug() << "C++ UserClient::protectedOnlineParticipants" << caller;

    QObject *signal_sender = sender();

    // Emit specific signal to caller
    if (caller)
    {
        Qt::ConnectionType connectionType =
            (QThread::currentThread() == caller->thread()) ? Qt::DirectConnection : Qt::QueuedConnection;


        qDebug() << "UserClient::protectedOnlineParticipants calling specific" << caller;
        QMetaObject::invokeMethod(caller,        // pointer to a QObject
                                  "onOnlineParticipantsAnswer",       // member name (no parameters here)
                                  connectionType,     // connection type
                                  //Q_ARG(QObject*, caller),
                                  Q_ARG(QVariant, results));     // parameters




    }

}
