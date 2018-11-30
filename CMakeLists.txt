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

if (WIN32)
    add_executable(registro WIN32 main.cpp registro.h registro.cpp createDB.h createDB.cpp delTS.h delTS.cpp upTS.h upTS.cpp addMK.h addMK.cpp upMK.h upMK.cpp about.h about.cpp path.h path.cpp resources/res.qrc)
endif (WIN32)

if (UNIX)
    add_executable(registro main.cpp registro.h registro.cpp createDB.h createDB.cpp delTS.h delTS.cpp upTS.h upTS.cpp addMK.h addMK.cpp upMK.h upMK.cpp about.h about.cpp path.h path.cpp resources/res.qrc)
endif (UNIX)

target_link_libraries(registro Qt5::Widgets Qt5::Core Qt5::Sql Qt5::PrintSupport)
