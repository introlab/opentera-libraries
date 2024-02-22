#include "QNetworkReplyWrapper.h"
#include <QDebug>
#include <QByteArray>
#include "QtCore/qjsonobject.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonValue>


QNetworkReplyWrapper::QNetworkReplyWrapper(QObject *parent)
    :   QObject(parent)
{

}

QNetworkReplyWrapper::QNetworkReplyWrapper(QNetworkReply *reply, QObject *parent)
    :   QObject(parent)
{
    Q_ASSERT(reply);

    //Set the QSharedPointer with a custom deleter
    m_replyPtr.reset(reply, [](QNetworkReply* obj) {
            obj->deleteLater();
        });

    connect(reply, &QNetworkReply::finished, this, &QNetworkReplyWrapper::onRequestfinished);
}

QNetworkReplyWrapper::~QNetworkReplyWrapper()
{
    qDebug() << "QNetworkReplyWrapper::~QNetworkReplyWrapper()";
    m_replyPtr.clear();
}

void QNetworkReplyWrapper::onRequestfinished()
{
    qDebug() << "QNetworkReplyWrapper::onRequestfinished()";
    QByteArray responseData = m_replyPtr->readAll();
    //qDebug() << responseData;
    QVariant statusCode = m_replyPtr->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (statusCode.toInt() != 200)
    {
        qDebug() << "QNetworkReplyWrapper emit requestFailed" << responseData << statusCode.toInt();
        emit requestFailed(QVariant(responseData),statusCode.toInt());
        return;
    }

    QJsonParseError jsonParseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);
    if (jsonParseError.error == QJsonParseError::NoError) {
        qDebug() << "QNetworkReplyWrapper emit requestSucceeded" << jsonResponse << statusCode.toInt();
        emit requestSucceeded(jsonResponse.toVariant(), statusCode.toInt());
    }

    // Important, this will deleteLater the reply
    m_replyPtr.clear();
}
