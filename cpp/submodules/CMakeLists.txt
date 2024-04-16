set(protoc_version 22.3)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|x86_64|x86)$")
        message(STATUS "Protobuf Intel Architecture")
        # Add Intel-specific configurations or settings here
        set(protoc_archive_name protoc-${protoc_version}-linux-x86_64.zip)
        set(protobuf_PROTOC_EXE ${CMAKE_CURRENT_BINARY_DIR}/protoc/bin/protoc)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)$")
        message(STATUS "Protobuf ARM Architecture")
        # Add ARM-specific configurations or settings here
        set(protoc_archive_name protoc-${protoc_version}-linux-aarch_64.zip)
        set(protobuf_PROTOC_EXE ${CMAKE_CURRENT_BINARY_DIR}/protoc/bin/protoc)
    else()
        message(STATUS "Unknown Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Protobuf on Windows")
    set(protoc_archive_name protoc-${protoc_version}-win64.zip)
    set(protobuf_PROTOC_EXE ${CMAKE_CURRENT_BINARY_DIR}/protoc/bin/protoc.exe)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Protobuf on macOS")
    set(protoc_archive_name protoc-${protoc_version}-osx-x86_64.zip)
    set(protobuf_PROTOC_EXE ${CMAKE_CURRENT_BINARY_DIR}/protoc/bin/protoc)
else()
    message(STATUS "Unknown host platform")
endif()


# FUll URL
set(protc_binaries_url https://github.com/protocolbuffers/protobuf/releases/download/v${protoc_version}/${protoc_archive_name})

# Download Archive
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${protoc_archive_name})
    file(DOWNLOAD
        ${protc_binaries_url}
        ${CMAKE_CURRENT_BINARY_DIR}/${protoc_archive_name}
        TIMEOUT 300 #seconds
        TLS_VERIFY ON
        SHOW_PROGRESS)
endif()

# Extract Archive
# No protoc, will download native
file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/${protoc_archive_name} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/protoc)

# Add protoc to current path
set(CMAKE_PROGRAM_PATH ${CMAKE_CURRENT_BINARY_DIR}/protoc/bin ${CMAKE_PROGRAM_PATH})

find_package(Qt6 REQUIRED COMPONENTS Protobuf ProtobufWellKnownTypes)

# GLOB ALL .proto files in the opentera_messages/proto directory
file(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/opentera_messages/proto/*.proto)

# Generate C++ files from .proto files
qt_add_protobuf(opentera_libraries_messages
    GENERATE_PACKAGE_SUBFOLDERS
    PROTO_FILES
        ${PROTO_FILES}
    OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/proto
    PROTO_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/opentera_messages/proto
        ${CMAKE_CURRENT_BINARY_DIR}/protoc/include
)


target_link_libraries(opentera_libraries_messages PUBLIC Qt6::Protobuf Qt6::ProtobufWellKnownTypes)
target_include_directories(opentera_libraries_messages PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/proto
    ${CMAKE_CURRENT_BINARY_DIR}/proto/opentera/protobuf
    ${CMAKE_CURRENT_BINARY_DIR}/protoc/include)

