find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network Qml)


set(openteralibs_user_headers
    UserWebAPI.h
    UserComManager.h
    UserWebSocketManager.h
    UserClient.h
)

set(openteralibs_user_srcs
    UserWebAPI.cpp
    UserComManager.cpp
    UserWebSocketManager.cpp
    UserClient.cpp
)

qt_add_qml_module(openteralibs_user_qml
    URI "OpenTeraLibs.UserClient"
    VERSION 1.0
    STATIC
    OUTPUT_DIRECTORY ${QT_QML_OUTPUT_DIRECTORY}/OpenTeraLibs/UserClient
    SOURCES ${openteralibs_user_headers} ${openteralibs_user_srcs}
)
target_link_libraries(openteralibs_user_qml PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Qml
    openteralibs_user
    opentera_libraries_messages
    openteralibs_common)

target_include_directories(openteralibs_user_qml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})


add_library(openteralibs_user STATIC ${openteralibs_user_headers} ${openteralibs_user_srcs} ${openteralibs_user_moc_srcs})
target_link_libraries(openteralibs_user PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Qml
    opentera_libraries_messages
    openteralibs_common)

target_include_directories(openteralibs_user PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})
