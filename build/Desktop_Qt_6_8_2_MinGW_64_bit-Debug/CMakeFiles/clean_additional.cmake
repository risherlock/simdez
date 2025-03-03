# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\simdez_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\simdez_autogen.dir\\ParseCache.txt"
  "simdez_autogen"
  )
endif()
