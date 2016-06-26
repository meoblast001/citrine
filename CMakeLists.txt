cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

include(TestBigEndian)
include(CheckIncludeFile)

#Project name and version
project(Citrine)
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)

#Build shared
set(BUILD_SHARED_LIBS ON)

#Check dependencies
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)
find_library(LIBGLEW GLEW)
if (${LIBGLEW} MATCHES "LIBGLEW-NOTFOUND")
	message(FATAL_ERROR "GLEW not found")
endif ()
find_package(PNG REQUIRED)
find_library(LIBBULLETDYNAMICS BulletDynamics)
if (${LIBBULLETDYNAMICS} MATCHES "LIBBULLETDYNAMICS-NOTFOUND")
	message(FATAL_ERROR "Bullet Physics not found")
endif ()
find_package(OpenAL REQUIRED)
find_library(LIBVORBISFILE vorbisfile)
if (${LIBVORBISFILE} MATCHES "LIBVORBISFILE-NOTFOUND")
	message(FATAL_ERROR "VorbisFile not found")
endif ()

#Check endianness
test_big_endian(HAVE_BIG_ENDIAN)
#Check for Xorg
find_library(X11 XORG)
if (${XORG} MATCHES "XORG-NOTFOUND")
	set(HAVE_XORG FALSE)
else ()
	set(HAVE_XORG TRUE)
endif ()
#Check for NT
check_include_file(windows.h, HAVE_NT)
#Output results to config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

#Required for C++
add_definitions("-Wall -ansi -pedantic -fpermissive")

#Specify build source files
add_library(citrine SHARED
	src/csmartpointer.cpp
	src/cendianness.cpp
	src/cerror.cpp
	src/math/cvector.cpp
	src/math/cmatrix.cpp
	src/math/cquaternion.cpp
	src/math/ctransform.cpp
	src/entities/cworld.cpp
	src/entities/ccanvas.cpp
	src/entities/ccamera.cpp
	src/entities/cmeshdata.cpp
	src/entities/cmesh.cpp
	src/entities/csprite2d.cpp
	src/entities/csprite3d.cpp
	src/entities/clight.cpp
	src/entities/carmature.cpp
	src/images/cimage.cpp
	src/images/cpng.cpp
	src/images/ctexture.cpp
	src/contexts/ccontext.cpp
	src/contexts/crossplatform/ccrossplatform.cpp
	src/contexts/crossplatform/cxorg.cpp
	src/contexts/graphics/cgraphics.cpp
	src/contexts/graphics/cgraphicscontext.cpp
	src/contexts/events/ceventcontext.cpp
	src/physics/cphysics.cpp
	src/audio/caudiocontext.cpp
	src/audio/caudiosource.cpp
	src/audio/caudioinstance.cpp
	src/audio/csoundsource.cpp
	src/audio/cvorbis.cpp
	src/audio/cwav.cpp
	src/loaders/centityloader.cpp
	src/loaders/ccsm.cpp
	src/loaders/ccse.cpp
	src/shaders/cshader.cpp
	src/shaders/cshaderprogram.cpp)

#Add libraries
target_link_libraries(citrine ${OPENGL_LIBRARY} ${GLU_LIBRARY} ${LIBGLEW} ${PNG_LIBRARY} ${LIBBULLETDYNAMICS} ${OPENAL_LIBRARY} ${LIBVORBISFILE})

#Specify install headers
set(HEADER_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include/citrine)
set(HEADERS
	src/citrine.h
	src/csmartpointer.h
	src/cendianness.h
	src/math/cvector.h
	src/math/cmatrix.h
	src/math/cquaternion.h
	src/math/ctransform.h
	src/entities/cworld.h
	src/entities/ccanvas.h
	src/entities/ccamera.h
	src/entities/cvertex.h
	src/entities/cpoly.h
	src/entities/cmeshdata.h
	src/entities/cmesh.h
	src/entities/csprite2d.h
	src/entities/csprite3d.h
	src/entities/clight.h
	src/entities/carmature.h
	src/images/cimage.h
	src/images/ctexture.h
	src/contexts/ccontext.h
	src/contexts/graphics/cgraphicscontext.h
	src/contexts/events/ceventcontext.h
	src/contexts/events/ckeys.h
	src/physics/cphysics.h
	src/audio/caudiocontext.h
	src/audio/caudiosource.h
	src/audio/caudioinstance.h
	src/audio/csoundsource.h
	src/loaders/centityloader.h
	src/shaders/cshader.h
	src/shaders/cshaderprogram.h)

#Install headers
foreach (HEADER ${HEADERS})
	string(REGEX REPLACE "/[^/]*\$" "" DIRECTORY ${HEADER})
	string(REGEX REPLACE "\^src/*" "" DIRECTORY ${DIRECTORY})
	install(FILES ${HEADER} DESTINATION ${HEADER_INSTALL_PATH}/${DIRECTORY})
endforeach ()

#Install library
install(TARGETS citrine LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
