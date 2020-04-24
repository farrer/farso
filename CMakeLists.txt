
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG=1 -D_DEBUG=1")

# Define project and its current version
project(farso)
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
option(FARSO_USE_OGRE_OVERLAY "Use overlays as Farso Ogre3D renderer" FALSE)
option(FARSO_BUILD_SDL_EXAMPLES "Build Farso SDL examples" TRUE)

# Let's assume, until found otherwise, that we can compile the 
# Ogre3D example.
set(FARSO_HAS_OGRE_EXAMPLE 1)

# Some compiler options
if(UNIX)
   if(${FARSO_DEBUG})
      add_definitions(-Wall -g)
   else(${FARSO_DEBUG})
      add_definitions(-Wall)
   endif(${FARSO_DEBUG})
endif(UNIX)

if(APPLE)
   set(FARSO_STATIC True)
   set(OGRE_STATIC True)
endif(APPLE)

#Find Packages
FIND_PACKAGE(Kobold REQUIRED)
include_directories(${KOBOLD_INCLUDE_DIR})
FIND_PACKAGE(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
FIND_PACKAGE(Rapidjson)
if(${RAPIDJSON_FOUND})
   include_directories(${RAPIDJSON_INCLUDE_DIR})
   set(FARSO_HAS_RAPIDJSON 1)
   message("Found")
else(${Rapidjson_FOUND})
   set(FARSO_HAS_RAPIDJSON 0)
   message("Not found")
endif(${RAPIDJSON_FOUND})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
   # For android, must build statically.
   set(FARSO_STATIC True)
   set(OGRE_STATIC True)
   # Android doesn't have OpenGL.
   set(FARSO_HAS_OPENGL 0)
   # Use Tremor instead of Vorbis and Vorbis File
   FIND_PACKAGE(Tremor)
   if(${VORBISFILE_FOUND})
      include_directories(${VORBISFILE_INCLUDE_DIR})
   else(${VORBISFILE_FOUND})
      set(FARSO_HAS_OGRE_EXAMPLE 0)
   endif(${VORBISFILE_FOUND})
else(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
   # SDL2 is only used on non android systems
   FIND_PACKAGE(SDL2 REQUIRED)
   include_directories(${SDL2_INCLUDE_DIR})
   # SDL_Image
   FIND_PACKAGE(SDL2_image REQUIRED)
   include_directories(${SDL2_IMAGE_INCLUDE_DIR})
   # OpenGL is only a valid target when not on mobile
   FIND_PACKAGE(OpenGL)
   if(${OPENGL_FOUND})
      include_directories(${OPENGL_INCLUDE_DIR})
      set(FARSO_HAS_OPENGL 1)
   endif(${OPENGL_FOUND})
   # Vorbis and vorbis file 
   FIND_PACKAGE(Vorbis)
   if(${VORBIS_FOUND})
      include_directories(${VORBIS_INCLUDE_DIR})
   else(${VORBIS_FOUND})
      set(FARSO_HAS_OGRE_EXAMPLE 0)
   endif(${VORBIS_FOUND})
if(NOT APPLE)
   FIND_PACKAGE(Vorbisfile)
   if(${VORBISFILE_FOUND})
      include_directories(${VORBISFILE_INCLUDE_DIR})
   else(${VORBISFILE_FOUND})
      set(FARSO_HAS_OGRE_EXAMPLE 0)
   endif(${VORBISFILE_FOUND})
endif(NOT APPLE)
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")

FIND_PACKAGE(OpenAL)
if(${OPENAL_FOUND})
   include_directories(${OPENAL_INCLUDE_DIR})
else(${OPENAL_FOUND})
   set(FARSO_HAS_OGRE_EXAMPLE 0)
endif(${OPENAL_FOUND})

FIND_PACKAGE(Ogg)
if(${OGG_FOUND})
   include_directories(${OGG_INCLUDE_DIR})
else(${OGG_FOUND})
   set(FARSO_HAS_OGRE_EXAMPLE 0)
endif(${OGG_FOUND})

# Find optional Ogre3d and dependencies
FIND_PACKAGE(OGRE)
if(${OGRE_FOUND})
   set(FARSO_HAS_OGRE 1)
   if(${FARSO_USE_OGRE_OVERLAY})
      set(FARSO_USE_OGRE_OVERLAY 1)
   else(${FARSO_USE_OGRE_OVERLAY})
      if(${OGRE_VERSION_MAJOR} EQUAL 1)
         set(FARSO_USE_OGRE_OVERLAY 1)
      else(${OGRE_VERSION_MAJOR} EQUAL 1)
         set(FARSO_USE_OGRE_OVERLAY 0)
      endif(${OGRE_VERSION_MAJOR} EQUAL 1)
   endif(${FARSO_USE_OGRE_OVERLAY})

   include_directories(${OGRE_INCLUDE_DIR})
   include_directories(${OGRE_INCLUDE_DIR}/Depracated/)
   include_directories(${OGRE_INCLUDE_DIR}/Hlms/Common/)
   
   # With Ogre3D, Goblin is mandatory only for example
   FIND_PACKAGE(Goblin)
   if(${GOBLIN_FOUND})
      include_directories(${GOBLIN_INCLUDE_DIR})
   else(${GOBLIN_FOUND})
      set(FARSO_HAS_OGRE_EXAMPLE 0)
   endif(${GOBLIN_FOUND})

   # And with Goblin, we also need kosound for example
   FIND_PACKAGE(Kosound)
   if(${KOSOUND_FOUND})
      include_directories(${KOSOUND_INCLUDE_DIR})
   else(${KOSOUND_FOUND})
      set(FARSO_HAS_OGRE_EXAMPLE 0)
   endif(${KOSOUND_FOUND})

   # With android, we must set GLES2 Ogre3d renderer (and android path).
   if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
      include_directories(${OGRE_INCLUDE_DIR}/RenderSystems/GLES2/)
      include_directories(${OGRE_INCLUDE_DIR}/RenderSystems/GLES2/Android/)
   endif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")

else(${OGRE_FOUND})
   set(FARSO_HAS_OGRE 0)
   set(FARSO_HAS_OGRE_EXAMPLE 0)
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
if(${FARSO_HAS_OPENGL})
   install(FILES ${FARSO_OPENGL_HEADERS} DESTINATION include/farso/opengl)
endif(${FARSO_HAS_OPENGL})

install(TARGETS farso DESTINATION lib)

# Print configuration options
message("\n******************************************************")
message("Farso building options: ")
if(${FARSO_HAS_OGRE})
   message("   Ogre3d renderer: enabled")
   if(${FARSO_USE_OGRE_OVERLAY})
      message("      Using overlays.")
   endif(${FARSO_USE_OGRE_OVERLAY})
else(${FARSO_HAS_OGRE})
   message("   Ogre3d renderer: disabled")
endif(${FARSO_HAS_OGRE})
if(${FARSO_HAS_OPENGL})
   message("   OpenGL renderer: enabled")
else(${FARSO_HAS_OPENGL})
   message("   OpenGL renderer: disabled")
endif(${FARSO_HAS_OPENGL})
if(${FARSO_HAS_EXAMPLE})
   message("   Build examples: yes")
   if(${FARSO_HAS_OGRE_EXAMPLE})
      message("      Ogre3d example: enabled")
   else(${FARSO_HAS_OGRE_EXAMPLE})
      message("      Ogre3d example: disabled")
   endif(${FARSO_HAS_OGRE_EXAMPLE})

   if(${FARSO_BUILD_SDL_EXAMPLES})
      message("      SDL example: enabled")
   else(${FARSO_BUILD_SDL_EXAMPLES})
      message("      SDL example: disabled")
   endif(${FARSO_BUILD_SDL_EXAMPLES})

   message("      OpenGL example: enabled")
else(${FARSO_HAS_EXAMPLE})
   message("   Build examples: no")
endif(${FARSO_HAS_EXAMPLE})
if(${FARSO_STATIC})
   message("   Static build")
else(${FARSO_STATIC})
   message("   Shared build")
endif(${FARSO_STATIC})
if(${FARSO_DEBUG})
   message("   With debug symbols")
endif(${FARSO_DEBUG})
message("******************************************************\n")

