# This file is part of: App-X11-Forward-win32-Development-Template-Stack
# Copyright (c) 2024 Nico Jan Eelhart
#
# This source code is licensed under the MIT License found in the  'LICENSE.md' file in the root directory of this source tree.
#
#
# Call:
#		 make out32

# Application Environment variables (scope during debug sessions) from file: .env
-include .env


#export OUT

# **Important** define the base header location, this make it easier to reference the header (i.e #include "views/Win32Template")
# See CFLAGS, you can add additional header base files (i.e:header_comapi=../../com_prj/headers)
headerbase = ./headers

OUT = _make_out
OBJDIR =$(OUT)/obj32

# C Compilers
CC32_COMP = i686-w64-mingw32-gcc

# Resource compiler
WINDRES32_COMP = i686-w64-mingw32-windres

# Compiler flags: headers and define unicode
CFLAGS_COMMON = -I$(headerbase) -DUNICODE -D_UNICODE


# Library flags (generic)
# for unicode version we must use the flag: -municode otherwise
# you get error that the WinMain could not be found
LDFLAGS_COMMON = -luser32 -lkernel32 -municode -mwindows
# Debug


# Get Header files: Instead of do the following:
# CFLAGS = -I./headers -I./headers/views -I./headers/controls -I./headers/models
# We use a shell command to discover all directory's all it's the sub directory's 
CFLAGS_COMMON += $(shell find ./headers -type d | sed 's/^/-I/')

# Get Source files: instead of using this:
# SRC = $(wildcard ./src/*.c ./src/views/*.c ./src/controls/*.c ./src/models/*.c)
# we let the find command search for all c files including sub directory's
SRC = $(shell find ./src -name '*.c')

# Define specific flags for debug and release
CFLAGS_DEBUG = $(CFLAGS_COMMON) -g 
CFLAGS_RELEASE = $(CFLAGS_COMMON)
LDFLAGS_DEBUG = $(LDFLAGS_COMMON) -g 
LDFLAGS_RELEASE = $(LDFLAGS_COMMON)



# Define the .rc file to compile and resulting .res file 
RC_FILE = ./resource/Win32Template.rc
RES32_FILE = $(OUT)/Win32Template32.res

OBJ32_FILES = $(SRC:.c=.o)


# General dependency tasks, called by targets
# ***************************************************************

# PRE-BUILD 32 bits. Compile resource file
$(RES32_FILE): $(RC_FILE)
	$(WINDRES32_COMP) $(RC_FILE) -O coff -o $(RES32_FILE)

	
	
# COMPILING source 32 bits
CC = $(CC32_COMP)  # Default to 32-bit compiler
%.o: %.c 
#	clear
	@echo "\n\nCOMPILE STAGE: " 
	@echo "\n\nplags: $(CFLAGS)\n\n"
	@echo " -> @$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@"
	@$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@	
	
# ***************************************************************
	
# ALL Targets
# ---------------------------------------------------------------
#.PHONY: .prepare

.prepare_cleanscr:
	clear	

# Create The same sub directorie structure in $(OBJDIR) as in ./src
# Suppress output(@) unless explicitly instructed (echo)
.prepare_obj_dir: 
	@echo "\n\n\tPREPARE STAGE: Create obj sub directories as in ./src! "
	@echo "\t- mkdir -p $(OBJDIR)"
	@mkdir -p $(OBJDIR)
	@echo "\t- find ./src -type d -exec mkdir -p $(OBJDIR)/{} \;"
	@find ./src -type d -exec mkdir -p $(OBJDIR)/{} \;
	           
	

all:   out32
##################################################################


# Target 32 bits Release
# ---------------------------------------------------------------
#
# Let source know this is 32 bits version(for entry point)
# Calls .prepare_* first
out32: CC = $(CC32_COMP) 
out32: CFLAGS = $(CFLAGS_RELEASE)
out32: LDFLAGS = $(LDFLAGS_RELEASE)
out32: .prepare_cleanscr .prepare_obj_dir $(OBJ32_FILES) $(RES32_FILE) 
	@echo "\n\nLINK STAGE (Releaes):  : $(OBJDIR)/$(OBJ32_FILES)  -->$(OUT)/$(APPLICATION_NAME) \n\t"
	@echo " -> $(CC) $(OBJDIR)/$(OBJ32_FILES) $(RES32_FILE) $(LDFLAGS) -o $(OUT)/$(APPLICATION_NAME)"
	@$(CC) $(OBJDIR)/$(OBJ32_FILES) $(RES32_FILE) $(LDFLAGS) -o $(OUT)/$(APPLICATION_NAME)

##################################################################


# Target 32 bits Debug
# ---------------------------------------------------------------
#
# Let source know this is 32 bits version(for entry point)
# Calls .prepare_* first
out32_debug: CC = $(CC32_COMP)
out32_debug: CFLAGS = $(CFLAGS_DEBUG)
out32_debug: LDFLAGS = $(LDFLAGS_DEBUG)
out32_debug: .prepare_cleanscr .prepare_obj_dir $(OBJ32_FILES) $(RES32_FILE)
	@echo "\n\nLINK STAGE (Debug):  : $(OBJDIR)/$(OBJ32_FILES)  -->$(OUT)/$(APPLICATION_NAME_DBG) \n\t"
	@echo " -> $(CC) $(OBJDIR)/$(OBJ32_FILES) $(RES32_FILE) $(LDFLAGS) -o  $(OUT)/$(APPLICATION_NAME_DBG)"	
	@$(CC) $(OBJDIR)/$(OBJ32_FILES) $(RES32_FILE) $(LDFLAGS) -o $(OUT)/$(APPLICATION_NAME_DBG)

##################################################################


# Clean up object files, resources, and the executable
# ---------------------------------------------------------------
clean: 
	rm -rf $(OBJDIR) $(RES32_FILE) $(OUT)/$(APPLICATION_NAME) $(OUT)/$(APPLICATION_NAME_DBG)
	
##################################################################	