#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "jansson" for configuration "Debug"
set_property(TARGET jansson APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(jansson PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "C:/Program Files (x86)/jansson/lib/jansson_d.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS jansson )
list(APPEND _IMPORT_CHECK_FILES_FOR_jansson "C:/Program Files (x86)/jansson/lib/jansson_d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
