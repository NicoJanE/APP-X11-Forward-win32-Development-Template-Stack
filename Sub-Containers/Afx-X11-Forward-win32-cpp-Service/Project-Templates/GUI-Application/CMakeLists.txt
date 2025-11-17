# General build instructions
# 1.) mkdir build
# 2.) cd build
# 3.) cmake ..						# configure 32 bits (default 32 bits)
# 3.) cmake .. -DUSE_64_BIT=ON		# configure 64 bits
#
# Alternatively:
#     3.1) cmake --build .
#     3.2) cmake --build . --target out32
#     3.3) cmake --build . --target out32 -- VERBOSE=1

#
# Edit: nano ../CMakeLists.txt -l
#


# Version and project name
# ==========================================================================================
cmake_minimum_required(VERSION 3.28)
project(Win32Project)

# ------------------------------------------------------------------------------------------


# Set the compiler and tools used to compile and build artifacts
# ------------------------------------------------------------------------------------------
set(CMAKE_C_COMPILER_64 x86_64-w64-mingw32-gcc)
set(CMAKE_RC_COMPILER_64 x86_64-w64-mingw32-windres)
set(CMAKE_C_COMPILER_32 i686-w64-mingw32-gcc)
set(CMAKE_RC_COMPILER_32 i686-w64-mingw32-windres)

option(USE_64_BIT "Use 64-bit compiler" OFF)  # Change ON to OFF for default 32-bit
if(USE_64_BIT)
    set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER_64})
    set(CMAKE_RC_COMPILER ${CMAKE_RC_COMPILER_64})
else()
    set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER_32})
    set(CMAKE_RC_COMPILER ${CMAKE_RC_COMPILER_32})
endif()


# ======================================================================


# Set the output directories for the compiled binary and objects
# ==========================================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# ------------------------------------------------------------------------------------------


# Define the base directories for headers and sources
# ==========================================================================================
#
# Set the base header variable (HEADER_DIR) for including sub-directories (recursive). 
# The first two lines set the root header directory, which is sufficient if all headers
# are in one directory.
#
# To support a hierarchy of directories like: headers/views, headers/models, headers/database,
# we need to recursively traverse the base header directory. This is handled below.
#
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/headers)
include_directories(${HEADER_DIR})
#	add recursive the other header files from other directories
file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/headers/*")
foreach(dir ${HEADER_DIRS})
  if(IS_DIRECTORY ${dir})
    include_directories(${dir})
  endif()
endforeach()
#
# Set the source variable (SOURCE_DIR) with all C source files, recursively in all 
# sub-directories 
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")
# ------------------------------------------------------------------------------------------


# RESOURCES
# Compile the resource files (.rc) into a .res file. The resulting .res file 
# will be in the binary directory next to the output executable.
# ==========================================================================================
#	Set the resource directory (RESOURCE_DIR)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resource)
#	Set the location of the .rc file to compile
set(RC_FILE "${RESOURCE_DIR}/Win32Template.rc")
#	Set the output name and location for the .res file
set(RES_FILE "${CMAKE_CURRENT_BINARY_DIR}/bin/Win32Template.res")
#
#	Add the resource file as a custom command to generate it
add_custom_command(
    OUTPUT ${RES_FILE}
    COMMAND ${CMAKE_RC_COMPILER} ${RC_FILE} -O coff -o ${RES_FILE}
    DEPENDS ${RC_FILE}
    COMMENT "Generating resource file: ${RES_FILE}"
)
message(STATUS "Resource file generated at: ${RES_FILE}")

# Run the custom resource command for all targets
add_custom_target(GenerateResources ALL DEPENDS ${RES_FILE})
# ------------------------------------------------------------------------------------------


# COMPILE & BUILD 
# ==========================================================================================
# Define the executable (out32), its sources, and build dependencies.
#	Note: We link the resource file via target_link_libraries (see below), 
#	instead of adding it directly to the executable with add_executable(), 
#	which failed.
add_executable(out32 ${SOURCES})
#
#	Set Compiler flags: DEFINES
target_compile_options(out32 PRIVATE -D_WIN32 -U_WIN64)
#	Set Compiler flags: We use Unicode
target_compile_options(out32 PRIVATE -DUNICODE -D_UNICODE)
#	Set Linker flags 
target_link_options(out32 PRIVATE -municode -mwindows)
#	Link Windows libraries
target_link_libraries(out32 PRIVATE user32 kernel32)
#
#	Link the compiled resources (.res file)
#	*** WARNING ***
#	Without this, the program builds and runs, but immediately
#	exits without displaying a GUI. GetLastError() = 1811 
#	in Windows API: CreateWindowW(...)
target_link_libraries(out32 PRIVATE ${RES_FILE})
#	
#
#	Ensure out32 depends on the resource generation target
add_dependencies(out32 GenerateResources)

# ------------------------------------------------------------------------------------------


# Clean up targets
# ==========================================================================================
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${RES_FILE})
