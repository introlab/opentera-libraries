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
#include <QJsonDocument>
#include "UserWebSocketManager.h"


class UserComManager : public QObject
{
    Q_OBJECT
public:
    explicit UserComManager(bool verify_ssl=true, QObject *parent = nullptr);

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

    QString getToken() const {
        return m_token;
    }

    QString getClientName() const {
        return m_clientName;
    }

    QString getClientVersion() const {
        return m_clientVersion;
    }

    //Get
    QNetworkReply* get(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());

    //Post
    QNetworkReply* post(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &data = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());

    //Delete
    QNetworkReply* deleteResource(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());

    //Download
    QNetworkReply* download(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());


    QJsonDocument downloadDocumentJson(const QString &endpoint, const QVariantMap &params = QVariantMap(), const QVariantMap &extra_headers = QVariantMap());

public slots:

    void loginToServer(QString username, QString password, QString server_name);
    void login();
    void logout();
    void setToken(const QString &token);
    void setClientName(const QString &client_name);
    void setClientVersion(const QString &client_version);


signals:

    void loginSucceeded();
    void loginFailed(const QString &errorMessage);

    void logoutSucceeded();
    void logoutFailed();


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
    bool m_verifySsl;
    QString m_clientName;
    QString m_clientVersion;

    QNetworkReply* _doPost(const QUrl &url,
                           const QUrlQuery &query_args = QUrlQuery(),
                           const QByteArray &data = QByteArray(),
                           const QMap<QString,QString> & extra_headers = QMap<QString, QString>(),
                           bool use_token = true);

    QNetworkReply* _doGet(const QUrl &url,
                          const QUrlQuery &query_args = QUrlQuery(),
                          const QMap<QString,QString> & extra_headers = QMap<QString, QString>(),
                          bool use_token = true);


    QNetworkReply* _doDelete(const QUrl &url,
                             const QUrlQuery &query_args = QUrlQuery(),
                             const QMap<QString,QString> & extra_headers = QMap<QString, QString>(),
                             bool use_token = true);


    QNetworkReply* _doDownload(const QUrl &url,
                          const QUrlQuery &query_args = QUrlQuery(),
                          const QMap<QString,QString> & extra_headers = QMap<QString, QString>(),
                          bool use_token = true);

    void _setRequestLanguage(QNetworkRequest &request);
    void _setRequestCredentials(QNetworkRequest &request, const bool &use_token);
    void _setRequestVersions(QNetworkRequest &request);
    void _setRequestExtraHeaders(QNetworkRequest &request, QMap<QString,QString> extra_headers);
    void _connectWebSocket(const QUrl &url);
    void _startRefreshTokenTimer();
    void _stopRefreshTokenTimer();
    void _refreshToken();

};

#endif // UserComManager_H
