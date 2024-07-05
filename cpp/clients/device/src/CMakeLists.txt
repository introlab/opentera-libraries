find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools WebSockets Network)

set(openteralibs_device_headers
    DeviceComManager.h
    DeviceWebApi.h
)

set(openteralibs_device_srcs
    DeviceComManager.cpp
)

include_directories(
    ${OPENTERA_LIBS_COMMON_INCLUDES}
)

#This will generate moc_* for Qt
qt_wrap_cpp(openteralibs_device_moc_srcs ${openteralibs_device_headers})

add_library(openteralibs_device STATIC ${openteralibs_device_headers} ${openteralibs_device_srcs} ${openteralibs_device_moc_srcs})
target_link_libraries(openteralibs_device PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Network)
