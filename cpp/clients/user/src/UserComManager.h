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

    QUrl getServerUrl() const {
        return m_serverUrl;
    }


public slots:

    void loginToServer(QString username, QString password, QString server_name);
    void login();
    void logout();
    void getOnlineParticipants();
    void getOnlineDevices();
    void getSessionTypes();

signals:

    void loginSucceeded();
    void loginFailed(const QString &errorMessage);

    void logoutSucceeded();
    void logoutFailed();

    void participantOnline(const int id_participant, const int id_participant_group, const int id_project,
                           const QString &participant_email, const QString &participant_name, const bool participant_token_enabled,
                           const QString &participant_uuid, const bool participant_online, const bool participant_busy);

    void deviceOnline(const bool device_busy, const bool device_enable, const QString &device_name, const bool device_online,
                      const qint64 timestamp, const QString &status_string, const QString &device_uuid, const int id_device,
                      const int id_device_subtype, const int id_device_type);


    void participantOffine(const QString &name, const QString &uuid);
    void deviceOffline(const QString &name, const QString &uuid);

    void sessionTypeAvailable(const int id_service, const int id_session_type, const int session_type_category,
                              const QString &session_type_color, const QString &session_type_name, const bool session_type_online,
                              const QString &session_type_service_key, const QString &session_type_service_uuid);


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
