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
    explicit QNetworkReplyWrapper(QNetworkReply *reply, QObject *parent=nullptr);

    virtual ~QNetworkReplyWrapper() override;

signals:
    void requestSucceeded(const QVariant &response, int status_code);
    void requestFailed(const QVariant &response, int status_code);

protected slots:
    void onRequestfinished();


protected:

    QSharedPointer<QNetworkReply> m_replyPtr;

};

#endif // QNETWORKREPLYWRAPPER_H
