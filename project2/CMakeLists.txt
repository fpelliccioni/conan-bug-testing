cmake_minimum_required(VERSION 3.15)
project(project2 CXX)


find_package(project1 REQUIRED)

add_library(project2 src/project2.cpp)
target_include_directories(project2 PUBLIC include)


target_link_libraries(${PROJECT_NAME} PUBLIC project1::project1)

set_target_properties(project2 PROPERTIES PUBLIC_HEADER "include/project2.h")
install(TARGETS project2)


enable_testing()
find_package(Catch2 3 REQUIRED)
add_executable(project2_test
    test/dummy.cpp
)

target_include_directories(project2_test PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>)

target_link_libraries(project2_test PUBLIC ${PROJECT_NAME})
target_link_libraries(project2_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(project2_test)
