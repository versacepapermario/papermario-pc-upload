cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

# Set the project version and language
project(PaperShip VERSION 0.1.0 LANGUAGES C CXX ASM)
include(FetchContent)

include(cmake/papership-cvars.cmake)
include(cmake/lus-cvars.cmake)

set(PROJECT_BUILD_NAME "Star Haven Alpha" CACHE STRING "" FORCE)
set(PROJECT_TEAM "PaperShip Team" CACHE STRING "" FORCE)

# Get Git Information
execute_process(
    COMMAND git branch --show-current
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CMAKE_PROJECT_GIT_BRANCH "${GIT_BRANCH}" CACHE STRING "Git branch" FORCE)

execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(SUBSTRING "${GIT_COMMIT_HASH}" 0 7 SHORT_COMMIT_HASH)

set(CMAKE_PROJECT_GIT_COMMIT_HASH "${SHORT_COMMIT_HASH}" CACHE STRING "Git commit hash" FORCE)

execute_process(
    COMMAND git describe --tags --abbrev=0 --exact-match HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT GIT_COMMIT_TAG)
    set(GIT_COMMIT_TAG "" CACHE STRING "Git commit tag" FORCE)
endif()

set(CMAKE_PROJECT_GIT_COMMIT_TAG "${GIT_COMMIT_TAG}" CACHE STRING "Git commit tag" FORCE)

if(APPLE)
  enable_language(OBJCXX)
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(YAML_CPP_STATIC_DEFINE ON)
add_compile_definitions(YAML_CPP_STATIC_DEFINE)

if (WIN32)
include(cmake/automate-vcpkg.cmake)
set(VCPKG_TRIPLET x64-windows-static)
set(VCPKG_TARGET_TRIPLET x64-windows-static)
vcpkg_bootstrap()
vcpkg_install_packages(zlib bzip2 libzip libpng sdl2 glew glfw3 nlohmann-json tinyxml2 spdlog)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

if (MSVC)
set(CPP "${CMAKE_C_COMPILER}" "/EP")
else()
set(CPP "${CMAKE_C_COMPILER}" "-E" "-P" "-Wno-trigraphs" "-x" "c")
endif()

################################################################################
# Sources
################################################################################
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/port/build.c.in ${CMAKE_CURRENT_SOURCE_DIR}/port/build.c @ONLY)

################################################################################
# Platform arch detection
################################################################################
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(NOT CMAKE_VS_PLATFORM_NAME)
        set(CMAKE_VS_PLATFORM_NAME "x64")
    endif()
    message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

    if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
        OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"))
        message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
    endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Global configuration types
################################################################################
if (CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
set(CMAKE_C_FLAGS_DEBUG "-g -ffast-math -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ffast-math -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -ffast-math -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -DNDEBUG")
add_compile_definitions(
    _LARGEFILE64_SOURCE=1
    _POSIX_C_SOURCE=200809L
)
else()
set(CMAKE_C_FLAGS_RELEASE "-O1 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O1 -DNDEBUG")
set(CMAKE_OBJCXX_FLAGS_RELEASE "-O1 -DNDEBUG")
endif()

# Set game compilation version
set(VERSION us)
set(USE_NETWORKING OFF)
set(SKIP_XCODE_VERSION_CHECK ON)

# Global compile definitions (shared with subprojects including libultraship)
# GBI_FLOATS MUST be global so libultraship and our code agree on Mtx types
# (Mtx = MtxF when GBI_FLOATS=1; C++ name mangling must match across libraries)
add_compile_definitions(
    _USE_MATH_DEFINES
    CIMGUI_DEFINE_ENUMS_AND_STRUCTS
    IMGUI_DEFINE_MATH_OPERATORS
    ENABLE_OPENGL=1
    GBI_FLOATS=1
    NO_OTR_TEXTURES=1
)

# Paper Mario-specific definitions (applied to PaperShip target only, after add_executable)
# These must NOT leak into Torch/libgfxd/libultraship subprojects
set(PM64_COMPILE_DEFINITIONS
    VERSION_US=1
    ENABLE_RUMBLE=1
    F3DEX_GBI_2=1
    GBI_FLOATS=1
    _LANGUAGE_C
    AVOID_UB=1
    PORT=1
)

# Find necessary libraries
if (UNIX AND NOT APPLE)
find_package(OpenGL REQUIRED)
endif()

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/properties.h.in ${CMAKE_CURRENT_SOURCE_DIR}/properties.h @ONLY)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${DIRECTORIES_TO_INCLUDE}
)

# Collect source files
# Paper Mario decomp sources
file(GLOB_RECURSE PM64_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/*.h"
    "src/*.c"
)

# Port-specific C++ sources
file(GLOB_RECURSE PORT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "port/*.h"
    "port/*.c"
    "port/*.cpp"
    "port/*.mm"
)

# Include headers
file(GLOB_RECURSE INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "include/**/*"
)

set(ALL_FILES ${PM64_SRC_FILES} ${PORT_FILES} ${INCLUDE_FILES})

# Exclude files that are replaced by libultraship or our port layer
# N64 OS implementation files (we use libultraship instead)
list(FILTER ALL_FILES EXCLUDE REGEX "src/os/.*")
# Assembly files
list(FILTER ALL_FILES EXCLUDE REGEX ".*.s$")
# NuSystem implementation (we provide shims)
list(FILTER ALL_FILES EXCLUDE REGEX "src/nusys/.*")
# .inc.c files are #included by parent .c files, not compiled directly
list(FILTER ALL_FILES EXCLUDE REGEX "\\.inc\\.c$")
# effect_table.c is #included by effects.c, not compiled directly
list(FILTER ALL_FILES EXCLUDE REGEX "include/effects/effect_table\\.c$")
# .gfx.inc.c and .vtx.inc.c files (display list data includes)
list(FILTER ALL_FILES EXCLUDE REGEX "\\.gfx\\.inc\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "\\.vtx\\.inc\\.c$")
# KZN2 unused_gfx.c is entirely generated ROM vertex/display list data
list(FILTER ALL_FILES EXCLUDE REGEX "src/battle/area/kzn2/anim/unused_gfx\\.c$")
# crash_screen.c is N64-only fault handler (not needed on PC)
list(FILTER ALL_FILES EXCLUDE REGEX "src/crash_screen\\.c$")
# filemenu_selectlanguage.c is PAL-only (not needed for US build)
list(FILTER ALL_FILES EXCLUDE REGEX "src/filemenu/filemenu_selectlanguage\\.c$")
# level_up_letters_*.c are PAL-only overlay files (not in US ROM)
list(FILTER ALL_FILES EXCLUDE REGEX "src/level_up_letters_en_de\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "src/level_up_letters_fr\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "src/level_up_letters_es\\.c$")
# starpoint_de/es/fr.c are PAL-only overlay files (not in US ROM)
# starpoint_en.c is also a PAL overlay; US data is already in starpoint.c (#if !VERSION_PAL)
list(FILTER ALL_FILES EXCLUDE REGEX "src/starpoint_de\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "src/starpoint_es\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "src/starpoint_fr\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "src/starpoint_en\\.c$")
# src/os/ contains N64 OS implementations — we use os_stubs.c + libultraship + NuSystemShims instead
list(FILTER ALL_FILES EXCLUDE REGEX "src/os/")
# These .c files are #included from their parent npc.c (not standalone compilation units)
list(FILTER ALL_FILES EXCLUDE REGEX "area_dro/dro_02/npc_merlee\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "area_dro/dro_02/npc_moustafa\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "area_dro/dro_01/npc_hint_dryite\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "area_dro/dro_01/npc_hint_dryite_companion\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "area_dro/dro_01/npc_composer\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "area_dro/dro_01/npc_shop_owner\\.c$")
list(FILTER ALL_FILES EXCLUDE REGEX "area_kzn/kzn_19/kzn_19_anim[0-9]+\\.c$")

# Create source groups for IDE organization
foreach(source IN LISTS ALL_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_adjusted "${source_path}")
    source_group("${source_path_adjusted}" FILES "${source}")
endforeach()

add_executable(${PROJECT_NAME} ${ALL_FILES})

# Apply Paper Mario-specific defines ONLY to our target (not subprojects)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PM64_COMPILE_DEFINITIONS})

################################################################################
# MSVC runtime library
################################################################################
if (MSVC)
  if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	  get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
	  if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
		  string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
			  $<$<CONFIG:Debug>:
                MultiThreadedDebug
			  >
			  $<$<CONFIG:Release>:
                MultiThreaded
			  >
			  $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
		  )
	  elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
		  string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
			  $<$<CONFIG:Debug>:
				  MultiThreadedDebug
			  >
			  $<$<CONFIG:Release>:
				  MultiThreaded
			  >
			  $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
		  )
	  endif()
	  set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})
  endif()
endif()

#==============================================================================#
# Libultraship Integration                                                     #
#==============================================================================#

set(EXCLUDE_MPQ_SUPPORT TRUE CACHE BOOL "")
set(ENABLE_EXP_AUTO_CONFIGURE_CONTROLLERS ON CACHE BOOL "")
add_compile_definitions(EXCLUDE_MPQ_SUPPORT)

if(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    find_package(SDL2)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/include
  ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/include/libultraship
  ${SDL2_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${dr_libs_SOURCE_DIR}
)

add_subdirectory(libultraship ${CMAKE_CURRENT_SOURCE_DIR}/libultraship)
add_dependencies(${PROJECT_NAME} libultraship)
target_link_libraries(${PROJECT_NAME} PRIVATE libultraship)

# Skip OTR resource handlers in Fast3D interpreter — we load assets from ROM,
# not from OTR archives. OTR opcodes (0x20-0x40) in display lists are garbage data.
target_compile_definitions(libultraship PRIVATE SKIP_OTR_HANDLERS=1)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
            "_CRT_SECURE_NO_WARNINGS;"
            "ENABLE_DX11;"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "INCLUDE_GAME_PRINTF;"
        "UNICODE;"
        "_UNICODE;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS;"
    )
    add_compile_definitions(STORMLIB_NO_AUTO_LINK)
	set(STORMLIB_NO_AUTO_LINK ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|Clang|AppleClang")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		"$<$<CONFIG:Debug>:"
			"_DEBUG"
		">"
		"$<$<CONFIG:Release>:"
			"NDEBUG"
		">"
		"SPDLOG_ACTIVE_LEVEL=0;"
		"_CONSOLE;"
		"_CRT_SECURE_NO_WARNINGS;"
		"UNICODE;"
		"_UNICODE"
	)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(ADDITIONAL_LIBRARY_DEPENDENCIES "")
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    find_package(SDL2)
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        -lglad
        SDL2::SDL2
    )
else()
 	set(ADDITIONAL_LIBRARY_DEPENDENCIES "")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "NintendoSwitch")
    # Switch-specific setup would go here
else()
option(BUILD_PM64 "Build with Paper Mario 64 support" ON)
add_subdirectory(Torch)
# Note: Torch is an executable (asset extraction tool), not a library.
# It's built as a tool dependency but not linked into the main executable.
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /w;
            /Od
        >
        $<$<CONFIG:Release>:
            /Oi;
            /Gy;
            /W3
        >
        /wd4024 /wd4047 /wd4090 /wd4101 /wd4133 /wd4244
        /sdl-;
        /permissive-;
        /MP;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
  target_compile_options(${PROJECT_NAME} PRIVATE  $<$<CONFIG:Debug>:/ZI;>)
  target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO;
            /FORCE:MULTIPLE
        >
        /MANIFEST:NO;
        /DEBUG;
        /SUBSYSTEM:WINDOWS
    )

  foreach (fentry
    CMAKE_C_FLAGS
    CMAKE_CXX_FLAGS
  )
    string (REGEX REPLACE "/RTC(su|[1su])" "" ${fentry} "${${fentry}}")
  endforeach(fentry)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wno-error
            -Wno-return-type
            -Wno-unused-parameter
            -Wno-unused-function
            -Wno-unused-variable
            -Wno-missing-field-initializers
            -Wno-parentheses
            -Wno-narrowing
            -Wno-missing-braces
            -Wno-int-conversion
            $<$<COMPILE_LANGUAGE:C>:
                -Wno-implicit-function-declaration
                -Wno-incompatible-pointer-types
                -Wno-initializer-not-constant
                -Wno-incompatible-library-redeclaration
                -Wno-macro-redefined
            >
            $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
            $<$<COMPILE_LANGUAGE:CXX>:
                -Wno-c++11-narrowing
                -Wno-deprecated-enum-enum-conversion
            >
            -pthread
        )
        target_link_options(${PROJECT_NAME} PRIVATE -pthread)
    elseif (CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
      target_compile_options(${PROJECT_NAME} PRIVATE
          -Wall -Wextra -Wno-error
          -Wno-return-type
          -Wno-unused-parameter
          -Wno-unused-function
          -Wno-unused-variable
          -Wno-missing-field-initializers
          -Wno-parentheses
          -Wno-narrowing
          -Wno-missing-braces
          -Wno-int-conversion
          $<$<COMPILE_LANGUAGE:C>:
              -Wno-implicit-function-declaration
              -Wno-incompatible-pointer-types
              -fpermissive
          >
          $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
          $<$<COMPILE_LANGUAGE:CXX>:
              -Wno-c++11-narrowing
              -Wno-deprecated-enum-enum-conversion
          >
          -pthread
      )
      target_link_options(${PROJECT_NAME} PRIVATE -pthread)
  else()
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(CPU_OPTION -msse2 -mfpmath=sse)
      elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(CPU_OPTION -mcpu=native)
      endif()

      target_compile_options(${PROJECT_NAME} PRIVATE
          -Wall -Wextra -Wno-error
          -Wno-unused-parameter
          -Wno-unused-function
          -Wno-unused-variable
          -Wno-missing-field-initializers
          -Wno-parentheses
          -Wno-narrowing
          -Wno-missing-braces
          -Wno-int-conversion
          $<$<COMPILE_LANGUAGE:C>:
              -Werror-implicit-function-declaration
              -Wno-incompatible-pointer-types
              -fpermissive
          >
          $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
          $<$<COMPILE_LANGUAGE:CXX>:-Wno-deprecated-enum-enum-conversion>
          -pthread
          ${CPU_OPTION}
      )
      target_link_options(${PROJECT_NAME} PRIVATE
          -pthread
          -Wl,-export-dynamic
      )
  endif()
endif()

################################################################################
# Post-build: copy asset yamls and config
################################################################################
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMENT "Copying asset yamls..."
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/config.yml" "$<TARGET_FILE_DIR:PaperShip>/config.yml"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/" "$<TARGET_FILE_DIR:PaperShip>/assets/"
)

################################################################################
# Torch asset extraction
################################################################################
if(NOT CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
include(ExternalProject)
ExternalProject_Add(TorchExternal
	PREFIX TorchExternal
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/Torch
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/Torch
)
ExternalProject_Get_Property(TorchExternal install_dir)
if (MSVC)
  set(TORCH_EXECUTABLE ${install_dir}/src/TorchExternal-build/$<CONFIGURATION>/torch)
else()
  set(TORCH_EXECUTABLE ${install_dir}/src/TorchExternal-build/torch)
endif()
add_custom_target(
    ExtractAssets
    DEPENDS TorchExternal
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${TORCH_EXECUTABLE} o2r baserom.us.z64 -u ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/pm64.o2r" "${CMAKE_BINARY_DIR}/pm64.o2r"
)

add_custom_target(
    GeneratePortO2R
    DEPENDS TorchExternal
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${TORCH_EXECUTABLE} pack assets/port papership.o2r o2r -u ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
)

find_program(CURL NAMES curl DOC "Path to the curl program. Used to download files.")
execute_process(COMMAND ${CURL} -sSfL https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt -o ${CMAKE_BINARY_DIR}/gamecontrollerdb.txt OUTPUT_VARIABLE RESULT)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  install(FILES "${CMAKE_BINARY_DIR}/papership.o2r" DESTINATION . COMPONENT ${PROJECT_NAME})
  install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets/" DESTINATION ./assets COMPONENT ${PROJECT_NAME})
  install(FILES "${CMAKE_BINARY_DIR}/config.yml" DESTINATION . COMPONENT ${PROJECT_NAME})
endif()

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ./debug COMPONENT ${PROJECT_NAME})
  install(FILES "${CMAKE_BINARY_DIR}/papership.o2r" DESTINATION . COMPONENT ${PROJECT_NAME})
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
add_custom_target(CreateOSXIcons
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/macosx/papership.iconset
    COMMAND sips -z 16 16     logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_16x16.png
    COMMAND sips -z 32 32     logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_16x16@2x.png
    COMMAND sips -z 32 32     logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_32x32.png
    COMMAND sips -z 64 64     logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_32x32@2x.png
    COMMAND sips -z 128 128   logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_128x128.png
    COMMAND sips -z 256 256   logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_128x128@2x.png
    COMMAND sips -z 256 256   logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_256x256.png
    COMMAND sips -z 512 512   logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_256x256@2x.png
    COMMAND sips -z 512 512   logo.png --out ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_512x512.png
    COMMAND cp                logo.png ${CMAKE_BINARY_DIR}/macosx/papership.iconset/icon_512x512@2x.png
    COMMAND iconutil -c icns -o ${CMAKE_BINARY_DIR}/macosx/papership.icns ${CMAKE_BINARY_DIR}/macosx/papership.iconset
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Creating OSX icons ..."
    )
    add_dependencies(${PROJECT_NAME} CreateOSXIcons)
    configure_file("${CMAKE_SOURCE_DIR}/Info.plist" "${CMAKE_BINARY_DIR}/Info.plist" COPYONLY)
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ../MacOS COMPONENT PaperShip)
INSTALL(FILES ${CMAKE_BINARY_DIR}/gamecontrollerdb.txt DESTINATION ../MacOS COMPONENT PaperShip)
INSTALL(FILES ${CMAKE_BINARY_DIR}/papership.o2r DESTINATION ../Resources COMPONENT PaperShip)
INSTALL(FILES ${CMAKE_BINARY_DIR}/config.yml DESTINATION ../Resources COMPONENT PaperShip)
INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/assets/ DESTINATION ../Resources/assets COMPONENT PaperShip)

install(CODE "
    include(BundleUtilities)
    fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/../MacOS/PaperShip\" \"\" \"${dirs}\")
")
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY APPIMAGE_DESKTOP_FILE_TERMINAL YES)
set_property(TARGET ${PROJECT_NAME} PROPERTY APPIMAGE_DESKTOP_FILE "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.desktop")
set_property(TARGET ${PROJECT_NAME} PROPERTY APPIMAGE_ICON_FILE "${CMAKE_SOURCE_DIR}/logo.png")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CPACK_GENERATOR "External")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows|NintendoSwitch")
    set(CPACK_GENERATOR "ZIP")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CPACK_GENERATOR "Bundle")
endif()
