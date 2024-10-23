cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra")
set(CMAKE_AUTOMOC ON)


set(QT_REQUIRED_VERSION "6.3.0")
find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Svg Concurrent)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(-DPAINTER_OP)
set(MODE "cli")

set(fdsource testcase.cpp)

add_executable(fuzzTestCase ${fdsource} )


target_link_libraries(fuzzTestCase PUBLIC Qt6::Core Qt6::Gui Qt6::Svg Qt6::Concurrent PRIVATE diceparser_static)
target_include_directories(fuzzTestCase PRIVATE ../../libparser/include ../../libparser/ ../../libparser/node ../../libparser/result)

#target_link_libraries(fuzzTestCase ) libdiceparser.a
