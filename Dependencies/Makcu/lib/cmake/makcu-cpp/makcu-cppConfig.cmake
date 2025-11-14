
include(CMakeFindDependencyMacro)

# Find required dependencies
find_dependency(Threads REQUIRED)

# Platform-specific dependencies
if(UNIX AND NOT APPLE)
    find_dependency(PkgConfig REQUIRED)
    pkg_check_modules(UDEV REQUIRED libudev)
endif()

# Include the targets file
include("${CMAKE_CURRENT_LIST_DIR}/makcu-cppTargets.cmake")

# Create convenient alias - prefer shared library if available, fallback to static
if(TARGET makcu::makcu-cpp_shared)
    if(NOT TARGET makcu::makcu-cpp)
        add_library(makcu::makcu-cpp ALIAS makcu::makcu-cpp_shared)
    endif()
elseif(TARGET makcu::makcu-cpp_static)
    if(NOT TARGET makcu::makcu-cpp)
        add_library(makcu::makcu-cpp ALIAS makcu::makcu-cpp_static)
    endif()
endif()
