# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/yohann1Pg2_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/yohann1Pg2_autogen.dir/ParseCache.txt"
  "yohann1Pg2_autogen"
  )
endif()
