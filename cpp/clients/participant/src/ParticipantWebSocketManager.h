#ifndef _PARTICIPANT_WEBSOCKET_MANAGER_H_
#define _PARTICIPANT_WEBSOCKET_MANAGER_H_

#include <QObject>
#include <QUrl>
#include <QWebSocket>
//#include <QProtobufJsonSerializer>

//Protobuf messages
#include <opentera/protobuf/ArchiveEvent.qpb.h>
#include <opentera/protobuf/DatabaseEvent.qpb.h>
#include <opentera/protobuf/DeviceEvent.qpb.h>
#include <opentera/protobuf/JoinSessionEvent.qpb.h>
#include <opentera/protobuf/JoinSessionReplyEvent.qpb.h>
#include <opentera/protobuf/LeaveSessionEvent.qpb.h>
#include <opentera/protobuf/ParticipantEvent.qpb.h>
#include <opentera/protobuf/StopSessionEvent.qpb.h>
#include <opentera/protobuf/TeraEvent.qpb.h>
#include <opentera/protobuf/UserEvent.qpb.h>
#include <opentera/protobuf/TeraMessage.qpb.h>


class ParticipantWebSocketManager : public QObject
{
    Q_OBJECT

    QWebSocket *m_websocket;

public:
    explicit ParticipantWebSocketManager(bool verify_ssl=true, QObject*parent = nullptr);

    void connectTo(const QUrl &url);
    void close();
    bool isConnected();

signals:

    void websocketConnected();
    void websocketDisconnected();

    void deviceEvent(const opentera::protobuf::DeviceEvent &event);
    void joinSessionEvent(const opentera::protobuf::JoinSessionEvent &event);
    void joinSessionReplyEvent(const opentera::protobuf::JoinSessionReplyEvent &event);
    void leaveSessionEvent(const opentera::protobuf::LeaveSessionEvent &event);
    void participantEvent(const opentera::protobuf::ParticipantEvent &event);
    void stopSessionEvent(const opentera::protobuf::StopSessionEvent &event);


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
    // JsonSerializer not working yet.
    //QProtobufJsonSerializer m_protobufSerializer;
    bool parseDeviceEvent(opentera::protobuf::DeviceEvent &event, const QJsonObject &jsonObject);
    bool parseJoinSessionEvent(opentera::protobuf::JoinSessionEvent &event, const QJsonObject &jsonObject);
    bool parseJoinSessionReplyEvent(opentera::protobuf::JoinSessionReplyEvent &event, const QJsonObject &jsonObject);
    bool parseLeaveSessionEvent(opentera::protobuf::LeaveSessionEvent &event, const QJsonObject &jsonObject);
    bool parseParticipantEvent(opentera::protobuf::ParticipantEvent &event, const QJsonObject &jsonObject);
    bool parseStopSessionEvent(opentera::protobuf::StopSessionEvent &event, const QJsonObject &jsonObject);
};
#endif // _PARTICIPANT_WEBSOCKET_MANAGER_H_
