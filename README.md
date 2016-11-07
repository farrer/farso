# Farso - A GUI toolkit

## About
Farso is a graphical user interface written in C++ with OpenGL and Ogre3D 
renderers.

Farso have the following widgets:  

 * Button
 * CheckBox
 * ClickablePicture
 * Container
 * FileSelector
 * Grid
 * Label
 * LabelledPicture
 * Menu
 * Picture
 * ProgressBar
 * ScrollBar
 * ScrollText
 * StackTab
 * TextEntry
 * TextSelector
 * Window

It's fully skinable and could be extended to support your own widgets.

Farso is DNT's GUI, being this version the active of DNT's Ogre3d port.

## Some History

Farso is by far the oldest Free/Libre project of mime, starting as a DOS
multitasker GUI back in the late 90s (code from this old dummy
learning-how-to-programming pascal era is at
[https://sourceforge.net/projects/farso]). Back in 2005 it got its first major
rewrite to C++ and was used on the former DNT project (code at src/gui folder
of [https://sourceforge.net/projects/dnt/]). When rewriting DNT to Ogre3d
instead of the old/buggy in-house OpenGL renderer, Farso got its last major
rewrite, with lots of improvements and code clean-up / refactoring, which is
hosted here.

## Dependencies

### OpenGL Renderer

 * OpenGL
 * Kobold [https://github.com/farrer/kobold]
 * SDL2 [https://libsdl.org/download-2.0.php]
 * SDL\_image 2.x [https://www.libsdl.org/projects/SDL\_image/]
 * FreeType 2.x [https://www.freetype.org/]

### Ogre3d Renderer

 * Ogre3D 1.10 [http://www.ogre3d.org/]
 * Kobold [https://github.com/farrer/kobold]
 * SDL2 [https://libsdl.org/download-2.0.php]
 * SDL\_image 2.x [https://www.libsdl.org/projects/SDL\_image/]
 * FreeType 2.x [https://www.freetype.org/]

### Ogre3d Example (not required for the library itself)
 Same as Ogre3D renderer plus:

 * Goblin [https://github.com/farrer/goblin]
 * Kosound [https://github.com/farrer/kosound]
 * OpenAL [http://kcat.strangesoft.net/openal.html]
 * Ogg [https://xiph.org/downloads/]
 * Vorbis [https://xiph.org/downloads/]
 * VorbisFile [https://xiph.org/downloads/]

## Building

Usually, you build with the following commands:

**mkdir** build  
**cd** build  
cmake ..  
make  
make install

### Building for Android

To build for Android, the CMake command call must set the needed NDK variables:

cmake -DCMAKE\_TOOLCHAIN\_FILE=../CMakeModules/android.toolchain.cmake -DANDROID\_NDK=path\_to\_Android\_Ndk -DCMAKE\_BUILD\_TYPE=Release -DANDROID\_ABI="armeabi-v7a" -DANDROID\_NATIVE\_API\_LEVEL=12 ..

Change those parameters to your needs.


### Options

There are some options that could be passed to CMake script:

 * FARSO\_DEBUG -> Build the library with debugging symbols;
 * FARSO\_STATIC -> Build a .a static library, instead of the shared one.


