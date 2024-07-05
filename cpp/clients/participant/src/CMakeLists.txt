find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network Qml)


set(openteralibs_participant_headers
    ParticipantClient.h
    ParticipantComManager.h
    ParticipantWebSocketManager.h
    ParticipantWebAPI.h
)

set(openteralibs_participant_srcs
    ParticipantClient.cpp
    ParticipantComManager.cpp
    ParticipantWebSocketManager.cpp
    ParticipantWebAPI.cpp
)

qt_add_qml_module(openteralibs_participant_qml
    URI "OpenTeraLibs.ParticipantClient"
    VERSION 1.0
    STATIC
    OUTPUT_DIRECTORY ${QT_QML_OUTPUT_DIRECTORY}/OpenTeraLibs/ParticipantClient
    SOURCES ${openteralibs_participant_headers} ${openteralibs_participant_srcs}
)
target_link_libraries(openteralibs_participant_qml PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Qml
    openteralibs_participant
    opentera_libraries_messages
    openteralibs_common)

target_include_directories(openteralibs_participant_qml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})


add_library(openteralibs_participant STATIC ${openteralibs_participant_headers} ${openteralibs_participant_srcs} ${openteralibs_participant_moc_srcs})
target_link_libraries(openteralibs_participant PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Qml
    opentera_libraries_messages
    openteralibs_common)

target_include_directories(openteralibs_participant PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})
