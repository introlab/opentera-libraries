#ifndef USER_CLIENT_H
#define USER_CLIENT_H

#include "QtQmlIntegration/qqmlintegration.h"
#include <QObject>
#include "UserComManager.h"
#include <QDebug>

class UserClient : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)

public:

    explicit UserClient(QObject *parent = nullptr)
        :   m_comManager(nullptr)
    {
        m_comManager = new UserComManager(this);
        QObject::connect(m_comManager, &UserComManager::loginSucceeded, this, &UserClient::loginSucceeded);
        QObject::connect(m_comManager, &UserComManager::loginFailed, this, &UserClient::loginFailed);

        QObject::connect(m_comManager, &UserComManager::logoutSucceeded, this, &UserClient::logoutSucceeded);
        QObject::connect(m_comManager, &UserComManager::logoutFailed, this, &UserClient::logoutFailed);

    }

    virtual ~UserClient()
    {
        if (isConnected())
        {
            disconnect();
        }
    }

    Q_INVOKABLE void connect(const QUrl &url, const QString &username, const QString &password)
    {
        qDebug() << "Connecting to server" << url.toString();
        m_comManager->loginToServer(username, password, url.toString());
    }

    Q_INVOKABLE void disconnect() {
        qDebug() << "Disconnecting from server";
        m_comManager->logout();
    }

    Q_INVOKABLE bool isConnected() {
        return m_comManager->isConnected();
    }

    void setUsername(const QString &username)
    {
        m_comManager->setUsername(username);
    }

    QString getUsername()
    {
        return m_comManager->getUsername();
    }

signals:
    void loginSucceeded();
    void loginFailed(const QString &errorMessage);
    void logoutSucceeded();
    void logoutFailed();
    void usernameChanged();
    void passwordChanged();


protected:

    UserComManager *m_comManager;

};


#endif
