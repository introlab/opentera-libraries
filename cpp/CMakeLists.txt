cmake_minimum_required(VERSION 3.14)

project(opentera-libraries LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_policy(SET QTP0001 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DOPENTERA_CLIENT_NAME="${CMAKE_PROJECT_NAME}")
add_definitions(-DOPENTERA_CLIENT_VERSION="1.0")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools QML)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools QML)

set(TS_FILES opentera-libraries_en_CA.ts)

# add_executable(opentera-libraries
#   main.cpp
#   ${TS_FILES}
# )

# target_link_libraries(opentera-libraries Qt${QT_VERSION_MAJOR}::Core)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


add_subdirectory(clients)

message(STATUS "**** ${opentera_libs_client_user_headers}")
# qt_add_library(opentera_libs_qml STATIC)
# qt_add_qml_module(opentera_libs_qml
#     URI "OpenTeraLibs"
#     VERSION 1.0
#     OUTPUT_DIRECTORY ${QT_QML_OUTPUT_DIRECTORY}/OpenTeraLibs
# )

# include(GNUInstallDirs)
# install(TARGETS opentera-libraries
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
