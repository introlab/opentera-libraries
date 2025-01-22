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
    //qDebug() << "UserWebSocketManager::connectTo " << url;
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

    // The following code should work but does not.
    // We will manually parse the JSON data instead until the problem is fixed.

    /*
        //We are receiving a JSON serialized protobuf / TeraMessage
        opentera::protobuf::TeraMessage rootMessage;


        if (rootMessage.deserialize(&m_protobufSerializer, message.toUtf8()))
        {
            qDebug() << "Got Message !";
            //Convert Any Message to TeraEvent Message (optional should be verified)
            auto optionalTeraEvent = rootMessage.message().as<opentera::protobuf::TeraEvent>(&m_protobufSerializer);
            if (optionalTeraEvent) {
                opentera::protobuf::TeraEvent event = optionalTeraEvent.value();

                qDebug() << "Event size: " << event.events().size();
            }
        }
    */

    //qDebug() << "onTextMessageReceived" << message;
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
            //qDebug() << "Header: " << header.version() << " " << header.time() << " " << header.topic();
            event.setHeader(header);
        }

        if (messageObject.contains("events"))
        {
            auto eventsArray = messageObject["events"].toArray();
            //qDebug() << "Events: " << eventsArray.size();

            for (auto eventValue : eventsArray)
            {
                auto eventObject = eventValue.toObject();
                auto type = eventObject["@type"].toString();

                // TODO Parse all message events
                if (type == "type.googleapis.com/opentera.protobuf.ArchiveEvent")
                {
                    opentera::protobuf::ArchiveEvent myEvent;
                    if (parseArchiveEvent(myEvent, eventObject))
                    {
                        emit archiveEvent(myEvent);
                    }
                }
                else if (type == "type.googleapis.com/opentera.protobuf.DeviceEvent")
                {
                    opentera::protobuf::DeviceEvent myEvent;
                    if (parseDeviceEvent(myEvent, eventObject))
                    {
                        emit deviceEvent(myEvent);
                    }
                }
                else if (type == "type.googleapis.com/opentera.protobuf.ParticipantEvent")
                {
                    opentera::protobuf::ParticipantEvent myEvent;
                    if (parseParticipantEvent(myEvent, eventObject))
                    {
                        emit participantEvent(myEvent);
                    }

                }
                else if (type == "type.googleapis.com/opentera.protobuf.UserEvent")
                {
                    opentera::protobuf::UserEvent myEvent;
                    if (parseUserEvent(myEvent, eventObject))
                    {
                        emit userEvent(myEvent);
                    }
                }
            }
        }

    }

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
    emit websocketConnected();
}

void UserWebSocketManager::onDisconnected()
{
    emit websocketDisconnected();
}

bool UserWebSocketManager::parseArchiveEvent(opentera::protobuf::ArchiveEvent &event, const QJsonObject &jsonObject)
{
    if (jsonObject.contains("archiveUuid"))
    {
        event.setArchiveUuid(jsonObject["archiveUuid"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("ownerUuid"))
    {
        event.setOwnerUuid(jsonObject["ownerUuid"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("archiveUrl"))
    {
        event.setArchiveUrl(jsonObject["archiveUrl"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("status"))
    {
        opentera::protobuf::ArchiveEvent::StatusType myType;
        if (jsonObject["status"].toString() == "STATUS_PENDING")
        {
            myType = opentera::protobuf::ArchiveEvent::StatusType::STATUS_PENDING;
        }
        else if (jsonObject["status"].toString() == "STATUS_INPROGRESS")
        {
            myType = opentera::protobuf::ArchiveEvent::StatusType::STATUS_INPROGRESS;
        }
        else if (jsonObject["status"].toString() == "STATUS_COMPLETED")
        {
            myType = opentera::protobuf::ArchiveEvent::StatusType::STATUS_COMPLETED;
        }
        else if (jsonObject["status"].toString() == "STATUS_CANCELLED")
        {
            myType = opentera::protobuf::ArchiveEvent::StatusType::STATUS_CANCELLED;
        }
        else if (jsonObject["status"].toString() == "STATUS_DELETED")
        {
            myType = opentera::protobuf::ArchiveEvent::StatusType::STATUS_DELETED;
        }

        event.setStatus(myType);
    }
    else {
        return false;
    }

    // Everything ok!
    return true;
}

bool UserWebSocketManager::parseDatabaseEvent(opentera::protobuf::DatabaseEvent &event, const QJsonObject &jsonObject)
{
    // TODO parse DatabaseEvent
    return false;
}

bool UserWebSocketManager::parseDeviceEvent(opentera::protobuf::DeviceEvent &event, const QJsonObject &jsonObject)
{
    if (jsonObject.contains("deviceUuid"))
    {
        event.setDeviceUuid(jsonObject["deviceUuid"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("type"))
    {
        opentera::protobuf::DeviceEvent::EventType myType;
        if (jsonObject["type"].toString() == "DEVICE_UNKNOWN")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_UNKNOWN;
        }
        else if (jsonObject["type"].toString() == "DEVICE_CONNECTED")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_CONNECTED;
        }
        else if (jsonObject["type"].toString() == "DEVICE_DISCONNECTED")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_DISCONNECTED;
        }
        else if (jsonObject["type"].toString() == "DEVICE_DELETED")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_DELETED;
        }
        else if (jsonObject["type"].toString() == "DEVICE_ADDED")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_ADDED;
        }
        else if (jsonObject["type"].toString() == "DEVICE_JOINED_SESSION")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_JOINED_SESSION;
        }
        else if (jsonObject["type"].toString() == "DEVICE_LEFT_SESSION")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_LEFT_SESSION;
        }
        else if (jsonObject["type"].toString() == "DEVICE_STATUS_CHANGED")
        {
            myType = opentera::protobuf::DeviceEvent::EventType::DEVICE_STATUS_CHANGED;
        }
        event.setType(myType);
    }
    else {
        return false;
    }

    if (jsonObject.contains("deviceName"))
    {
        event.setDeviceName(jsonObject["deviceName"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("deviceStatus"))
    {
        event.setDeviceStatus(jsonObject["deviceStatus"].toString());
    }
    else {
        return false;
    }

    // Everything ok!
    return true;
}

bool UserWebSocketManager::parseJoinSessionEvent(opentera::protobuf::JoinSessionEvent &event, const QJsonObject &jsonObject)
{
    // TODO parse JoinSessionEvent
    return false;
}

bool UserWebSocketManager::parseJoinSessionReplyEvent(opentera::protobuf::JoinSessionReplyEvent &event, const QJsonObject &jsonObject)
{
    // TODO parse JoinSessionReplyEvent
    return false;
}

bool UserWebSocketManager::parseLeaveSessionEvent(opentera::protobuf::LeaveSessionEvent &event, const QJsonObject &jsonObject)
{
    // TODO parse LeaveSessionEvent
    return false;
}

bool UserWebSocketManager::parseParticipantEvent(opentera::protobuf::ParticipantEvent &event, const QJsonObject &jsonObject)
{
    if (jsonObject.contains("participantUuid"))
    {
        event.setParticipantUuid(jsonObject["participantUuid"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("type"))
    {
        opentera::protobuf::ParticipantEvent::EventType myType;
        if (jsonObject["type"].toString() == "PARTICIPANT_UNKNOWN")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_UNKNOWN;
        }
        else if (jsonObject["type"].toString() == "PARTICIPANT_CONNECTED")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_CONNECTED;
        }
        else if (jsonObject["type"].toString() == "PARTICIPANT_DISCONNECTED")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_DISCONNECTED;
        }
        else if (jsonObject["type"].toString() == "PARTICIPANT_DELETED")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_DELETED;
        }
        else if (jsonObject["type"].toString() == "PARTICIPANT_ADDED")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_ADDED;
        }
        else if (jsonObject["type"].toString() == "PARTICIPANT_JOINED_SESSION")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_JOINED_SESSION;
        }
        else if (jsonObject["type"].toString() == "PARTICIPANT_LEFT_SESSION")
        {
            myType = opentera::protobuf::ParticipantEvent::EventType::PARTICIPANT_LEFT_SESSION;
        }
        event.setType(myType);
    }
    else {
        return false;
    }

    if (jsonObject.contains("participantName"))
    {
        event.setParticipantName(jsonObject["participantName"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("participantProjectName"))
    {
        event.setParticipantProjectName(jsonObject["participantProjectName"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("participantSiteName"))
    {
        event.setParticipantSiteName(jsonObject["participantSiteName"].toString());
    }
    else {
        return false;
    }

    // Everything ok!
    return true;
}

bool UserWebSocketManager::parseStopSessionEvent(opentera::protobuf::StopSessionEvent &event, const QJsonObject &jsonObject)
{
    // TODO parse StopSessionEvent
    return false;
}

bool UserWebSocketManager::parseUserEvent(opentera::protobuf::UserEvent &event, const QJsonObject &jsonObject)
{
    if (jsonObject.contains("userUuid"))
    {
        event.setUserUuid(jsonObject["userUuid"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("type"))
    {
        opentera::protobuf::UserEvent::EventType myType;
        if (jsonObject["type"].toString() == "USER_UNKNOWN")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_UNKNOWN;
        }
        else if (jsonObject["type"].toString() == "USER_CONNECTED")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_CONNECTED;
        }
        else if (jsonObject["type"].toString() == "USER_DISCONNECTED")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_DISCONNECTED;
        }
        else if (jsonObject["type"].toString() == "USER_DELETED")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_DELETED;
        }
        else if (jsonObject["type"].toString() == "USER_ADDED")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_ADDED;
        }
        else if (jsonObject["type"].toString() == "USER_UPDATED")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_UPDATED;
        }
        else if (jsonObject["type"].toString() == "USER_JOINED_SESSION")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_JOINED_SESSION;
        }
        else if (jsonObject["type"].toString() == "USER_LEFT_SESSION")
        {
            myType = opentera::protobuf::UserEvent::EventType::USER_LEFT_SESSION;
        }

        event.setType(myType);
        //myEvent.setType(eventObject["type"].toString());
    }
    else {
        return false;
    }

    if (jsonObject.contains("userFullname"))
    {
        event.setUserFullname(jsonObject["userFullname"].toString());
    }
    else {
        return false;
    }

    // Everything ok!
    return true;
}
