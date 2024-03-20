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

QNetworkReplyWrapper::QNetworkReplyWrapper(QNetworkReply *reply, bool processFinished)
    :   QObject(nullptr), m_processFinished(processFinished)
{
    Q_ASSERT(reply);

    //Set the QSharedPointer with a custom deleter
    m_replyPtr.reset(reply, [](QNetworkReply* obj) {
            obj->deleteLater();
        });

    if (m_processFinished)
    {
        connect(reply, &QNetworkReply::finished, this, &QNetworkReplyWrapper::onRequestfinished);
    }

    connect(reply, &QNetworkReply::downloadProgress, this, &QNetworkReplyWrapper::downloadProgress);
    connect(reply, &QNetworkReply::uploadProgress, this, &QNetworkReplyWrapper::uploadProgress);
    connect(reply, &QNetworkReply::readyRead, this, &QNetworkReplyWrapper::readyRead);
}

QNetworkReplyWrapper::~QNetworkReplyWrapper()
{
    //qDebug() << "QNetworkReplyWrapper::~QNetworkReplyWrapper()";
    m_replyPtr.clear();
}

qint64 QNetworkReplyWrapper::bytesAvailable() const
{
    return m_replyPtr->bytesAvailable();
}

qint64 QNetworkReplyWrapper::bytesToWrite() const
{
    return m_replyPtr->bytesToWrite();
}

qint64 QNetworkReplyWrapper::read(char *data, qint64 maxlen)
{
    return m_replyPtr->read(data, maxlen);
}

QByteArray QNetworkReplyWrapper::read(qint64 maxlen)
{
    return m_replyPtr->read(maxlen);
}

QByteArray QNetworkReplyWrapper::readAll()
{
    return m_replyPtr->readAll();
}

qint64 QNetworkReplyWrapper::readLine(char *data, qint64 maxlen)
{
    return m_replyPtr->readLine(data, maxlen);
}

QByteArray QNetworkReplyWrapper::readLine(qint64 maxlen)
{
    return m_replyPtr->readLine(maxlen);
}

bool QNetworkReplyWrapper::canReadLine() const
{
    return m_replyPtr->canReadLine();
}

qint64 QNetworkReplyWrapper::write(const char *data, qint64 len)
{
    return m_replyPtr->write(data, len);
}

qint64 QNetworkReplyWrapper::write(const char *data)
{
    return m_replyPtr->write(data);
}

qint64 QNetworkReplyWrapper::write(const QByteArray &data)
{
    return m_replyPtr->write(data);
}

void QNetworkReplyWrapper::onRequestfinished()
{

    if (m_processFinished)
    {
        //qDebug() << "QNetworkReplyWrapper::onRequestfinished()";
        QByteArray responseData = m_replyPtr->readAll();
        //qDebug() << responseData;
        QVariant statusCode = m_replyPtr->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (statusCode.toInt() != 200)
        {
            //qDebug() << "QNetworkReplyWrapper emit requestFailed" << responseData << statusCode.toInt();
            emit requestFailed(QVariant(responseData),statusCode.toInt());
        }
        else
        {
            QJsonParseError jsonParseError;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonParseError);
            if (jsonParseError.error == QJsonParseError::NoError) {
                //qDebug() << "QNetworkReplyWrapper emit requestSucceeded" << jsonResponse << statusCode.toInt();
                emit requestSucceeded(jsonResponse.toVariant(), statusCode.toInt());
            }
            else
            {
                //qDebug() << "QNetworkReplyWrapper emit requestFailed" << responseData << statusCode.toInt();
                emit requestFailed(QVariant(jsonParseError.errorString()),statusCode.toInt());
            }
        }
    }
    else
    {
        emit finished();
    }

    // Important, this will deleteLater the reply
    m_replyPtr.clear();
}
