#ifndef USER_CLIENT_H
#define USER_CLIENT_H

#include "QtQmlIntegration/qqmlintegration.h"
#include <QObject>
#include "UserComManager.h"
#include "QNetworkReplyWrapper.h"
#include <QDebug>


class UserClient : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QUrl url READ getServerUrl WRITE setServerUrl NOTIFY serverUrlChanged)

public:

    explicit UserClient(QObject *parent = nullptr);
    virtual ~UserClient() override;

    Q_INVOKABLE void connect(const QUrl &url, const QString &username, const QString &password);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE bool isConnected();


    Q_INVOKABLE QNetworkReplyWrapper* get(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());


    //Getters and Setters
    void setUsername(const QString &username);
    QString getUsername();
    void setPassword(const QString &password);
    QString getPassword();
    void setServerUrl(const QUrl &url);
    QUrl getServerUrl();

signals:
    // Special signals for login/logout
    void loginSucceeded();
    void loginFailed(const QString &errorMessage);
    void logoutSucceeded();
    void logoutFailed();

    // Properties
    void usernameChanged();
    void passwordChanged();
    void serverUrlChanged();

protected:

    UserComManager *m_comManager;
};


#endif
