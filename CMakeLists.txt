cmake_minimum_required(VERSION 3.0.0)
project(Registro VERSION 1.3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)

add_executable(Registro main.cpp registro.h registro.cpp createDB.h createDB.cpp delTS.h delTS.cpp upTS.h upTS.cpp addMK.h addMK.cpp)
target_link_libraries(Registro Qt5::Widgets Qt5::Core Qt5::Sql)