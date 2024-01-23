find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network)

set(opentera_libs_client_user_headers
    UserWebAPI.h
    UserComManager.h
    UserWebSocketManager.h
)

set(opentera_libs_client_user_srcs
    UserComManager.cpp
    UserWebSocketManager.cpp
)

include_directories(
    ${OPENTERA_LIBS_COMMON_INCLUDES}
)

#This will generate moc_* for Qt
qt_wrap_cpp(opentera_libs_client_user_moc_srcs ${opentera_libs_client_user_headers})

add_library(opentera_libs_client_user STATIC ${opentera_libs_client_user_headers} ${opentera_libs_client_user_srcs} ${opentera_libs_client_user_moc_srcs})
target_link_libraries(opentera_libs_client_user PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network)
