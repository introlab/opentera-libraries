#ifndef USERWEBSOCKETMANAGER_H
#define USERWEBSOCKETMANAGER_H

#include <QObject>
#include <QUrl>
#include <QWebSocket>

class UserWebSocketManager : public QObject
{
    Q_OBJECT

    QWebSocket *m_websocket;

public:
    explicit UserWebSocketManager(bool verify_ssl=true, QObject*parent = nullptr);

    void connectTo(const QUrl &url);
    void close();

    bool isConnected();

signals:

private slots:
    void onBinaryMessageReceived(const QByteArray &message);
    void onTextMessageReceived(const QString &message);
#ifndef OPENTERA_WEBASSEMBLY
    void onSslErrors(const QList<QSslError> &errors);
#endif
    void onConnected();
    void onDisconnected();

private:
    bool m_verifySsl;

};
#endif // USERWEBSOCKETMANAGER_H
