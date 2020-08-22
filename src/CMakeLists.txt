cmake_minimum_required(VERSION 3.0.0)
project(ex2 VERSION 0.1.0)

include(CTest)
enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(ex2 bmp_tester.cpp bmp_parser.cpp Matrix.cpp)
target_link_libraries(ex2 matrix)
add_compile_options(-Wall -Wextra -pedantic -Werror)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
