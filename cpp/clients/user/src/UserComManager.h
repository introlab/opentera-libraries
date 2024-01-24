#ifndef UserComManager_H
#define UserComManager_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QAuthenticator>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QTimer>

#include "UserWebSocketManager.h"



class UserComManager : public QObject
{
    Q_OBJECT
public:
    explicit UserComManager(QObject *parent = nullptr);

    bool isConnected();

    void setUsername(const QString& username) {
        m_username = username;
    }

    QString getUsername() const {
        return m_username;
    }

    void setPassword(const QString& password) {
        m_password = password;
    }

    QString getPassword() const
    {
        return m_password;
    }

    void setServerUrl(const QString &url) {
        m_serverUrl = QUrl(url);
    }

    void setServerUrl(const QUrl &url)
    {
        m_serverUrl = url;
    }

    QUrl getServerUrl() const {
        return m_serverUrl;
    }


public slots:

    void loginToServer(QString username, QString password, QString server_name);
    void login();
    void logout();
    void getOnlineParticipants();
    void getOnlineDevices();
    void getOnlineUsers();
    void getSessionTypes();

signals:

    void loginSucceeded();
    void loginFailed(const QString &errorMessage);

    void logoutSucceeded();
    void logoutFailed();

    // Results
    void onlineParticipants(const QVariantList &results);
    void onlineDevices(const QVariantList &results);
    void onlineUsers(const QVariantList &results);
    void sessionTypes(const QVariantList &results);

    // Other
    void tokenRefreshed(const QString &token);

private slots:

    void	onNetworkAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

    void	onNetworkFinished(QNetworkReply *reply);

#ifndef OPENTERA_WEBASSEMBLY
    void	onNetworkEncrypted(QNetworkReply *reply);
    void	onNetworkSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif
    void    onRefreshTokenTimeout();

private:
    QNetworkAccessManager *m_networkAccessManager;
    UserWebSocketManager *m_websocketManager;
    QString m_username;
    QString m_password;
    QString m_token;
    QUrl m_serverUrl;
    QTimer *m_refreshTokenTimer;

    QNetworkReply* _doPost(const QUrl &url, const QUrlQuery &query_args = QUrlQuery(), const QString &data = QString(), bool use_token = true);
    QNetworkReply* _doGet(const QUrl &url, const QUrlQuery &query_args = QUrlQuery(), bool use_token = true);
    void _setRequestLanguage(QNetworkRequest &request);
    void _setRequestCredentials(QNetworkRequest &request, const bool &use_token);
    void _setRequestVersions(QNetworkRequest &request);
    void _connectWebSocket(const QUrl &url);
    void _startRefreshTokenTimer();
    void _stopRefreshTokenTimer();
    void _refreshToken();

};

#endif // UserComManager_H
