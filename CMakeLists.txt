cmake_minimum_required(VERSION 3.10.2)

project(WiseEnum VERSION 3.0.0 LANGUAGES CXX)

option(BUILD_EXAMPLES "Build the examples" OFF)
option(BUILD_TESTS "Build the tests" OFF)
option(NO_EXCEPTIONS "Build without exceptions" OFF)

file(GLOB interface_hdrs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
set_property(SOURCE ${interface_hdrs} PROPERTY HEADER_FILE_ONLY ON)

add_library(wise_enum INTERFACE)
target_include_directories(wise_enum INTERFACE $<INSTALL_INTERFACE:include>)

if(NO_EXCEPTIONS)
    target_compile_definitions(wise_enum INTERFACE WISE_ENUM_NO_EXCEPT)
endif()

#
# installation
#

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include/wise_enum")
set(lib_install_dir "lib")
set(pkgconfig_install_dir "lib/pkgconfig")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(pkg_config "${generated_dir}/${PROJECT_NAME}.pc")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "WiseEnum::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_file("cmake/WiseEnum.pc.in" "${pkg_config}" @ONLY)

configure_package_config_file(
        "cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

install(TARGETS wise_enum
        EXPORT "${targets_export_name}"
        INCLUDES DESTINATION "${include_install_dir}"
        ARCHIVE DESTINATION "${lib_install_dir}")

install(FILES ${interface_hdrs} DESTINATION "${include_install_dir}")

install(
        FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}"
)

install(
        FILES "${pkg_config}"
        DESTINATION "${pkgconfig_install_dir}"
)

install(
        EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}"
)

#
# Build examples
#

# To make the example cmake look a little more like if it were a package
# found with find_package
add_library(WiseEnum::wise_enum ALIAS wise_enum)

if(BUILD_EXAMPLES)
    add_executable(example11 examples/example11.x.cpp)
    set_property(TARGET example11 PROPERTY CXX_STANDARD 11)
    target_link_libraries(example11 PRIVATE WiseEnum::wise_enum)

    add_executable(example examples/example.x.cpp)
    target_link_libraries(example PRIVATE WiseEnum::wise_enum)
endif()
