MESSAGE(STATUS "IODB")
file(GLOB iodbSource ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
MESSAGE("DEF ${iodbSource}")
SET(iodbSource ${iodbSource} CACHE INTERNAL "")
