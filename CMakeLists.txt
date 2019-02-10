cmake_minimum_required(VERSION 3.9)
project(registro VERSION 1.3.2)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# Remove old variables from cache
unset(PROJECT_SOURCE_VERSION CACHE)
unset(PROJECT_GIT_REF CACHE)
unset(PROJECT_BUILD_DATE CACHE)
unset(PROJECT_BUILD_TIME CACHE)

# Get project version
if(EXISTS "${PROJECT_SOURCE_DIR}/.git/HEAD")
    file(READ "${PROJECT_SOURCE_DIR}/.git/HEAD" PROJECT_SOURCE_VERSION)
    if("${PROJECT_SOURCE_VERSION}" MATCHES "^ref:")
        string(REGEX REPLACE "^ref: *([^ \n\r]*).*" "\\1" PROJECT_GIT_REF "${PROJECT_SOURCE_VERSION}")
        file(READ "${PROJECT_SOURCE_DIR}/.git/${PROJECT_GIT_REF}" PROJECT_SOURCE_VERSION)
    endif()
    string(STRIP "${PROJECT_SOURCE_VERSION}" PROJECT_SOURCE_VERSION)
endif()

# Get build date
if(WIN32)
    # It doesn't work on this platform
else()
    execute_process(COMMAND "date" "+%d %b %Y/%H:%M:%S" OUTPUT_VARIABLE DATE_TIME)
    string(REGEX REPLACE "([^/]*)/.*" "\\1" PROJECT_BUILD_DATE "${DATE_TIME}")
    string(REGEX REPLACE "[^/]*/([0-9:]*).*" "\\1" PROJECT_BUILD_TIME "${DATE_TIME}")
endif()

configure_file(config.h.in config.h)

# Declaring files
set( SOURCES
     sources/main.cpp
     sources/registro.cpp
     sources/createDB.cpp
     sources/delSBTS.cpp
     sources/upTS.cpp
     sources/addMK.cpp 
     sources/upMK.cpp
     sources/about.cpp
     sources/path.cpp
)
set( HEADERS
     sources/registro.h 
     sources/createDB.h
     sources/delSBTS.h  
     sources/upTS.h  
     sources/addMK.h 
     sources/upMK.h  
     sources/about.h  
     sources/path.h
)
set( RES
     resources/res.qrc
)

# compile
if(WIN32)
    add_executable(registro WIN32 ${SOURCES} ${RES})
endif()

if(UNIX)
    add_executable(registro ${SOURCES} ${RES})
endif()

target_link_libraries(registro Qt5::Widgets Qt5::Core Qt5::Sql Qt5::PrintSupport)
