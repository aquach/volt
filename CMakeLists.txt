CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)
    
PROJECT(Volt)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

IF(UNIX)
    FIND_PACKAGE(OpenGL REQUIRED)
    FIND_PACKAGE(SDL REQUIRED)
    FIND_PACKAGE(SDL_image REQUIRED)
    set(CMAKE_CXX_FLAGS "-g -Wall -Wno-reorder")
ENDIF() 

IF(UNIX)
    SET(LIBBOX2D ${CMAKE_SOURCE_DIR}/Contrib/Box2D_v2.1.2/Box2D/Box2D/libBox2D.a)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${LIBBOX2D}
        COMMAND cd ${CMAKE_SOURCE_DIR}/Contrib/Box2D_v2.1.2/Box2D/ && cmake . && make
    )
    ADD_CUSTOM_TARGET(Box2D DEPENDS ${LIBBOX2D})
ENDIF()

IF(WIN32)
    INCLUDE_DIRECTORIES(Contrib/Windows)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/Contrib/Windows/lib)
ENDIF()
    
IF(UNIX)    
    SET(VOLT_DEPENDENCIES SDL SDL_image json GL GLU GLEW openal ogg vorbisfile ${LIBBOX2D})
ELSEIF(WIN32)
    SET(VOLT_DEPENDENCIES SDL SDL_image SDLmain json OpenGL32 GLEW32 OpenAL32 glu32
        debug libogg_static_d optimized libogg_static
        debug libvorbis_static_d optimized libvorbis_static
        debug libvorbisfile_static_d optimized libvorbisfile_static
        debug Box2D_d optimized Box2D)
ENDIF()    

ADD_SUBDIRECTORY(Game)
#ADD_SUBDIRECTORY(Editor)

# Include dirs for Volt and VoltTest targets.
INCLUDE_DIRECTORIES(Volt Contrib Contrib/Box2D_v2.1.2/Box2D)    

file(GLOB_RECURSE Json_SOURCES Contrib/json/*.cpp)
ADD_LIBRARY(json STATIC ${Json_SOURCES})

file(GLOB_RECURSE Volt_SOURCES Volt/*.cpp)
ADD_LIBRARY(volt STATIC ${Volt_SOURCES})
TARGET_LINK_LIBRARIES(volt ${VOLT_DEPENDENCIES})
ADD_DEPENDENCIES(volt Box2D)

file(GLOB_RECURSE VoltTest_SOURCES Test/*.cpp)
ADD_EXECUTABLE(volt_test ${VoltTest_SOURCES})
TARGET_LINK_LIBRARIES(volt_test ${VOLT_DEPENDENCIES} volt)