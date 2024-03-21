#include "FileDownloader.h"
#include <QDebug>
#include <QUrl>

FileDownloader::FileDownloader(const QString &filePath, QNetworkReplyWrapper* networkReply, QObject *parent)
    : QObject(parent), m_networkReply(networkReply), m_file(nullptr)
{
    QUrl url(filePath);
    m_filePath = url.toLocalFile();

    m_file = new QFile(m_filePath, this);
    m_file->open(QIODevice::WriteOnly);

    qDebug() << "Opening file: " << m_filePath << "isOpen: " << m_file->isOpen();

    connect(m_networkReply, &QNetworkReplyWrapper::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(m_networkReply, &QNetworkReplyWrapper::finished, this, &FileDownloader::finished);
    connect(m_networkReply, &QNetworkReplyWrapper::requestFailed, this, &FileDownloader::requestFailed);

    //Internal signal handling
    connect(m_networkReply, &QNetworkReplyWrapper::readyRead, this, &FileDownloader::onReadyRead);
    connect(m_networkReply, &QNetworkReplyWrapper::finished, this, &FileDownloader::onFinished);

}

QNetworkReplyWrapper *FileDownloader::getNetworkReply()
{
    return m_networkReply;
}

QString FileDownloader::getFilePath()
{
    return m_filePath;
}

void FileDownloader::onReadyRead()
{
    if (m_file && m_file->isOpen())
    {
        QByteArray data = m_networkReply->readAll();
        qDebug() << "FileDownloader::onReadyRead()" << " writing bytes: " << data.size();
        m_file->write(data);
    }
}

void FileDownloader::onFinished()
{
    if (m_file && m_file->isOpen())
    {
        qDebug() << "FileDownloader::onFinished()" << " closing file at size: " << m_file->size();
        m_file->close();
    }
}


