find_package(Qt6Core REQUIRED)

set(opentera_lib_client_common_headers
    BaseComManager.h
)

set(opentera_lib_client_common_srcs
    BaseComManager.cpp
)


#This will generate moc_* for Qt
qt_wrap_cpp(opentera_lib_client_common_moc_srcs ${opentera_lib_client_common_headers})

add_library(opentera_lib_client_common STATIC ${opentera_lib_client_common_headers} ${opentera_lib_client_common_srcs} ${opentera_lib_client_common_moc_srcs})
target_link_libraries(opentera_lib_client_common PUBLIC Qt6::Core)
