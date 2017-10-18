set(FARSO_SOURCES
src/button.cpp
src/checkbox.cpp
src/clickablepicture.cpp
src/colors.cpp
src/container.cpp
src/controller.cpp
src/cursor.cpp
src/draw.cpp
src/event.cpp
src/fileselector.cpp
src/font.cpp
src/grid.cpp
src/label.cpp
src/labelledpicture.cpp
src/menu.cpp
src/picture.cpp
src/progressbar.cpp
src/rect.cpp
src/scrollbar.cpp
src/scrolltext.cpp
src/skin.cpp
src/spin.cpp
src/stacktab.cpp
src/surface.cpp
src/textentry.cpp
src/textselector.cpp
src/treeview.cpp
src/widget.cpp
src/widgetjsonparser.cpp
src/widgetrenderer.cpp
src/window.cpp
src/sdl/sdlsurface.cpp
src/sdl/sdldraw.cpp
src/sdl/sdljunction.cpp
src/sdl/sdlwidgetrenderer.cpp
)

set(FARSO_HEADERS
src/button.h
src/checkbox.h
src/clickablepicture.h
src/colors.h
src/container.h
src/controller.h
src/cursor.h
src/draw.h
src/event.h
src/eventtype.h
src/fileselector.h
src/font.h
src/grid.h
src/label.h
src/labelledpicture.h
src/menu.h
src/picture.h
src/progressbar.h
src/rect.h
src/scrollbar.h
src/scrolltext.h
src/skin.h
src/spin.h
src/stacktab.h
src/surface.h
src/textentry.h
src/textselector.h
src/treeview.h
src/widget.h
src/widgetjsonparser.h
src/widgetrenderer.h
src/widgeteventlistener.h
src/window.h
src/sdl/sdlsurface.h
src/sdl/sdldraw.h
src/sdl/sdljunction.h
src/sdl/sdlwidgetrenderer.h
)

set(FARSO_FULL_SOURCES ${FARSO_SOURCES})
set(FARSO_FULL_HEADERS ${FARSO_HEADERS})

set(FARSO_OPENGL_SOURCES
src/opengl/opengldraw.cpp
src/opengl/opengljunction.cpp
src/opengl/openglsurface.cpp
src/opengl/openglwidgetrenderer.cpp
)

set(FARSO_OPENGL_HEADERS 
src/opengl/opengldraw.h
src/opengl/opengljunction.h
src/opengl/openglsurface.h
src/opengl/openglwidgetrenderer.h
)

if(${OPENGL_FOUND})
   set(FARSO_FULL_SOURCES ${FARSO_FULL_SOURCES} ${FARSO_OPENGL_SOURCES})
   set(FARSO_FULL_HEADERS ${FARSO_FULL_HEADERS} ${FARSO_OPENGL_HEADERS})
endif(${OPENGL_FOUND})

set(FARSO_OGRE_SOURCES
src/ogre3d/ogredraw.cpp
src/ogre3d/ogrejunction.cpp
src/ogre3d/ogresurface.cpp
src/ogre3d/ogrewidgetmovable.cpp
src/ogre3d/ogrewidgetrenderable.cpp
src/ogre3d/ogrewidgetrenderer.cpp
)

set(FARSO_OGRE_HEADERS
src/ogre3d/ogredraw.h
src/ogre3d/ogrejunction.h
src/ogre3d/ogresurface.h
src/ogre3d/ogrewidgetmovable.h
src/ogre3d/ogrewidgetrenderable.h
src/ogre3d/ogrewidgetrenderer.h
)

if(${OGRE_FOUND})
   set(FARSO_FULL_SOURCES ${FARSO_FULL_SOURCES} ${FARSO_OGRE_SOURCES})
   set(FARSO_FULL_HEADERS ${FARSO_FULL_HEADERS} ${FARSO_OGRE_HEADERS})
endif(${OGRE_FOUND})

set(FARSO_COMMON_EXAMPLE_SOURCES
examples/src/common.cpp
examples/src/jsonloader.cpp
)

set(FARSO_COMMON_EXAMPLE_HEADERS
examples/src/common.h
examples/src/jsonloader.h
)

set(FARSO_SDL_COMMON_SOURCES
examples/src/sdl/sdl_app.cpp
)
set(FARSO_SDL_COMMON_HEADERS
examples/src/sdl/sdl_app.h
)

set(FARSO_SDL_EXAMPLE_SOURCES
examples/src/sdl/sdl_example.cpp
)
set(FARSO_SDL_EXAMPLE_HEADERS
examples/src/sdl/sdl_example.h
)

set(FARSO_SDL_JSON_SOURCES
examples/src/sdl/sdl_jsonloader.cpp
)
set(FARSO_SDL_JSON_HEADERS
examples/src/sdl/sdl_jsonloader.h
)

set(FARSO_OGRE3D_EXAMPLE_SOURCES
examples/src/ogre3d/ogre3d_example.cpp
examples/src/ogre3d/ogre3d_example_main.cpp
)
set(FARSO_OGRE3D_EXAMPLE_HEADERS
examples/src/ogre3d/ogre3d_example.h
)

set(FARSO_OPENGL_COMMON_SOURCES
examples/src/opengl/opengl_app.cpp
)
set(FARSO_OPENGL_COMMON_HEADERS
examples/src/opengl/opengl_app.h
)

set(FARSO_OPENGL_EXAMPLE_SOURCES
examples/src/opengl/opengl_example.cpp
)
set(FARSO_OPENGL_EXAMPLE_HEADERS
examples/src/opengl/opengl_example.h
)

set(FARSO_OPENGL_JSON_SOURCES
examples/src/opengl/opengl_jsonloader.cpp
)
set(FARSO_OPENGL_JSON_HEADERS
examples/src/opengl/opengl_jsonloader.h
)

set(FARSO_HAS_EXAMPLE 0)

if(${OGRE_FOUND})
   # Let's set each ogre library we should link to
   set(FARSO_OGRE_LIBRARIES ${OGRE_LIBRARIES})

   if(${OGRE_VERSION_MAJOR} EQUAL 1)
      # We should use RTSS at version 1.x
      set(FARSO_OGRE_LIBRARIES ${FARSO_OGRE_LIBRARIES} 
                               ${OGRE_RTShaderSystem_LIBRARIES})
   else(${OGRE_VERSION_MAJOR} EQUAL 1)
      # We should use HLMS at version 2.x+
      set(FARSO_OGRE_LIBRARIES ${FARSO_OGRE_LIBRARIES} 
                               ${OGRE_HlmsUnlit_LIBRARIES}
                               ${OGRE_HlmsPbs_LIBRARIES})
   endif(${OGRE_VERSION_MAJOR} EQUAL 1)

   # We try to link woth overlays, if available (if not the variable is
   # unset), as Goblin could had be built with it
   set(FARSO_OGRE_LIBRARIES ${FARSO_OGRE_LIBRARIES} 
                            ${OGRE_Overlay_LIBRARIES})
endif(${OGRE_FOUND})


if(${FARSO_HAS_OGRE_EXAMPLE})
   set(FARSO_EXAMPLE_SOURCES ${FARSO_COMMON_EXAMPLE_SOURCES}
                             ${FARSO_OGRE3D_EXAMPLE_SOURCES})
   set(FARSO_EXAMPLE_HEADERS ${FARSO_COMMON_EXAMPLE_HEADERS}
                             ${FARSO_OGRE3D_EXAMPLE_HEADERS})
   add_executable(farso_ogre3d_example WIN32 
                        ${FARSO_EXAMPLE_SOURCES} 
                        ${FARSO_COMMON_EXAMPLE_HEADERS})
   target_link_libraries(farso_ogre3d_example farso
                        ${GOBLIN_LIBRARY} ${KOSOUND_LIBRARY} ${KOBOLD_LIBRARY}
                        ${FARSO_OGRE_LIBRARIES} 
                        ${FREETYPE_LIBRARIES}
                        ${OPENGL_LIBRARY}
                        ${SDL2_IMAGE_LIBRARY}
                        ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                        ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                        ${OGG_LIBRARY} m
                        ${LIBINTL_LIBRARIES} pthread)
   add_custom_command(TARGET farso_ogre3d_example PRE_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/examples/data 
                      $<TARGET_FILE_DIR:farso_ogre3d_example>/data)
   set(FARSO_HAS_EXAMPLE 1)
endif(${FARSO_HAS_OGRE_EXAMPLE})

if(${OPENGL_FOUND})
   add_executable(farso_opengl_example WIN32 
                        ${FARSO_COMMON_EXAMPLE_SOURCES}
                        ${FARSO_OPENGL_COMMON_SOURCES}
                        ${FARSO_OPENGL_EXAMPLE_SOURCES}
                        ${FARSO_COMMON_EXAMPLE_HEADERS}
                        ${FARSO_OPENGL_COMMON_HEADERS}
                        ${FARSO_OPENGL_EXAMPLE_HEADERS})

   if(${FARSO_HAS_RAPIDJSON})
      add_executable(farso_opengl_jsonloader WIN32 
                           ${FARSO_COMMON_EXAMPLE_SOURCES}
                           ${FARSO_OPENGL_COMMON_SOURCES}
                           ${FARSO_OPENGL_JSON_SOURCES}
                           ${FARSO_COMMON_EXAMPLE_HEADERS}
                           ${FARSO_OPENGL_COMMON_HEADERS}
                           ${FARSO_OPENGL_JSON_HEADERS})
   endif(${FARSO_HAS_RAPIDJSON})

   if(${FARSO_HAS_OGRE_EXAMPLE})
      # Must link with all ogre example dependencies
      set(LIBRARIES farso
                    ${GOBLIN_LIBRARY} ${KOSOUND_LIBRARY} ${KOBOLD_LIBRARY}
                    ${FARSO_OGRE_LIBRARIES} 
                    ${FREETYPE_LIBRARIES}
                    ${OPENGL_LIBRARY}
                    ${SDL2_IMAGE_LIBRARY}
                    ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                    ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                    ${OGG_LIBRARY} m
                    ${LIBINTL_LIBRARIES} pthread)
   else(${FARSO_HAS_OGRE_EXAMPLE})
      if(${FARSO_HAS_OGRE})
         # Must link with ogre3d dependencies
         set(LIBRARIES farso
                       ${KOBOLD_LIBRARY}
                       ${FARSO_OGRE_LIBRARIES}
                       ${FREETYPE_LIBRARIES}
                       ${OPENGL_LIBRARY}
                       ${SDL2_IMAGE_LIBRARY}
                       ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                       m ${LIBINTL_LIBRARIES} pthread)
      else(${FARSO_HAS_OGRE})
         # must link only with OpenGL dependencies
         set(LIBRARIES farso
                       ${KOBOLD_LIBRARY}
                       ${FREETYPE_LIBRARIES}
                       ${OPENGL_LIBRARY}
                       ${SDL2_IMAGE_LIBRARY}
                       ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                       m ${LIBINTL_LIBRARIES} pthread)
      endif(${FARSO_HAS_OGRE})
   endif(${FARSO_HAS_OGRE_EXAMPLE})


   target_link_libraries(farso_opengl_example ${LIBRARIES})

   if(${FARSO_HAS_RAPIDJSON})
      target_link_libraries(farso_opengl_jsonloader ${LIBRARIES})
   endif(${FARSO_HAS_RAPIDJSON})
  
   add_custom_command(TARGET farso_opengl_example PRE_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/examples/data 
                      $<TARGET_FILE_DIR:farso_opengl_example>/data)
   set(FARSO_HAS_EXAMPLE 1)

endif(${OPENGL_FOUND})
   

add_executable(farso_sdl_example WIN32 
               ${FARSO_COMMON_EXAMPLE_SOURCES}
               ${FARSO_SDL_COMMON_SOURCES}
               ${FARSO_SDL_EXAMPLE_SOURCES}
               ${FARSO_COMMON_EXAMPLE_HEADERS}
               ${FARSO_SDL_COMMON_HEADERS}
               ${FARSO_SDL_EXAMPLE_HEADERS})

if(${FARSO_HAS_RAPIDJSON})
   add_executable(farso_sdl_jsonloader WIN32 
                  ${FARSO_COMMON_EXAMPLE_SOURCES}
                  ${FARSO_SDL_COMMON_SOURCES}
                  ${FARSO_SDL_JSON_SOURCES}
                  ${FARSO_COMMON_EXAMPLE_HEADERS}
                  ${FARSO_SDL_COMMON_HEADERS}
                  ${FARSO_SDL_JSON_HEADERS})
endif(${FARSO_HAS_RAPIDJSON})

if(${FARSO_HAS_OGRE_EXAMPLE})
   # Must link with all ogre example dependencies
   set(LIBRARIES farso
                 ${GOBLIN_LIBRARY} ${KOSOUND_LIBRARY} ${KOBOLD_LIBRARY}
                 ${FARSO_OGRE_LIBRARIES} 
                 ${FREETYPE_LIBRARIES}
                 ${OPENGL_LIBRARY}
                 ${SDL2_IMAGE_LIBRARY}
                 ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                 ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                 ${OGG_LIBRARY} m
                 ${LIBINTL_LIBRARIES} pthread)
else(${FARSO_HAS_OGRE_EXAMPLE})
   if(${FARSO_HAS_OGRE})
      # Must link with ogre3d dependencies
      set(LIBRARIES farso
                    ${KOBOLD_LIBRARY}
                    ${FARSO_OGRE_LIBRARIES}
                    ${FREETYPE_LIBRARIES}
                    ${OPENGL_LIBRARY}
                    ${SDL2_IMAGE_LIBRARY}
                    ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                    m ${LIBINTL_LIBRARIES} pthread)
   else(${FARSO_HAS_OGRE})
     # must link only with OpenGL dependencies
      set(LIBRARIES farso
                    ${KOBOLD_LIBRARY}
                    ${FREETYPE_LIBRARIES}
                    ${OPENGL_LIBRARY}
                    ${SDL2_IMAGE_LIBRARY}
                    ${SDL2_LIBRARY} ${OPENAL_LIBRARY} 
                    m ${LIBINTL_LIBRARIES} pthread)
   endif(${FARSO_HAS_OGRE})
endif(${FARSO_HAS_OGRE_EXAMPLE})


target_link_libraries(farso_sdl_example ${LIBRARIES})

if(${FARSO_HAS_RAPIDJSON})
   target_link_libraries(farso_sdl_jsonloader ${LIBRARIES})
endif(${FARSO_HAS_RAPIDJSON})
  
add_custom_command(TARGET farso_sdl_example PRE_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_SOURCE_DIR}/examples/data 
   $<TARGET_FILE_DIR:farso_sdl_example>/data)
set(FARSO_HAS_EXAMPLE 1)


