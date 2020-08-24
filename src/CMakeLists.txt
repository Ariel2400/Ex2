cmake_minimum_required(VERSION 3.0.0)
project(ex2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror -Wnon-virtual-dtor -Wshadow)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(ex2.out bmp_parser.cpp bmp_tester.cpp MatrixClass.cpp main.cpp)
target_link_libraries(ex2.out matrix)

# to include the src directory
target_link_directories(ex2.out PUBLIC ${PROJECT_SOURCE_DIR})
# to include the project's directory
target_link_directories(ex2.out PUBLIC ..)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)