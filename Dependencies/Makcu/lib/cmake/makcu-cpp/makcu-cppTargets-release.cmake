#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "makcu::makcu-cpp_shared" for configuration "Release"
set_property(TARGET makcu::makcu-cpp_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(makcu::makcu-cpp_shared PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/makcu-cpp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/makcu-cpp.dll"
  )

list(APPEND _cmake_import_check_targets makcu::makcu-cpp_shared )
list(APPEND _cmake_import_check_files_for_makcu::makcu-cpp_shared "${_IMPORT_PREFIX}/lib/makcu-cpp.lib" "${_IMPORT_PREFIX}/bin/makcu-cpp.dll" )

# Import target "makcu::makcu-cpp_static" for configuration "Release"
set_property(TARGET makcu::makcu-cpp_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(makcu::makcu-cpp_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/makcu-cpp.lib"
  )

list(APPEND _cmake_import_check_targets makcu::makcu-cpp_static )
list(APPEND _cmake_import_check_files_for_makcu::makcu-cpp_static "${_IMPORT_PREFIX}/lib/makcu-cpp.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
