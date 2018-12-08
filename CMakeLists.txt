cmake_minimum_required(VERSION 3.9)
project(registro VERSION 1.3)

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
    # It doesn't work for this platform
else()
    execute_process(COMMAND "date" "+%d %b %Y/%H:%M:%S" OUTPUT_VARIABLE DATE_TIME)
    string(REGEX REPLACE "([^/]*)/.*" "\\1" PROJECT_BUILD_DATE "${DATE_TIME}")
    string(REGEX REPLACE "[^/]*/([0-9:]*).*" "\\1" PROJECT_BUILD_TIME "${DATE_TIME}")
endif()

configure_file(config.h.in config.h)

if (WIN32)
    add_executable(registro WIN32 main.cpp registro.h registro.cpp createDB.h createDB.cpp delTS.h delTS.cpp upTS.h upTS.cpp addMK.h addMK.cpp upMK.h upMK.cpp about.h about.cpp path.h path.cpp config.h.in resources/res.qrc)
endif (WIN32)

if (UNIX)
    add_executable(registro main.cpp registro.h registro.cpp createDB.h createDB.cpp delTS.h delTS.cpp upTS.h upTS.cpp addMK.h addMK.cpp upMK.h upMK.cpp about.h about.cpp path.h path.cpp config.h.in resources/res.qrc)
endif (UNIX)

target_link_libraries(registro Qt5::Widgets Qt5::Core Qt5::Sql Qt5::PrintSupport)
