cmake_minimum_required(VERSION 3.5...4.0)
project(game.shader.presets)

include(CheckIncludeFiles)

# --- Add-on Dependencies ------------------------------------------------------

find_package(Kodi REQUIRED)

include_directories(${KODI_INCLUDE_DIR}
                    ${KODI_INCLUDE_DIR}/.. # Hack way with "/..", need bigger Kodi cmake rework to match right include ways (becomes done in future)
                    ${PROJECT_SOURCE_DIR}/src/depends/libretro-common/include
                    ${PROJECT_SOURCE_DIR}/src/depends
                    ${PROJECT_SOURCE_DIR}/src)

set(SHADERPRESETS_SOURCES
  src/addon.cpp
  src/depends/gfx/video_shader_parse.c
  src/depends/libretro-common/compat/compat_posix_string.c
  src/depends/libretro-common/compat/compat_strl.c
  src/depends/libretro-common/encodings/encoding_utf.c
  src/depends/libretro-common/file/config_file.c
  src/depends/libretro-common/file/file_path.cpp
  src/depends/libretro-common/lists/string_list.c
  src/depends/libretro-common/streams/file_stream.cpp
  src/depends/libretro-common/string/stdstring.c
  src/log/Log.cpp
  src/log/LogAddon.cpp
  src/log/LogConsole.cpp
  src/utils/RarchTranslator.cpp
  src/utils/StringUtils.cpp
  src/utils/URIUtils.cpp
)

set(SHADERPRESETS_HEADERS
  src/addon.h
  src/depends/gfx/video_shader_parse.h
  src/depends/libretro-common/include/array/rhmap.h
  src/depends/libretro-common/include/boolean.h
  src/depends/libretro-common/include/compat/msvc.h
  src/depends/libretro-common/include/compat/posix_string.h
  src/depends/libretro-common/include/compat/strl.h
  src/depends/libretro-common/include/encodings/utf.h
  src/depends/libretro-common/include/file/config_file.h
  src/depends/libretro-common/include/file/file_path.h
  src/depends/libretro-common/include/lists/string_list.h
  src/depends/libretro-common/include/retro_inline.h
  src/depends/libretro-common/include/retro_miscellaneous.h
  src/depends/libretro-common/include/streams/file_stream.h
  src/depends/libretro-common/include/string/stdstring.h
  src/log/ILog.h
  src/log/Log.h
  src/log/LogAddon.h
  src/log/LogConsole.h
  src/utils/RarchTranslator.h
  src/utils/StringUtils.h
  src/utils/URIUtils.h
)

# Fix deprecated strdup breaking UWP builds
if (MSVC)
  add_definitions(-Dstrdup=_strdup)
endif()

if(CORE_SYSTEM_NAME STREQUAL osx OR CORE_SYSTEM_NAME STREQUAL darwin_embedded)
  list(APPEND DEPLIBS "-framework CoreFoundation")
endif()

if(CORE_SYSTEM_NAME STREQUAL windows)
  set(SHADERPRESETS_CUSTOM_DATA ${CMAKE_CURRENT_SOURCE_DIR}/libretro/hlsl)
else()
  set(SHADERPRESETS_CUSTOM_DATA ${CMAKE_CURRENT_SOURCE_DIR}/libretro/glsl)
endif()

build_addon(game.shader.presets SHADERPRESETS DEPLIBS)

include(CPack)
