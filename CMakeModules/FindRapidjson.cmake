# Locate Rapidjson include headers
# This module defines
# Rapidjson_INCLUDE_DIR, where to find rapidjsonconfig.h

FIND_PATH(RAPIDJSON_INCLUDE_DIR NAMES rapidjson/rapidjson.h
  HINTS
  $ENV{RapidjsonDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/rapidjson
  /usr/include/rapidjson
  /usr/local/include
  /usr/include
  /sw/include/rapidjson # Fink
  /sw/include
  /opt/local/include/rapidjson # DarwinPorts
  /opt/local/include
  /opt/csw/include/kodold # Blastwave
  /opt/csw/include 
  /opt/include/rapidjson
  /opt/include
)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(RAPIDJSON
   REQUIRED_VARS RAPIDJSON_INCLUDE_DIR)
