find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network Qml)

set(openteralibs_common_headers
    BaseComManager.h
    UploadingFile.h
    TransferringFile.h
    DownloadingFile.h
    QNetworkReplyWrapper.h
    FileDownloader.h
)

set(openteralibs_common_srcs
    BaseComManager.cpp
    UploadingFile.cpp
    TransferringFile.cpp
    DownloadingFile.cpp
    QNetworkReplyWrapper.cpp
    FileDownloader.cpp
)

set(OPENTERA_LIBS_COMMON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "doc string")

#This will generate moc_* for Qt
qt_wrap_cpp(openteralibs_common_moc_srcs ${openteralibs_common_headers})

add_library(openteralibs_common STATIC ${openteralibs_common_headers} ${openteralibs_common_srcs} ${openteralibs_common_moc_srcs})
target_link_libraries(openteralibs_common PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Qml)
