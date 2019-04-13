cmake_minimum_required(VERSION 3.10)
PROJECT("lds")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

# Add local -----------------------------------------------------
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Build Driver library ----------------------------------------------
find_package(Boost REQUIRED system)

include_directories(. lds)
link_directories(build)
list(APPEND ALL_LIBS "${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY}")

# LDS Library ---------------------------------------------------------
add_library(${PROJECT_NAME} SHARED lds.cpp SerialPort.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY})

add_executable(lds-test main.cpp)
target_link_libraries(lds-test ${PROJECT_NAME})

# Debian Package ------------------------------------------------------
# kevin@dalek build $ dpkg -c lds-0.1.0-Linux.deb
# drwxr-xr-x root/root         0 2019-04-13 10:05 ./usr/
# drwxr-xr-x root/root         0 2019-04-13 10:05 ./usr/local/
# drwxr-xr-x root/root         0 2019-04-13 10:05 ./usr/local/include/
# drwxr-xr-x root/root         0 2019-04-13 10:05 ./usr/local/include/lds/
# -rw-r--r-- root/root       510 2019-04-13 09:25 ./usr/local/include/lds/lds.h
# drwxr-xr-x root/root         0 2019-04-13 10:05 ./usr/local/lib/
# -rw-r--r-- root/root    297040 2019-04-13 10:04 ./usr/local/lib/liblds.so

set(CPACK_GENERATOR "DEB")
find_program(DPKG "dpkg")

set(CPACK_GENERATOR "DEB")
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armhf")  # RPi
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Me <support@example.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-dev")
set(CPACK_PACKAGE_DESCRIPTION "LDS-01 LIDAR driver")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LDS-01 LIDAR driver C++")
set(CPACK_PACKAGE_CONTACT "Kevin Walchko")
# set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.html")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
# set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.html")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

include(CPack)

# Install ------------------------------------------------------------
install(FILES lds/lds.hpp DESTINATION include/lds)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
