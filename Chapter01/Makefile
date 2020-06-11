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

## Target name
TARGET = $(BUILDDIR)/a.out

## Compiler options
CC = clang
CFLAGS = -O2 -Wall -std=gnu11
CXX = clang
CXXFLAGS = -O2 -Wall -std=gnu++11
LDFLAGS =

SRCS := $(shell find $(SRCDIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -lfreetype -lpng16

SDL2INCS := -I../External/SDL/include
SDL2LIBS := -L../External/SDL/lib/mac
CPPFLAGS += $(SDL2INCS) -I.
LDFLAGS += $(SDL2LIBS) $(LIBS)

# Target
default:
	make all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

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
