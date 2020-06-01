cmake_minimum_required(VERSION 3.16)
project(AVC_Robot)

set(CMAKE_CXX_STANDARD 14)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics audio network window system REQUIRED)
add_executable(AVC_Robot robot.cpp)
target_link_libraries(AVC_Robot sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
