cmake_minimum_required(VERSION 3.15)
project(project1 CXX)



find_package(Boost 1.81.0 COMPONENTS program_options REQUIRED)
find_package(Boost 1.81.0 COMPONENTS thread REQUIRED)


add_library(project1 src/project1.cpp)
target_include_directories(project1 PUBLIC include)


# target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::program_options)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::thread)


set_target_properties(project1 PROPERTIES PUBLIC_HEADER "include/project1.h")
install(TARGETS project1)



enable_testing()
find_package(Catch2 3 REQUIRED)
add_executable(project1_test
    test/dummy.cpp
)

target_include_directories(project1_test PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>)

target_link_libraries(project1_test PUBLIC ${PROJECT_NAME})
target_link_libraries(project1_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(project1_test)
