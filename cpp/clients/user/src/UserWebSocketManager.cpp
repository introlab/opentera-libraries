#include "UserWebSocketManager.h"
#include <QDebug>

UserWebSocketManager::UserWebSocketManager(bool verify_ssl, QObject *parent)
    : QObject(parent), m_websocket(nullptr), m_verifySsl(verify_ssl)
{
    m_websocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest,parent);
    connect(m_websocket, &QWebSocket::binaryMessageReceived, this, &UserWebSocketManager::onBinaryMessageReceived);
    connect(m_websocket, &QWebSocket::textMessageReceived, this, &UserWebSocketManager::onTextMessageReceived);
#ifndef OPENTERA_WEBASSEMBLY
    connect(m_websocket, &QWebSocket::sslErrors, this, &UserWebSocketManager::onSslErrors);
#endif
    connect(m_websocket, &QWebSocket::connected, this, &UserWebSocketManager::onConnected);
    connect(m_websocket, &QWebSocket::disconnected, this, &UserWebSocketManager::onDisconnected);
}

void UserWebSocketManager::connectTo(const QUrl &url)
{
    qDebug() << "UserWebSocketManager::connectTo " << url;
    m_websocket->open(url);
}

void UserWebSocketManager::close()
{
    if (m_websocket)
    {
        m_websocket->close();
    }
}

bool UserWebSocketManager::isConnected()
{
    if (m_websocket)
    {
        return m_websocket->state() == QAbstractSocket::ConnectedState;
    }
    return false;
}

void UserWebSocketManager::onBinaryMessageReceived(const QByteArray &message)
{
    qDebug() << "onBinaryMessageReceived" << message;
}

void UserWebSocketManager::onTextMessageReceived(const QString &message)
{
    qDebug() << "onTextMessageReceived" << message;
}

#ifndef OPENTERA_WEBASSEMBLY
void UserWebSocketManager::onSslErrors(const QList<QSslError> &errors)
{
    if(m_websocket)
    {
        if (!m_verifySsl)
        {
            m_websocket->ignoreSslErrors();
        }
        else {
            qDebug() << "UserWebSocketManager::onSslErrors" << errors;
        }

    }
}
#endif

void UserWebSocketManager::onConnected()
{

}

void UserWebSocketManager::onDisconnected()
{

}
