#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "jansson" for configuration "MinSizeRel"
set_property(TARGET jansson APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(jansson PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "C"
  IMPORTED_LOCATION_MINSIZEREL "C:/Program Files (x86)/jansson/lib/jansson.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS jansson )
list(APPEND _IMPORT_CHECK_FILES_FOR_jansson "C:/Program Files (x86)/jansson/lib/jansson.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
