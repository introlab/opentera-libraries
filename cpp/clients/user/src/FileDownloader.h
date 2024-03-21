#ifndef FILE_DOWNLOADER_H_
#define FILE_DOWNLOADER_H_

#include "QtQmlIntegration/qqmlintegration.h"
#include <QObject>
#include "QNetworkReplyWrapper.h"
#include <QDebug>
#include <QFile>

class FileDownloader : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    FileDownloader(const QString &filePath, QNetworkReplyWrapper* networkReply, QObject *parent=nullptr);

    Q_INVOKABLE QNetworkReplyWrapper* getNetworkReply();
    Q_INVOKABLE QString getFilePath();

signals:
    void fileOpenError(const QString& message);
    void requestFailed(const QVariant &response, int status_code);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();

private slots:
    void onReadyRead();
    void onFinished();

private:
    QNetworkReplyWrapper *m_networkReply;
    QString m_filePath;
    QFile *m_file;

};

#endif //FILE_DOWNLOADER_H_
