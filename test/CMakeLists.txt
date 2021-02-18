cmake_minimum_required(VERSION 3.12)
project(wise_enum_tests)
enable_testing()

include(CheckCXXSourceCompiles)

#  report an updated value for recent C++ language standards support. By
#  default, Visual Studio always returns the value "199711L" for the
#  __cplusplus preprocessor macro.
if(MSVC)
  string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
endif()

set(OLD_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
if(MSVC)
    set(CMAKE_REQUIRED_FLAGS /std:c++17)
else()
    set(CMAKE_REQUIRED_FLAGS -std=c++17)
endif()

check_cxx_source_compiles("
#include <string_view>
#include <type_traits>
int main() { return std::is_same_v<std::string_view, int>; }" CAN_COMPILE_17)

set(CMAKE_REQUIRED_FLAGS "${OLD_CMAKE_REQUIRED_FLAGS}")

if (EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
    message(WARN Got conan)
    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif ()

find_package(Catch2 REQUIRED)

if (CAN_COMPILE_17)
    add_executable(wise_enum_test test_main.cpp cxx17.cpp)
    target_compile_features(wise_enum_test PRIVATE cxx_std_17)
    target_include_directories(wise_enum_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
else ()
    add_executable(wise_enum_test test_main.cpp)
endif ()
target_link_libraries(wise_enum_test PRIVATE Catch2::Catch2)

add_executable(wise_enum_test14 test_main.cpp cxx14.cpp)
target_compile_features(wise_enum_test14 PRIVATE cxx_std_14)
target_link_libraries(wise_enum_test14 PRIVATE Catch2::Catch2)
target_include_directories(wise_enum_test14 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

add_test(NAME wise_enum.unit_tests COMMAND wise_enum_test)
add_test(NAME wise_enum.unit_tests14 COMMAND wise_enum_test14)
