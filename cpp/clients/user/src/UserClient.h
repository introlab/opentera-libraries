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

public:

    explicit UserClient(QObject *parent = nullptr)
        :   m_comManager(nullptr)
    {
        m_comManager = new UserComManager(this);
    }

    virtual ~UserClient()
    {
        //TODO Close connection

    }

    Q_INVOKABLE void connect(const QUrl &url, const QString &username, const QString &password)
    {
        qDebug() << "Connecting to " << url.path();
        m_comManager->loginToServer(username, password, url.path());
    }

    Q_INVOKABLE bool isConnected() {
        return false;
    }


protected:

    UserComManager *m_comManager;

};


#endif
