# Locate SDL2_gpu library
# This module defines
# SDL2_GPU_LIBRARY, the name of the library to link against
# SDL2_GPU_FOUND, if false, do not try to link to SDL2_gpu
# SDL2_GPU_INCLUDE_DIR, where to find SDL_gpu.h
#
# Additional Note: If you see an empty SDL2_GPU_LIBRARY_TEMP in your configuration
# and no SDL2_GPU_LIBRARY, it means CMake did not find your SDL2_Image library
# (SDL2_gpu.dll, libsdl2_gpu.so, SDL2_gpu.framework, etc).
# Set SDL2_GPU_LIBRARY_TEMP to point to your SDL2 library, and configure again.
# Similarly, if you see an empty SDL2MAIN_LIBRARY, you should set this value
# as appropriate. These values are used to generate the final SDL2_GPU_LIBRARY
# variable, but when these values are unset, SDL2_GPU_LIBRARY does not get created.
#
# $SDL2 is an environment variable that would
# correspond to the ./configure --prefix=$SDL2
# used in building SDL2.
# l.e.galup 9-20-02
#
# Modified by Adam Demuri from FindSDL2_image.cmake.

#=============================================================================
# Copyright 2003-2009 Kitware, Inc.
#
# CMake - Cross Platform Makefile Generator
# Copyright 2000-2014 Kitware, Inc.
# Copyright 2000-2011 Insight Software Consortium
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
# nor the names of their contributors may be used to endorse or promote
# products derived from this software without specific prior written
# permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
# License text for the above reference.)

FIND_PATH(SDL2_GPU_INCLUDE_DIR SDL_gpu.h
	HINTS
	${SDL2}
	$ENV{SDL2}
	$ENV{SDL2_GPU}
	PATH_SUFFIXES include/SDL2 include SDL2
	i686-w64-mingw32/include/SDL2
	x86_64-w64-mingw32/include/SDL2
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/include/SDL2
	/usr/include/SDL2
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
)

# Lookup the 64 bit libs on x64
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	FIND_LIBRARY(SDL2_GPU_LIBRARY_TEMP
		NAMES SDL2_gpu
		HINTS
		${SDL2}
		$ENV{SDL2}
		$ENV{SDL2_GPU}
		PATH_SUFFIXES lib64 lib
		lib/x64
		x86_64-w64-mingw32/lib
		PATHS
		/sw
		/opt/local
		/opt/csw
		/opt
	)
# On 32bit build find the 32bit libs
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
	FIND_LIBRARY(SDL2_GPU_LIBRARY_TEMP
		NAMES SDL2_gpu
		HINTS
		${SDL2}
		$ENV{SDL2}
		$ENV{SDL2_GPU}
		PATH_SUFFIXES lib
		lib/x86
		i686-w64-mingw32/lib
		PATHS
		/sw
		/opt/local
		/opt/csw
		/opt
	)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

SET(SDL2_GPU_FOUND "NO")
	IF(SDL2_GPU_LIBRARY_TEMP)
	# Set the final string here so the GUI reflects the final state.
	SET(SDL2_GPU_LIBRARY ${SDL2_GPU_LIBRARY_TEMP} CACHE STRING "Where the SDL2_gpu Library can be found")
	# Set the temp variable to INTERNAL so it is not seen in the CMake GUI
	SET(SDL2_GPU_LIBRARY_TEMP "${SDL2_GPU_LIBRARY_TEMP}" CACHE INTERNAL "")
	SET(SDL2_GPU_FOUND "YES")
ENDIF(SDL2_GPU_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_GPU REQUIRED_VARS SDL2_GPU_LIBRARY SDL2_GPU_INCLUDE_DIR)
message(STATUS "SDL2_gpu library: ${SDL2_GPU_LIBRARY}")
