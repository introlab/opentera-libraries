find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network Qml)


set(opentera_libs_client_user_headers
    UserWebAPI.h
    UserComManager.h
    UserWebSocketManager.h
    UserClient.h
    QNetworkReplyWrapper.h
)

set(opentera_libs_client_user_srcs
    UserComManager.cpp
    UserWebSocketManager.cpp
    UserClient.cpp
    QNetworkReplyWrapper.cpp
)

qt_add_qml_module(opentera_libs_client_user_qml
    URI "OpenTeraLibs.UserClient"
    VERSION 1.0
    STATIC
    OUTPUT_DIRECTORY ${QT_QML_OUTPUT_DIRECTORY}/OpenTeraLibs/UserClient
    SOURCES ${opentera_libs_client_user_headers} ${opentera_libs_client_user_srcs}
)
target_link_libraries(opentera_libs_client_user_qml PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Qml)
target_include_directories(opentera_libs_client_user_qml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})


add_library(opentera_libs_client_user STATIC ${opentera_libs_client_user_headers} ${opentera_libs_client_user_srcs} ${opentera_libs_client_user_moc_srcs})
target_link_libraries(opentera_libs_client_user PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Qml)
target_include_directories(opentera_libs_client_user PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENTERA_LIBS_COMMON_INCLUDES})
