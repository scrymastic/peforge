cmake_minimum_required(VERSION 3.14)
project(peforge LANGUAGES CXX)

# Create the static library
add_library(peforge STATIC
    src/pe_headers.cpp
    src/caves.cpp
    src/resource_util.cpp
    src/common.cpp
)

# Set the include directories
# This allows #include <peforge.hpp> and #include <peforge/xxx.hpp>
target_include_directories(peforge PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set C++ standard
set_target_properties(peforge PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

# Install rules
install(TARGETS peforge
    EXPORT peforgeTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# Export the package for use with find_package
install(EXPORT peforgeTargets
    FILE peforgeTargets.cmake
    NAMESPACE peforge::
    DESTINATION lib/cmake/peforge
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/peforgeConfigVersion.cmake"
    VERSION "1.0.0"
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/peforgeConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/peforgeConfigVersion.cmake"
    DESTINATION lib/cmake/peforge
) 