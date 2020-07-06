# From: https://note.com/funmylife/n/n7fe4068ba29b

# Makefile Template.

# .
# |-- Makefile (This file)
# |-- build
# |   `-- target file.
#     |-- obj
#         `-- object files.
# |-- header files.
# `-- source files.

# Variables
## Directory defines
BUILDDIR = ./build
OBJDIR = $(BUILDDIR)/obj
SRCDIR = .
EXTEDI = ../External
SDLDIR = $(EXTEDI)/SDL
GLEWDIR = $(EXTEDI)/GLEW
SOILDIR = $(EXTEDI)/SOIL
RJSONDIR = $(EXTEDI)/Rapidjson
FMODAPIDIR = $(EXTEDI)/FMOD/api
FMODSTUDIO = $(FMODAPIDIR)/studio
FMODCORE = $(FMODAPIDIR)/core

## Target name
TARGET = $(BUILDDIR)/a.out

## libGLEWのシンボリックリンク
SYMLINK = $(BUILDDIR)/libGLEW.2.1.0.dylib

## Compiler options
CC = clang
CFLAGS = -O2 -Wall -std=c17
CXX = clang++
CXXFLAGS = -O2 -Wall -std=c++11
LDFLAGS =

SRCS := $(shell find $(SRCDIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)
LIBS := -lSDL2-2.0.0 -lSDL2_image-2.0.0 -lGLEW.2.1.0 -lSOIL -lfmodL -lfmodstudioL
FRAMEWORKS := -framework CoreFoundation -framework OpenGL

INCDIRS := -I$(SDLDIR)/include -I$(GLEWDIR)/include -I$(SOILDIR)/include -I$(RJSONDIR)/include -I$(FMODCORE)/inc -I$(FMODSTUDIO)/inc
CPPFLAGS := $(INCDIRS) -I. -I/usr/local/opt/llvm/include

LIBDIRS := -L$(SDLDIR)/lib/mac -L$(GLEWDIR)/lib/mac -L$(SOILDIR)/lib/mac -L$(FMODCORE)/lib -L$(FMODSTUDIO)/lib
LDFLAGS := $(LIBDIRS) $(LIBS) $(FRAMEWORKS)

# Target
default:
	make all

all: $(TARGET) $(SYMLINK)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(SYMLINK): $(abspath $(GLEWDIR)/lib/mac/libGLEW.2.1.0.dylib)
	ln -s $(abspath $(GLEWDIR)/lib/mac/libGLEW.2.1.0.dylib) build/.

# assembly
$(OBJDIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(OBJDIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(OBJDIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean rebuild

clean:
	$(RM) -r $(BUILDDIR)

rebuild:
	make clean && make

-include $(DEPS)

MKDIR_P = mkdir -p
