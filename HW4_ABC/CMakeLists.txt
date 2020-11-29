cmake_minimum_required(VERSION 3.17)
project(HW4_ABC)

set(CMAKE_CXX_STANDARD 14)

add_executable(HW4_ABC main.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OpenMP_CXX_LIBRARIES})
