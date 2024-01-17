find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network)

set(opentera_libs_client_common_headers
    BaseComManager.h
    UploadingFile.h
    TransferringFile.h
    DownloadingFile.h
)

set(opentera_libs_client_common_srcs
    BaseComManager.cpp
    UploadingFile.cpp
    TransferringFile.cpp
    DownloadingFile.cpp
)

set(OPENTERA_LIBS_COMMON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "doc string")

#This will generate moc_* for Qt
qt_wrap_cpp(opentera_libs_client_common_moc_srcs ${opentera_libs_client_common_headers})

add_library(opentera_libs_client_common STATIC ${opentera_libs_client_common_headers} ${opentera_libs_client_common_srcs} ${opentera_libs_client_common_moc_srcs})
target_link_libraries(opentera_libs_client_common PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network)
