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
    Q_PROPERTY(QString token READ getToken WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(QString clientName READ getClientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString clientVersion READ getClientVersion WRITE setClientVersion NOTIFY clientVersionChanged)


public:

    explicit UserClient(QObject *parent = nullptr);
    virtual ~UserClient() override;

    Q_INVOKABLE void connect(const QUrl &url, const QString &username, const QString &password);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE bool isConnected();


    Q_INVOKABLE QNetworkReplyWrapper* get(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());
    Q_INVOKABLE QNetworkReplyWrapper* post(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &data = QVariantMap(),
                                           const QVariantMap &extra_headers = QVariantMap());
    Q_INVOKABLE QNetworkReplyWrapper* deleteResource(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());

    Q_INVOKABLE QNetworkReplyWrapper* download(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());


    //Getters and Setters
    void setUsername(const QString &username);
    QString getUsername();

    void setPassword(const QString &password);
    QString getPassword();

    void setServerUrl(const QUrl &url);
    QUrl getServerUrl();

    void setToken(const QString &token);
    QString getToken();

    void setClientName(const QString &clientName);
    QString getClientName();

    void setClientVersion(const QString &clientVersion);
    QString getClientVersion();

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
    void tokenChanged();
    void clientNameChanged();
    void clientVersionChanged();

protected:

    UserComManager *m_comManager;
};


#endif
