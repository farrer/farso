
cmake_minimum_required(VERSION 2.8)

# Define project and its current version
set(PACKAGE farso)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
if(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
else(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
endif(${VERSION_PATCH})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

# Define some options
option(FARSO_STATIC "Static build" FALSE)
option(FARSO_DEBUG "Enable debug symbols" FALSE)

# Some compiler options
if(UNIX)
   if(${FARSO_DEBUG})
      add_definitions(-Wall -g)
   else(${FARSO_DEBUG})
      add_definitions(-Wall)
   endif(${FARSO_DEBUG})
endif(UNIX)

#Find Packages
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
   # For android, must build statically.
   set(FARSO_STATIC True)
   set(OGRE_STATIC True)
   # Android doesn't have OpenGL.
   set(FARSO_HAS_OPENGL 0)
   # Use Tremor instead of Vorbis and Vorbis File
   FIND_PACKAGE(Tremor REQUIRED)
   include_directories(${VORBISFILE_INCLUDE_DIR})
else(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
   # SDL2 is only used on non android systems
   FIND_PACKAGE(SDL2 REQUIRED)
   include_directories(${SDL2_INCLUDE_DIR})
   # SDL_Image
   FIND_PACKAGE(SDL2_image REQUIRED)
   include_directories(${SDL2_IMAGE_INCLUDE_DIR})
   # OpenGL is only a valid target when not on mobile
   FIND_PACKAGE(OpenGL REQUIRED)
   include_directories(${OPENGL_INCLUDE_DIR})
   set(FARSO_HAS_OPENGL 1)
   # Vorbis and vorbis file 
   FIND_PACKAGE(Vorbis REQUIRED)
   include_directories(${VORBIS_INCLUDE_DIR})
   FIND_PACKAGE(Vorbisfile REQUIRED)
   include_directories(${VORBISFILE_INCLUDE_DIR})
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")

FIND_PACKAGE(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})
FIND_PACKAGE(Ogg REQUIRED)
include_directories(${OGG_INCLUDE_DIR})
FIND_PACKAGE(Kobold REQUIRED)
include_directories(${KOBOLD_INCLUDE_DIR})
FIND_PACKAGE(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Find optional Ogre3d and dependencies
FIND_PACKAGE(OGRE)
if(${OGRE_FOUND})
   set(FARSO_HAS_OGRE 1)
   include_directories(${OGRE_INCLUDE_DIR})
   
   # With Ogre3D, Goblin is mandatory
   FIND_PACKAGE(Goblin REQUIRED)
   include_directories(${GOBLIN_INCLUDE_DIR})

   if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
      include_directories(${OGRE_INCLUDE_DIR}/RenderSystems/GLES2/)
      include_directories(${OGRE_INCLUDE_DIR}/RenderSystems/GLES2/Android/)
   endif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")

else(${OGRE_FOUND})
   set(FARSO_HAS_OGRE 0)
endif(${OGRE_FOUND})

# Include directories: src and the binary src (as generating config.h there)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

# Generate dynamic info
set(FARSO_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/src/farsoconfig.h)
configure_file("./src/farsoconfig.h.in" "./src/farsoconfig.h")

# Include headers and files
include(sources.cmake)

# Build the library with defined mode (static or shared)
if(${FARSO_STATIC})
   add_library(farso ${FARSO_FULL_SOURCES} ${FARSO_FULL_HEADERS})
else(${FARSO_STATIC})
   add_library(farso SHARED ${FARSO_FULL_SOURCES} ${FARSO_FULL_HEADERS} )
endif(${FARSO_STATIC})

set_target_properties(farso PROPERTIES VERSION ${VERSION}
                             SOVERSION ${VERSION_MAJOR} )

# install the include files and created library.
install(FILES ${FARSO_CONFIG_FILE} DESTINATION include/farso)
install(FILES ${FARSO_HEADERS} DESTINATION include/farso)
if(${FARSO_HAS_OGRE})
   install(FILES ${FARSO_OGRE_HEADERS} DESTINATION include/farso/ogre3d)
endif(${FARSO_HAS_OGRE})
install(TARGETS farso DESTINATION lib)

# Print configuration options
message("\n******************************************************")
message("Farso building options: ")
if(${FARSO_HAS_OGRE})
   message("   Ogre renderer: enabled")
else(${FARSO_HAS_OGRE})
   message("   Ogre renderer: disabled")
endif(${FARSO_HAS_OGRE})
if(${FARSO_HAS_OPENGL})
   message("   OpenGL renderer: enabled")
else(${FARSO_HAS_OPENGL})
   message("   OpenGL renderer: disabled")
endif(${FARSO_HAS_OPENGL})
if(${FARSO_HAS_EXAMPLE})
   message("   Build examples: yes")
else(${FARSO_HAS_EXAMPLE})
   message("   Build examples: no")
endif(${FARSO_HAS_EXAMPLE})
if(${FARSO_STATIC})
   message("   Static build")
else(${FARSO_STATIC})
   message("   Shared build")
endif(${FARSO_STATIC})
message("******************************************************\n")

