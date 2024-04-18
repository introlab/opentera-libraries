#include "UserWebSocketManager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <Any>


UserWebSocketManager::UserWebSocketManager(bool verify_ssl, QObject *parent)
    : QObject(parent), m_websocket(nullptr), m_verifySsl(verify_ssl)
{
    m_websocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest,parent);
    connect(m_websocket, &QWebSocket::binaryMessageReceived, this, &UserWebSocketManager::onBinaryMessageReceived);
    connect(m_websocket, &QWebSocket::textMessageReceived, this, &UserWebSocketManager::onTextMessageReceived);
#ifndef OPENTERA_WEBASSEMBLY
    connect(m_websocket, &QWebSocket::sslErrors, this, &UserWebSocketManager::onSslErrors);
#endif
    connect(m_websocket, &QWebSocket::connected, this, &UserWebSocketManager::onConnected);
    connect(m_websocket, &QWebSocket::disconnected, this, &UserWebSocketManager::onDisconnected);
}

void UserWebSocketManager::connectTo(const QUrl &url)
{
    qDebug() << "UserWebSocketManager::connectTo " << url;
    m_websocket->open(url);
}

void UserWebSocketManager::close()
{
    if (m_websocket)
    {
        m_websocket->close();
    }
}

bool UserWebSocketManager::isConnected()
{
    if (m_websocket)
    {
        return m_websocket->state() == QAbstractSocket::ConnectedState;
    }
    return false;
}

void UserWebSocketManager::onBinaryMessageReceived(const QByteArray &message)
{
    qDebug() << "onBinaryMessageReceived" << message;
}

void UserWebSocketManager::onTextMessageReceived(const QString &message)
{
    qDebug() << "onTextMessageReceived" << message;
    opentera::protobuf::TeraEvent event;

    //Manually load JSON document because it contains another "message" object
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
    auto rootObject = jsonDocument.object();

    if (rootObject.contains("message"))
    {
        //This is what is needed by the QJsonSerializer
        auto messageObject = rootObject["message"].toObject();

        if (messageObject.contains("header"))
        {
            auto headerObject = messageObject["header"].toObject();
            // Extract header information and update the event
            opentera::protobuf::TeraEvent_QtProtobufNested::Header header;
            if (headerObject.contains("version"))
            {
                header.setVersion(headerObject["version"].toVariant().toUInt());
            }
            if (headerObject.contains("time"))
            {
                header.setTime(headerObject["time"].toDouble());
            }
            if (headerObject.contains("topic"))
            {
                header.setTopic(headerObject["topic"].toString());
            }

            //Print Header
            qDebug() << "Header: " << header.version() << " " << header.time() << " " << header.topic();
            event.setHeader(header);
        }

        if (messageObject.contains("events"))
        {
            auto eventsArray = messageObject["events"].toArray();
            qDebug() << "Events: " << eventsArray.size();

            for (auto eventValue : eventsArray)
            {
                auto eventObject = eventValue.toObject();
                auto type = eventObject["@type"].toString();

                if (type == "type.googleapis.com/opentera.protobuf.UserEvent")
                {
                    opentera::protobuf::UserEvent myEvent;

                    if (eventObject.contains("userUuid"))
                    {
                        myEvent.setUserUuid(eventObject["userUuid"].toString());
                    }

                    if (eventObject.contains("type"))
                    {
                        opentera::protobuf::UserEvent::EventType myType;
                        if (eventObject["type"].toString() == "USER_UNKNOWN")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_UNKNOWN;
                        }
                        else if (eventObject["type"].toString() == "USER_CONNECTED")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_CONNECTED;
                        }
                        else if (eventObject["type"].toString() == "USER_DISCONNECTED")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_DISCONNECTED;
                        }
                        else if (eventObject["type"].toString() == "USER_DELETED")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_DELETED;
                        }
                        else if (eventObject["type"].toString() == "USER_ADDED")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_ADDED;
                        }
                        else if (eventObject["type"].toString() == "USER_UPDATED")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_UPDATED;
                        }
                        else if (eventObject["type"].toString() == "USER_JOINED_SESSION")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_JOINED_SESSION;
                        }
                        else if (eventObject["type"].toString() == "USER_LEFT_SESSION")
                        {
                            myType = opentera::protobuf::UserEvent::EventType::USER_LEFT_SESSION;
                        }

                        myEvent.setType(myType);
                        //myEvent.setType(eventObject["type"].toString());
                    }

                    if (eventObject.contains("userFullname"))
                    {
                       myEvent.setUserFullname(eventObject["userFullname"].toString());
                    }

                    emit userEvent(myEvent);
                }

            }
        }

    }

#if 0
    if (event.events().size() > 0)
    {
        qDebug() << "Number of events " << event.events().size();
        qDebug() << "Event: " << event.hasHeader();
        qDebug() << "Event Unkwnown fields: " << event.unknownFieldNumbers();


        foreach (auto anyMsg, event.events())
        {
            if (auto newArchiveEvent = anyMsg.as<opentera::protobuf::ArchiveEvent>(&m_protobufSerializer))
            {
                emit archiveEvent(newArchiveEvent.value());
            }
            else if (auto newDatabaseEvent = anyMsg.as<opentera::protobuf::DatabaseEvent>(&m_protobufSerializer))
            {
                emit databaseEvent(newDatabaseEvent.value());
            }
            else if (auto newDeviceEvent = anyMsg.as<opentera::protobuf::DeviceEvent>(&m_protobufSerializer))
            {
                emit deviceEvent(newDeviceEvent.value());
            }
            else if (auto newJoinSessionEvent = anyMsg.as<opentera::protobuf::JoinSessionEvent>(&m_protobufSerializer))
            {
                emit joinSessionEvent(newJoinSessionEvent.value());
            }
            else if (auto newJoinSessionReplyEvent = anyMsg.as<opentera::protobuf::JoinSessionReplyEvent>(&m_protobufSerializer))
            {
                emit joinSessionReplyEvent(newJoinSessionReplyEvent.value());
            }
            else if (auto newLeaveSessionEvent = anyMsg.as<opentera::protobuf::LeaveSessionEvent>(&m_protobufSerializer))
            {
                emit leaveSessionEvent(newLeaveSessionEvent.value());
            }
            else if (auto newParticipantEvent = anyMsg.as<opentera::protobuf::ParticipantEvent>(&m_protobufSerializer))
            {
                emit participantEvent(newParticipantEvent.value());
            }
            else if (auto newStopSessionEvent = anyMsg.as<opentera::protobuf::StopSessionEvent>(&m_protobufSerializer))
            {
                emit stopSessionEvent(newStopSessionEvent.value());
            }
            else if (auto newUserEvent = anyMsg.as<opentera::protobuf::UserEvent>(&m_protobufSerializer))
            {
                emit userEvent(newUserEvent.value());
            }
            else
            {
                qDebug() << "onTextMessageReceived: Unknown message type" << message;
            }
        }
    }
    else
    {
        qDebug() << "onTextMessageReceived: Error deserializing message" << message;
    }
#endif
}

#ifndef OPENTERA_WEBASSEMBLY
void UserWebSocketManager::onSslErrors(const QList<QSslError> &errors)
{
    if(m_websocket)
    {
        if (!m_verifySsl)
        {
            m_websocket->ignoreSslErrors();
        }
        else {
            qDebug() << "UserWebSocketManager::onSslErrors" << errors;
        }

    }
}
#endif

void UserWebSocketManager::onConnected()
{

}

void UserWebSocketManager::onDisconnected()
{

}
