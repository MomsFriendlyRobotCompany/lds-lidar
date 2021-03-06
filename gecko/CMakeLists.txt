cmake_minimum_required(VERSION 3.10)
PROJECT("lds-gecko")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")


list(APPEND ALL_LIBS "")

# Add local -----------------------------------------------------
# include_directories(./include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# kevin ---------------------------------------------------------
# include_directories(/home/kevin/.local/include)
# include_directories(/home/kevin/.local/include/gecko/zmq)
# link_directories(/home/kevin/.local/lib)

# gecko ---------------------------------------------------------
include_directories($ENV{HOME}/github/gecko/src)
include_directories($ENV{HOME}/github)
include_directories($ENV{HOME}/github/gecko)
include_directories($ENV{HOME}/github/gecko/zmq)
link_directories($ENV{HOME}/github/gecko/build)
list(APPEND ALL_LIBS "gecko")

if (NOT APPLE)
    set(msgpack_DIR $ENV{HOME}/.local/lib/cmake/msgpack)
    include_directories($ENV{HOME}/.local/include)
    include_directories($ENV{HOME}/.local/include/gecko/msgpack)
    link_directories($ENV{HOME}/.local/lib)
endif()

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Get msgpack -----------------------------------------------------
if(APPLE)
  pkg_check_modules(MessagePack REQUIRED msgpack)
  if(MessagePack_FOUND)
      include_directories(${MessagePack_INCLUDE_DIR})
      message(STATUS ">> MsgPack Found: ${MessagePack_VERSION}")
      message(STATUS ">> MsgPack: ${MessagePack_LIBRARIES}  ${MessagePack_INCLUDE_DIR}")
      list(APPEND ALL_LIBS ${MessagePack_LIBRARIES})

      # this is a template library, so no cpp
      include_directories(msgpack)

  else()
      message(WARNING "\n<<--- Could not find MsgPack --->>\n")
  endif()
elseif(UNIX)
    find_package(msgpack REQUIRED)
    if(msgpack_FOUND)
        include_directories(${msgpack_INCLUDE_DIR})
        message(STATUS ">> MsgPack Found: ${msgpack_VERSION}")
        message(STATUS ">> MsgPack: ${msgpack_LIBS}  ${msgpack_INCLUDE_DIR}")
        list(APPEND ALL_LIBS ${msgpack_LIBRARIES})

        # this is a template library, so no cpp
        include_directories(msgpack)

    else()
        message(WARNING "\n<<--- Could not find MsgPack --->>\n")
    endif()
endif()

# Get zlib -----------------------------------------------------
# find_package(ZLIB)
# if(ZLIB_FOUND)
#     message(STATUS ">> zlib Found: ${ZLIB_VERSION_STRING}")
#     message(STATUS ">> zlib Found: ${ZLIB_LIBRARIES}")
#     list(APPEND ALL_LIBS ${ZLIB_LIBRARIES})
# else()
#     message(WARNING "\n<<--- Could not find zlib --->>\n")
# endif()

# Threads ---------------------------------------------------------
# find_package(Threads REQUIRED)
# if(Threads_FOUND)
#     message(STATUS ">> Threads found")
#     if(CMAKE_USE_PTHREADS_INIT)
#         message(STATUS " -> Using pthreads")
#     endif()
#     list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
# else()
#     message(WARNING "*** Could not find Threads ***")
# endif()

# ZeroMQ ----------------------------------------------------------
pkg_check_modules(libzmq REQUIRED libzmq)
if(libzmq_FOUND)
    message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
    # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
    # add_compile_definitions(ZMQ_CPP11)
    include_directories(${libzmq_INCLUDE_DIRS})
    # list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})
    list(APPEND ALL_LIBS ${libzmq_LIBRARIES})

    # ZeroMQ CPP ------------------------------------------------------
    # https://github.com/zeromq/cppzmq
    include_directories(zmq)
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

# Build Gecko library ----------------------------------------------
# message(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../cpp)
link_directories(../cpp/build)

# add_executable(pub pub.cpp)
# target_link_libraries(pub ${ALL_LIBS})

# Boost -------------------------------------------------------------
find_package(Boost REQUIRED system)
list(APPEND ALL_LIBS ${Boost_SYSTEM_LIBRARY} lds)

# pub/sub ------------------------------------------------------------
# add_executable(sub sub.cpp)
# target_link_libraries(sub lds)
# target_link_libraries(sub ${ALL_LIBS})
# list(APPEND ALL_LIBS ${PROJECT_NAME})

add_executable(pub pub.cpp)
target_link_libraries(pub ${ALL_LIBS})
# target_link_libraries(lds-test ${ALL_LIBS})

# add_executable(sub sub.cpp)
# target_link_libraries(sub ${ALL_LIBS})
# list(APPEND ALL_LIBS ${PROJECT_NAME})
