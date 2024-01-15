#ifndef BASECOMMANAGER_H
#define BASECOMMANAGER_H

#include <QObject>

class BaseComManager : public QObject
{
    Q_OBJECT
public:
    explicit BaseComManager(QObject *parent = nullptr);

signals:
};

#endif // BASECOMMANAGER_H
