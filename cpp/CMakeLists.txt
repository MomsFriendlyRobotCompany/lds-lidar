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

include_directories(.)
link_directories(build)
list(APPEND ALL_LIBS "${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY}")
# list(APPEND ALL_LIBS "${PROJECT_NAME}")

add_library(${PROJECT_NAME} SHARED lds_driver.cpp SerialPort.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY})

add_executable(lds-test main.cpp)
target_link_libraries(lds-test ${ALL_LIBS})
