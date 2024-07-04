find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network Qml)


set(opentera_libs_client_participant_headers

)

set(opentera_libs_client_participant_srcs

)

qt_add_qml_module(opentera_libs_client_participant_qml
    URI "OpenTeraLibs.ParticipantClient"
    VERSION 1.0
    STATIC
    OUTPUT_DIRECTORY ${QT_QML_OUTPUT_DIRECTORY}/OpenTeraLibs/ParticipantClient
    SOURCES ${opentera_libs_client_participant_headers} ${opentera_libs_client_participant_srcs}
)
target_link_libraries(opentera_libs_client_participant_qml PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Qml
    opentera_libs_client_participant
    opentera_libraries_messages
    opentera_libs_client_common)

target_include_directories(opentera_libs_client_participant_qml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})


add_library(opentera_libs_client_participant STATIC ${opentera_libs_client_participant_headers} ${opentera_libs_client_participant_srcs} ${opentera_libs_client_participant_moc_srcs})
target_link_libraries(opentera_libs_client_participant PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Qml
    opentera_libraries_messages
    opentera_libs_client_common)

target_include_directories(opentera_libs_client_participant PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})
