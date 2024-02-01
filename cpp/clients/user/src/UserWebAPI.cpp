#include "UserWebAPI.h"

const QString UserWebAPI::ENDPOINT_USER_LOGIN = QString("/api/user/login");
const QString UserWebAPI::ENDPOINT_USER_LOGOUT = QString("/api/user/logout");
const QString UserWebAPI::ENDPOINT_USER_REFRESH_TOKEN = QString("/api/user/refresh_token");
const QString UserWebAPI::ENDPOINT_USER_USERS = QString("/api/user/users");
const QString UserWebAPI::ENDPOINT_USER_USERGROUPS = QString("/api/user/usergroups");
const QString UserWebAPI::ENDPOINT_USER_USERS_USERGROUPS = QString("/api/user/users/usergroups");
const QString UserWebAPI::ENDPOINT_USER_USERS_PREFERENCES = QString("/api/user/users/preferences");
const QString UserWebAPI::ENDPOINT_USER_SITES = QString("/api/user/sites");
const QString UserWebAPI::ENDPOINT_USER_PROJECTS = QString("/api/user/projects");
const QString UserWebAPI::ENDPOINT_USER_SITEACCESS = QString("/api/user/siteaccess");
const QString UserWebAPI::ENDPOINT_USER_PROJECTACCESS = QString("/api/user/projectaccess");
const QString UserWebAPI::ENDPOINT_USER_DEVICES = QString("/api/user/devices");
const QString UserWebAPI::ENDPOINT_USER_DEVICES_SITES = QString("/api/user/devices/sites");
const QString UserWebAPI::ENDPOINT_USER_DEVICES_PROJECTS = QString("/api/user/devices/projects");
const QString UserWebAPI::ENDPOINT_USER_DEVICES_PARTICIPANTS = QString("/api/user/devices/participants");
const QString UserWebAPI::ENDPOINT_USER_DEVICESUBTYPES = QString("/api/user/devicesubtypes");
const QString UserWebAPI::ENDPOINT_USER_DEVICETYPES = QString("/api/user/devicetypes");
const QString UserWebAPI::ENDPOINT_USER_PARTICIPANTS = QString("/api/user/participants");
const QString UserWebAPI::ENDPOINT_USER_GROUPS = QString("/api/user/groups");
const QString UserWebAPI::ENDPOINT_USER_SESSIONS = QString("/api/user/sessions");
const QString UserWebAPI::ENDPOINT_USER_SESSIONTYPES = QString("/api/user/sessiontypes");
const QString UserWebAPI::ENDPOINT_USER_SESSIONTYPES_PROJECTS = QString("/api/user/sessiontypes/projects");
const QString UserWebAPI::ENDPOINT_USER_SESSIONTYPES_SITES = QString("/api/user/sessiontypes/sites");
const QString UserWebAPI::ENDPOINT_USER_SESSIONS_EVENTS = QString("/api/user/sessions/events");
const QString UserWebAPI::ENDPOINT_USER_SERVICES = QString("/api/user/services");
const QString UserWebAPI::ENDPOINT_USER_SERVICES_PROJECTS = QString("/api/user/services/projects");
const QString UserWebAPI::ENDPOINT_USER_SERVICES_SITES = QString("/api/user/services/sites");
const QString UserWebAPI::ENDPOINT_USER_SERVICES_ACCESS = QString("/api/user/services/access");
const QString UserWebAPI::ENDPOINT_USER_SERVICES_CONFIGS = QString("/api/user/services/configs");
const QString UserWebAPI::ENDPOINT_USER_SERVICES_ROLES = QString("/api/user/services/roles");
const QString UserWebAPI::ENDPOINT_USER_STATS = QString("/api/user/stats");
const QString UserWebAPI::ENDPOINT_USER_ASSETS = QString("/api/user/assets");
const QString UserWebAPI::ENDPOINT_USER_TESTTYPES = QString("/api/user/testtypes");
const QString UserWebAPI::ENDPOINT_USER_TESTTYPES_SITES = QString("/api/user/testtypes/sites");
const QString UserWebAPI::ENDPOINT_USER_TESTTYPES_PROJECTS = QString("/api/user/testtypes/projects");
const QString UserWebAPI::ENDPOINT_USER_TESTS = QString("/api/user/tests");
const QString UserWebAPI::ENDPOINT_USER_DISCONNECT = QString("/api/user/disconnect");
const QString UserWebAPI::ENDPOINT_USER_SESSIONS_MANAGER = QString("/api/user/sessions/manager");
const QString UserWebAPI::ENDPOINT_USER_USERS_ONLINE = QString("/api/user/users/online");
const QString UserWebAPI::ENDPOINT_USER_PARTICIPANTS_ONLINE = QString("/api/user/participants/online");
const QString UserWebAPI::ENDPOINT_USER_DEVICES_ONLINE = QString("/api/user/devices/online");
const QString UserWebAPI::ENDPOINT_USER_VERSIONS = QString("/api/user/versions");
const QString UserWebAPI::ENDPOINT_USER_FORMS = QString("/api/user/forms");

UserWebAPI::UserWebAPI(QObject *parent)
    : QObject(parent) {
}

UserWebAPI::~UserWebAPI()
{

}

QString UserWebAPI::getENDPOINT_USER_FORMS()
{
    return ENDPOINT_USER_FORMS;
}

QString UserWebAPI::getENDPOINT_USER_VERSIONS()
{
    return ENDPOINT_USER_VERSIONS;
}

QString UserWebAPI::getENDPOINT_USER_DEVICES_ONLINE()
{
    return ENDPOINT_USER_DEVICES_ONLINE;
}

QString UserWebAPI::getENDPOINT_USER_PARTICIPANTS_ONLINE()
{
    return ENDPOINT_USER_PARTICIPANTS_ONLINE;
}

QString UserWebAPI::getENDPOINT_USER_USERS_ONLINE()
{
    return ENDPOINT_USER_USERS_ONLINE;
}

QString UserWebAPI::getENDPOINT_USER_SESSIONS_MANAGER()
{
    return ENDPOINT_USER_SESSIONS_MANAGER;
}

QString UserWebAPI::getENDPOINT_USER_DISCONNECT()
{
    return ENDPOINT_USER_DISCONNECT;
}

QString UserWebAPI::getENDPOINT_USER_TESTS()
{
    return ENDPOINT_USER_TESTS;
}

QString UserWebAPI::getENDPOINT_USER_TESTTYPES_PROJECTS()
{
    return ENDPOINT_USER_TESTTYPES_PROJECTS;
}

QString UserWebAPI::getENDPOINT_USER_TESTTYPES_SITES()
{
    return ENDPOINT_USER_TESTTYPES_SITES;
}

QString UserWebAPI::getENDPOINT_USER_TESTTYPES()
{
    return ENDPOINT_USER_TESTTYPES;
}

QString UserWebAPI::getENDPOINT_USER_ASSETS()
{
    return ENDPOINT_USER_ASSETS;
}

QString UserWebAPI::getENDPOINT_USER_STATS()
{
    return ENDPOINT_USER_STATS;
}

QString UserWebAPI::getENDPOINT_USER_SERVICES_ROLES()
{
    return ENDPOINT_USER_SERVICES_ROLES;
}

QString UserWebAPI::getENDPOINT_USER_SERVICES_CONFIGS()
{
    return ENDPOINT_USER_SERVICES_CONFIGS;
}

QString UserWebAPI::getENDPOINT_USER_SERVICES_ACCESS()
{
    return ENDPOINT_USER_SERVICES_ACCESS;
}

QString UserWebAPI::getENDPOINT_USER_SERVICES_SITES()
{
    return ENDPOINT_USER_SERVICES_SITES;
}

QString UserWebAPI::getENDPOINT_USER_SERVICES_PROJECTS()
{
    return ENDPOINT_USER_SERVICES_PROJECTS;
}

QString UserWebAPI::getENDPOINT_USER_SERVICES()
{
    return ENDPOINT_USER_SERVICES;
}

QString UserWebAPI::getENDPOINT_USER_SESSIONS_EVENTS()
{
    return ENDPOINT_USER_SESSIONS_EVENTS;
}

QString UserWebAPI::getENDPOINT_USER_SESSIONTYPES_SITES()
{
    return ENDPOINT_USER_SESSIONTYPES_SITES;
}

QString UserWebAPI::getENDPOINT_USER_SESSIONTYPES_PROJECTS()
{
    return ENDPOINT_USER_SESSIONTYPES_PROJECTS;
}

QString UserWebAPI::getENDPOINT_USER_SESSIONTYPES()
{
    return ENDPOINT_USER_SESSIONTYPES;
}

QString UserWebAPI::getENDPOINT_USER_SESSIONS()
{
    return ENDPOINT_USER_SESSIONS;
}

QString UserWebAPI::getENDPOINT_USER_GROUPS()
{
    return ENDPOINT_USER_GROUPS;
}

QString UserWebAPI::getENDPOINT_USER_PARTICIPANTS()
{
    return ENDPOINT_USER_PARTICIPANTS;
}

QString UserWebAPI::getENDPOINT_USER_DEVICETYPES()
{
    return ENDPOINT_USER_DEVICETYPES;
}

QString UserWebAPI::getENDPOINT_USER_DEVICESUBTYPES()
{
    return ENDPOINT_USER_DEVICESUBTYPES;
}

QString UserWebAPI::getENDPOINT_USER_DEVICES_PARTICIPANTS()
{
    return ENDPOINT_USER_DEVICES_PARTICIPANTS;
}

QString UserWebAPI::getENDPOINT_USER_DEVICES_PROJECTS()
{
    return ENDPOINT_USER_DEVICES_PROJECTS;
}

QString UserWebAPI::getENDPOINT_USER_DEVICES_SITES()
{
    return ENDPOINT_USER_DEVICES_SITES;
}

QString UserWebAPI::getENDPOINT_USER_DEVICES()
{
    return ENDPOINT_USER_DEVICES;
}

QString UserWebAPI::getENDPOINT_USER_PROJECTACCESS()
{
    return ENDPOINT_USER_PROJECTACCESS;
}

QString UserWebAPI::getENDPOINT_USER_SITEACCESS()
{
    return ENDPOINT_USER_SITEACCESS;
}

QString UserWebAPI::getENDPOINT_USER_PROJECTS()
{
    return ENDPOINT_USER_PROJECTS;
}

QString UserWebAPI::getENDPOINT_USER_SITES()
{
    return ENDPOINT_USER_SITES;
}

QString UserWebAPI::getENDPOINT_USER_USERS_PREFERENCES()
{
    return ENDPOINT_USER_USERS_PREFERENCES;
}

QString UserWebAPI::getENDPOINT_USER_USERGROUPS()
{
    return ENDPOINT_USER_USERGROUPS;
}

QString UserWebAPI::getENDPOINT_USER_USERS()
{
    return ENDPOINT_USER_USERS;
}

QString UserWebAPI::getENDPOINT_USER_REFRESH_TOKEN()
{
    return ENDPOINT_USER_REFRESH_TOKEN;
}



QString UserWebAPI::getENDPOINT_USER_LOGOUT()
{
    return ENDPOINT_USER_LOGOUT;
}


QString UserWebAPI::getENDPOINT_USER_LOGIN()
{
    return ENDPOINT_USER_LOGIN;
}
