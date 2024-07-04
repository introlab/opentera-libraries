#include "ParticipantWebAPI.h"

const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_ASSETS = QString("/api/participant/assets");
const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_DEVICES = QString("/api/participant/devices");
const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_LOGIN = QString("/api/participant/login");
const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_LOGOUT = QString("/api/participant/logout");
const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_PARTICIPANTS = QString("/api/participant/participants");
const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_REFRESH_TOKEN = QString("/api/participant/refresh_token");
const QString ParticipantWebAPI::ENDPOINT_PARTICIPANT_SESSIONS = QString("/api/participant/sessions");



ParticipantWebAPI::ParticipantWebAPI(QObject *parent)
    : QObject(parent) {
}

ParticipantWebAPI::~ParticipantWebAPI()
{

}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_ASSETS()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_ASSETS;
}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_DEVICES()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_DEVICES;
}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_LOGIN()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_LOGIN;
}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_LOGOUT()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_LOGOUT;
}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_PARTICIPANTS()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_PARTICIPANTS;
}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_REFRESH_TOKEN()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_REFRESH_TOKEN;
}

QString ParticipantWebAPI::getENDPOINT_PARTICIPANT_SESSIONS()
{
    return ParticipantWebAPI::ENDPOINT_PARTICIPANT_SESSIONS;
}

