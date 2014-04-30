cmake_minimum_required (VERSION 2.6)
project (directoryd)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

#set(CMAKE_EXE_LINKER_FLAGS "-flto")
#set(CMAKE_SHARED_LINKER_FLAGS "-flto")
#set(CMAKE_MODULE_LINKER_FLAGS "-flto")
#set(CMAKE_C_FLAGS "-Ofast -flto")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c1x")

find_library(ZMQ_LIBRARY NAMES zmq libzmq)
find_library(PROTOBUF_LIBS NAMES protobuf)
find_library(AVAHI_LIBS NAMES avahi-client)
find_path(ZMQ_INCLUDE zmq.h)
find_path(AVAHI_INCLUDE avahi-client/client.h)

set(ZMQ_LIBS ${ZMQ_LIBRARY})

include_directories(${ZMQ_INCLUDE} ${AVAHI_INCLUDE})

add_library(avahi++ SHARED Avahi.cpp Browser.cpp Registrar.cpp)
target_link_libraries(avahi++ ${AVAHI_LIBS})

add_executable(test_browse test_browse.cpp)
target_link_libraries(test_browse avahi++)

add_executable(test_register test_register.cpp)
target_link_libraries(test_register avahi++)

add_custom_command(OUTPUT "${PROJECT_SOURCE_DIR}/protobuf/Services.pb.cc"
                          "${PROJECT_SOURCE_DIR}/protobuf/Services.pb.h"
                   COMMAND mkdir -p "${PROJECT_SOURCE_DIR}/protobuf"
                   COMMAND protoc "${PROJECT_SOURCE_DIR}/Services.proto"
                                  --cpp_out="${PROJECT_SOURCE_DIR}/protobuf"
                                  --proto_path="${PROJECT_SOURCE_DIR}"
                   MAIN_DEPENDENCY "${PROJECT_SOURCE_DIR}/Services.proto")

add_executable(directoryd main.cpp protobuf/Services.pb.cc)
target_link_libraries(directoryd avahi++ ${ZMQ_LIBS} ${PROTOBUF_LIBS})

add_library(ddclient++ SHARED ddclient/ddclient.cpp ddclient/query.cpp ddclient/register.cpp protobuf/Services.pb.cc)
target_link_libraries(ddclient++ ${ZMQ_LIBS} ${PROTOBUF_LIBS})

add_executable(test_query ddclient/test_query.cpp)
target_link_libraries(test_query ddclient++)

add_executable(test_reg ddclient/test_register.cpp)
target_link_libraries(test_reg ddclient++)

INSTALL(FILES Browser.hpp Registrar.hpp Service.hpp ServiceGroup.hpp Avahi.hpp
        DESTINATION include/avahi++)
INSTALL(FILES Services.proto DESTINATION share/directoryd)
INSTALL(FILES ddclient/query.hpp ddclient/ddclient.hpp ddclient/register.hpp
        DESTINATION include/ddclient++/ddclient)
INSTALL(PROGRAMS ${PROJECT_SOURCE_DIR}/bin/directoryd
                 DESTINATION bin)
INSTALL(TARGETS avahi++ LIBRARY DESTINATION lib)
INSTALL(TARGETS ddclient++ LIBRARY DESTINATION lib)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Wrapper library for Avahi")
SET(CPACK_PACKAGE_VENDOR "rotap")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_STRIP_FILES "bin/directoryd" "libavahi++.so" "libddclient++.so")
SET(CPACK_SOURCE_STRIP_FILES "")
include(CPack)

