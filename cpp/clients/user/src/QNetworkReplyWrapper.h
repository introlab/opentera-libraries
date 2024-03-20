#ifndef QNETWORKREPLYWRAPPER_H
#define QNETWORKREPLYWRAPPER_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkReply>
#include <QSharedPointer>


class QNetworkReplyWrapper : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    QNetworkReplyWrapper(QObject *parent=nullptr);
    explicit QNetworkReplyWrapper(QNetworkReply *reply, QObject *parent=nullptr);

    virtual ~QNetworkReplyWrapper() override;

    //Taken from QIODevice
    Q_INVOKABLE qint64 bytesAvailable() const;
    Q_INVOKABLE qint64 bytesToWrite() const;
    Q_INVOKABLE qint64 read(char *data, qint64 maxlen);
    Q_INVOKABLE QByteArray read(qint64 maxlen);
    Q_INVOKABLE QByteArray readAll();
    Q_INVOKABLE qint64 readLine(char *data, qint64 maxlen);
    Q_INVOKABLE QByteArray readLine(qint64 maxlen = 0);
    Q_INVOKABLE bool canReadLine() const;
    Q_INVOKABLE qint64 write(const char *data, qint64 len);
    Q_INVOKABLE qint64 write(const char *data);
    Q_INVOKABLE qint64 write(const QByteArray &data);

signals:
    void requestSucceeded(const QVariant &response, int status_code);
    void requestFailed(const QVariant &response, int status_code);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readyRead();

protected slots:
    void onRequestfinished();


protected:

    QSharedPointer<QNetworkReply> m_replyPtr;

};

#endif // QNETWORKREPLYWRAPPER_H
